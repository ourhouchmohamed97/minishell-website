/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:45:20 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/20 15:56:22 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <termios.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <dirent.h>
# include <sys/stat.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

// ================= structs =================

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	AND,
	OR,
	PAREN_OPEN,
	PAREN_CLOSE
}					t_token_type;

typedef struct s_match_list
{
	char				*filename;
	struct s_match_list	*next;
}					t_match_list;

typedef struct s_token
{
	char			*val;
	t_token_type	type;
	int				need_join;
	int				have_quote;
	int				quote_type;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_list_to_array
{
	t_env	*curr;
	char	**arr;
	char	*tmp;
	int		i;
	int		len;
}			t_list_to_array;

typedef struct s_export
{
	int		from_her;
	int		exit_status;
	int		braces;
	int		*i;
	char	**env;
	char	*tmp1;
	char	*tmp2;
	char	*home;
	t_token	*node;
	t_env	*node_env;
}					t_export;

typedef struct s_add_or_update_env
{
	char	*name;
	char	*eq_ptr;
	int		type;
}					t_add_or_update_env;

typedef struct s_redirection
{
	t_token_type			type;
	char					*filename;
	int						fd;
	int						quote_type;
	struct s_redirection	*next;
}		t_redirection;

typedef struct s_tree_node
{
	t_token_type		type;
	char				*val;
	t_redirection		*redirection;
	char				*cmd;
	char				**args;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
}						t_tree_node;

typedef struct s_minishell
{
	t_tree_node			*ast;
	int					exit_status;
	t_env				*env_list;
	int					is_interactive;
	char				*input;
}					t_minishell;

typedef struct s_node_stack
{
	t_tree_node	**nodes;
	int			top;
}	t_node_stack;

typedef struct s_token_stack
{
	t_token	**tokens;
	int		top;
}	t_oper_stack;

typedef struct s_pipe
{
	int		type;
	int		fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;
	int		sig_right;
}	t_pipe;

typedef struct s_args
{
	int	arg_count;
	int	argc;
}	t_args;

// ================= Tree =================

// ----------------- builder_tree.c -----------------
void			handle_word_token(t_token *current, t_node_stack *stack);
int				handle_operator_token(t_token *current, t_node_stack *stack);
void			handle_redirection_token(t_tree_node *node, t_token **cur,
					t_token_type redir_type);
t_tree_node		*construct_ast(t_token **postfix);
t_tree_node		*build_tree(t_token *tokens);

// ----------------- classification.c -----------------
int				get_precedence(t_token_type type);
bool			is_operator(t_token_type type);
bool			is_redirection(t_token_type type);
t_tree_node		*init_command_node(void);
int				process_command(t_tree_node *node, char **args, t_token **cur,
					t_args *arg_struct);

// ----------------- counter.c -----------------
int				count_args(t_token *tokens);
int				count_token_array(t_token **tokens);
int				count_total_tokens(t_token *tokens);
int				count_operators(t_token **tokens);

// ----------------- creation.c -----------------
t_node_stack	*create_node_stack(int capacity);
t_oper_stack	*create_oper_stack(int capacity);
t_tree_node		*create_operator_node(t_token_type type);
t_tree_node		*create_command_with_args_and_redirections(t_token *tokens,
					t_token **next_token);
t_token			*create_cmd_token(t_token *token, t_token **next);

// ----------------- group_tokens.c -----------------
t_token			**fill_token_array(t_token *tokens, int count);
t_token			**group_tokens(t_token **arr, int count);
int				is_simple_command_start(t_token *token);
t_token			**tokens_to_infix(t_token *tokens);

// ----------------- shunting_yard.c -----------------
void			pop_until_paren(t_oper_stack *stack, t_token **output,
					int *out_pos);
void			pop_ops_by_precedence(t_oper_stack *stack, t_token **output,
					t_token *current, int *out_pos);
void			shunting_process_token(t_token *current, t_oper_stack *stack,
					t_token **output, int *out_pos);
void			pop_remaining_ops(t_oper_stack *stack, t_token **output,
					int *out_pos);
t_token			**infix_to_postfix(t_token **tokens);

// ----------------- stack_operator.c -----------------
void			push_node(t_node_stack *stack, t_tree_node *node);
t_tree_node		*pop_node(t_node_stack *stack);
void			push_oper(t_oper_stack *stack, t_token *token);
t_token			*pop_oper(t_oper_stack *stack);

// ================= redirection =================

// ----------------- setup_redir.c  -----------------
int				setup_redirections(t_tree_node *node);
int				handle_input_redirection(t_redirection *redir);
int				handle_output_redirection(t_redirection *redir);
int				handle_append_redirection(t_redirection *redir);
int				handle_heredoc_redirection(t_redirection *redir);

// ----------------- clean_heredoc.c  -----------------
void			cleanup_heredoc_fds(t_tree_node *ast_root);
void			cleanup_redirection_heredoc_fds(t_redirection *redir_list);
void			cleanup_node_heredoc_fds(t_tree_node *node);

// ----------------- exec_heredoc.c  -----------------
char			*heredoc_expand(char *input_line, t_minishell *shell);
void			execute_heredoc(char *delimiter, int write_fd,
					t_minishell *shell, int should_expand);
int				handle_fork_error(int pipe_fds[2]);
int				wait_heredoc_child(pid_t child_pid, int pipe_fd,
					int *exit_status);
int				create_single_heredoc(t_redirection *heredoc_redir,
					t_minishell *shell, int should_expand,
					int *exit_status);

// ----------------- process_heredoc.c  -----------------
int				prepare_all_heredocs(t_tree_node *ast_root, t_minishell *shell);
int				process_node_heredocs(t_tree_node *node, t_minishell *shell);
int				process_redirections_heredocs(t_redirection *redir_list,
					t_minishell *shell);
void			handle_heredoc_signal(int signal_num);
int				handle_redirection(t_redirection *redir);

// ----------------- run_builtin.c  -----------------
int				run_builtin(t_tree_node *node, t_minishell *data, int f_p);
void			restore_fds(int saved_stdout, int saved_stdin);
void			set_default_signals(void);

// ================= parsing/lexer =================

// ----------------- lexing.c  -----------------
int				ft_lexing(t_minishell *shell, char *input);
int				ft_handle_close_parenth(char *input, int i);
int				ft_test_parenth(char *input, int *i, int *last);

// ----------------- handle_parenth.c  -----------------
int				handle_parenth(char *input, int *i);
int				only_spaces(char *str, int len);
int				ft_put_error(int type);
int				ft_handle_tow_or_more(char *input, int *i);

// ----------------- lexing_utils.c  -----------------
void			skip_whitespace(char *input, int *i);
int				is_valid_var_char(char c);
void			skip_word(char *input, int *i);
void			operator_error(char *s, int i);

// ----------------- lexing_handler.c  -----------------
int				handle_operator(char *input, int *i);
int				handle_dollar_brace(char *input, int *i);
int				handle_quote(char *input, int *i, char quote_char, int *last);
int				parse_operator(t_minishell *shell, char *input,
					int *i, int *last);
int				ft_test(char *input, int j, int last, t_minishell *shell);

// ================= parsing/tokenize =================

// ----------------- tokenize.c  -----------------
t_token			*ft_tokenize(char *input);
void			ft_add_type(t_token *node);
int				ft_helper_substr(char *str, int *i, int flag, char stock_flag);
int				ft_substr_m(char *str, char **copy, int *i, int *quote_type);
int				ft_add_back(t_token **head, t_token node);

//------------------ tokenize_utils.c ------------------
int				ft_is_space(char c);
int				ft_skip_space(char *str, int *i);
int				ft_limiter(char c);
int				ft_bad_space(char *str);
void			ft_clean_tokens(t_token **head);

//-------------------ft_handle_tilde.c-------------------
int				ft_handle_tilde(t_token **head, char **env);
int				ft_join_tokens(t_token **head);
void			ft_stock_limiter(char *str, t_token *node, int *i);
int				ft_init_tilde(t_export *var, t_token **head,
					char **env, int *i);
int				ft_replace_tilde(t_export *var, int *i);

//-------------------ft_get_char.c-------------------
int				ft_condition(char *str, int *i, char c, int double_q);
int				ft_init_condition(int *s_q, int *d_q, int q_type, char c);
int				ft_get_c(char *str, int *i, char c, t_export *v);
int				ft_valide_parenth(t_token *head);
int				ft_helper_parenth(t_token *head, t_token_type last_type);
// ----------------- ft_expand_variables.c  -----------------
int				ft_expand_variables(t_token **head, t_minishell *data,
					int from_her);
int				ft_replace_word(t_export *var, int *i);
int				ft_init_export(t_export *var, t_token **head, t_minishell *data,
					int *i);

// ----------------- ft_expand_variables_utils.c  -----------------
int				ft_replace_variable(t_export var);
int				ft_replace_question_mark(t_export var);
int				ft_replace(t_export var, int len, char *value);
int				ft_long_name(char *str, int i, int braces);
char			*ft_compare_env(t_export var, int len);

// ------------------ ft_word_splitting_after_expansion.c ------------------
void			ft_word_splitting_after_expansion(t_token **head, char **env);
t_token			*split_single_token(t_token *token, t_token **head);
int				ft_should_split_token(t_token *token);
t_token			*get_tail_token(t_token *head);
t_token			*create_word_token(char *word);
t_token			*ft_create_token_chain(char **words);
void			link_new_tokens(t_token *prev, t_token *new_head,
					t_token *new_tail, t_token *next);

// ================= execution =================

// ----------------- execution.c  -----------------
int				execute_tree(t_tree_node *node, t_minishell *data,
					int from_pipe);
int				is_builtin(char *cmd);
int				exec_builtin(t_tree_node *node, t_minishell *data, int f_p);
int				handle_command_not_found(t_tree_node *node, int from_pipe);
int				fork_exec_child(t_tree_node *node, char **env, char *path,
					int from_pipe);

// ----------------- external_cmd.c  -----------------
int				exec_child_process(t_tree_node *node, char **env, char *path);
int				exec_parent_process(pid_t pid, char *path);
void			exec_not_found(char *cmd);
int				exec_external(t_tree_node *node, char **env, int from_pipe);
void			add_redir_to_node(t_tree_node *node, t_redirection *redir);

// ----------------- find_executable.c  -----------------
char			*try_direct_path(const char *cmd);
char			*find_in_path_env(char **env);
char			*search_in_paths(char **paths, const char *cmd);
char			*find_executable(char *cmd, char **env, int *is_direct);
void			print_no_such_file(const char *cmd);

// ----------------- builtins.c  -----------------
int				ft_cd(t_tree_node *node, t_minishell *data);
int				ft_echo(t_tree_node *node);
int				ft_exit(t_tree_node *node, t_minishell *data, int f_p);
int				ft_export(t_tree_node *node, t_minishell *data);
int				ft_pwd(void);
int				ft_unset(t_tree_node *node, t_minishell *data);
int				ft_env(t_tree_node *node, t_minishell *data);

// ----------------- exec_operate.c  -----------------
int				execute_command(t_tree_node *node, char **env,
					t_minishell *data, int from_pipe);
int				execute_pipe(t_tree_node *node, t_minishell *data);
int				execute_and(t_tree_node *node, t_minishell *data);
int				execute_or(t_tree_node *node, t_minishell *data);

// ----------------- external_cmds.c  -----------------
void			add_redir_to_node(t_tree_node *node, t_redirection *redir);

// ------------------- ft_handle_tilde.c-------------------
int				ft_handle_tilde(t_token **head, char **env);
int				ft_join_tokens(t_token **head);
void			ft_stock_limiter(char *str, t_token *node, int *i);

// ------------------- ft_wildcard_expand.c-------------------
void			ft_wildcard_expand(t_token **head);

// -------------------- ft_wildcard_utils.c --------------------
int				match(const char *pattern, const char *name);
void			sorted_insert_match(t_match_list **head,
					t_match_list *new_node);
t_match_list	*new_match_node(const char *filename);
void			free_match_list(t_match_list *head);

// ================= main =================

// ----------------- main.c  -----------------
void			handle_sigint(int sig);
void			handle_sigquit(int sig);
void			run_command_line(char *input, t_minishell *data);
int				ft_isspace(unsigned char c);

// ----------------- init.c  -----------------
t_tree_node		*parse_ast(char *input, t_minishell *data);
int				init_minishell(t_minishell *data, char **env);
char			*get_shell_input(int is_interactive, t_minishell *data);
void			free_env(char **env);
int				ft_is_non_empty(char *input);

// ----------------- free.c  -----------------
void			free_redirection_node(t_redirection *r);
void			free_redirections(t_redirection *head);

// ----------------- ft_cd.c -----------------
int				set_env_value(t_env **head, const char *name,
					const char *value);
int				add_new_node(t_env **head, const char *name, char *value);

// ----------------- ft_env.c -----------------
void			ft_increment_shell_level(t_minishell *data);

// ----------------- ft_env_to_list.c -----------------
t_env			*ft_new_env_node(const char *env_var);
t_env			*ft_env_to_list(char **env);
void			free_env_list(t_env *head);
void			add_node_to_end(t_env **head, t_env *new_node);
t_env			*find_env_node(t_env *head, const char *name);
void			free_double_array(char **arr);

//-------------------- ft_export_utils.c --------------------
char			**ft_list_to_array(t_env *env_list);
void			sorted_insert(t_env **head, t_env *node_to_insert);
t_env			*create_sorted_copy(t_env *main_env_list);
int				ft_is_vide(char **args);

// ================= memory management =================
void			free_token_list(t_token *head);
t_tree_node		*free_ast(t_tree_node *node);
void			free_double_token_list(t_token **arr);
void			free_parser_arrays(t_token **grouped, t_token **postfix);

int				execute_redirection_only_command(t_tree_node *node);

#endif