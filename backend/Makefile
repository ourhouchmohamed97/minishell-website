NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
RM = rm -rf

BONUS_DIR = ./bonus
HEADER_BONUS = ./$(BONUS_DIR)/include/minishell_bonus.h
BONUS_LIBFT = ./$(BONUS_DIR)/libft

SRC_BONUS = $(BONUS_DIR)/main/main_bonus.c \
	$(BONUS_DIR)/main/init_bonus.c \
	$(BONUS_DIR)/main/free_bonus.c \
	$(BONUS_DIR)/main/memory_bonus.c \
	$(BONUS_DIR)/parsing/lexer/lexing_bonus.c \
	$(BONUS_DIR)/parsing/lexer/handle_parenth_bonus.c \
	$(BONUS_DIR)/parsing/lexer/lexing_handler_bonus.c \
	$(BONUS_DIR)/parsing/lexer/lexing_utils_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/tokenize_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/ft_word_splitting_after_expansion_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/ft_get_char_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/tokenize_utils_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/ft_expand_variables_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/ft_wildcard_expand_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/ft_wildcard_expand_utils_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/ft_expand_utils_bonus.c \
	$(BONUS_DIR)/parsing/tokenize/ft_handle_tilde_bonus.c \
	$(BONUS_DIR)/tree/build_tree_bonus.c \
	$(BONUS_DIR)/tree/counter_bonus.c \
	$(BONUS_DIR)/tree/stack_operator_bonus.c \
	$(BONUS_DIR)/tree/classification_bonus.c \
	$(BONUS_DIR)/tree/creation_bonus.c \
	$(BONUS_DIR)/tree/shunting_yard_bonus.c \
	$(BONUS_DIR)/tree/group_tokens_bonus.c \
	$(BONUS_DIR)/execution/execute_bonus.c \
	$(BONUS_DIR)/execution/pipe_bonus.c \
	$(BONUS_DIR)/execution/exec_operate_bonus.c \
	$(BONUS_DIR)/execution/external_cmd_bonus.c \
	$(BONUS_DIR)/execution/find_executable_bonus.c \
	$(BONUS_DIR)/execution/exec_utils_bonus.c \
	$(BONUS_DIR)/buildin/ft_cd_bonus.c \
	$(BONUS_DIR)/buildin/ft_echo_bonus.c \
	$(BONUS_DIR)/buildin/ft_env_to_list_bonus.c \
	$(BONUS_DIR)/buildin/ft_env_bonus.c \
	$(BONUS_DIR)/buildin/ft_exit_bonus.c \
	$(BONUS_DIR)/buildin/ft_export_utils_bonus.c \
	$(BONUS_DIR)/buildin/ft_export_bonus.c \
	$(BONUS_DIR)/buildin/ft_pwd_bonus.c \
	$(BONUS_DIR)/buildin/ft_unset_bonus.c \
	$(BONUS_DIR)/redirections/run_redirection_bonus.c \
	$(BONUS_DIR)/redirections/setup_redir_bonus.c \
	$(BONUS_DIR)/redirections/clean_heredoc_bonus.c \
	$(BONUS_DIR)/redirections/exec_heredoc_bonus.c \
	$(BONUS_DIR)/redirections/process_heredoc_bonus.c \
	$(BONUS_LIBFT)/ft_isalpha_bonus.c \
	$(BONUS_LIBFT)/ft_isdigit_bonus.c \
	$(BONUS_LIBFT)/ft_is_numeric_bonus.c \
	$(BONUS_LIBFT)/ft_isalnum_bonus.c \
	$(BONUS_LIBFT)/ft_isascii_bonus.c \
	$(BONUS_LIBFT)/ft_isprint_bonus.c \
	$(BONUS_LIBFT)/ft_strlen_bonus.c \
	$(BONUS_LIBFT)/ft_memset_bonus.c \
	$(BONUS_LIBFT)/ft_bzero_bonus.c \
	$(BONUS_LIBFT)/ft_memcpy_bonus.c \
	$(BONUS_LIBFT)/ft_memmove_bonus.c \
	$(BONUS_LIBFT)/ft_strlcpy_bonus.c \
	$(BONUS_LIBFT)/ft_strlcat_bonus.c \
	$(BONUS_LIBFT)/ft_strcpy_bonus.c \
	$(BONUS_LIBFT)/ft_strcat_bonus.c \
	$(BONUS_LIBFT)/ft_toupper_bonus.c \
	$(BONUS_LIBFT)/ft_tolower_bonus.c \
	$(BONUS_LIBFT)/ft_strchr_bonus.c \
	$(BONUS_LIBFT)/ft_strrchr_bonus.c \
	$(BONUS_LIBFT)/ft_strncmp_bonus.c \
	$(BONUS_LIBFT)/ft_memchr_bonus.c \
	$(BONUS_LIBFT)/ft_memcmp_bonus.c \
	$(BONUS_LIBFT)/ft_strnstr_bonus.c \
	$(BONUS_LIBFT)/ft_atoi_bonus.c \
	$(BONUS_LIBFT)/ft_calloc_bonus.c \
	$(BONUS_LIBFT)/ft_strdup_bonus.c \
	$(BONUS_LIBFT)/ft_substr_bonus.c \
	$(BONUS_LIBFT)/ft_strjoin_bonus.c \
	$(BONUS_LIBFT)/ft_strtrim_bonus.c \
	$(BONUS_LIBFT)/ft_split_bonus.c \
	$(BONUS_LIBFT)/ft_split_m_bonus.c \
	$(BONUS_LIBFT)/ft_itoa_bonus.c \
	$(BONUS_LIBFT)/ft_strmapi_bonus.c \
	$(BONUS_LIBFT)/ft_striteri_bonus.c \
	$(BONUS_LIBFT)/ft_putchar_fd_bonus.c \
	$(BONUS_LIBFT)/ft_putstr_fd_bonus.c \
	$(BONUS_LIBFT)/ft_putendl_fd_bonus.c \
	$(BONUS_LIBFT)/ft_putnbr_fd_bonus.c \
	$(BONUS_DIR)/get_next_line/get_next_line_bonus.c \
	$(BONUS_DIR)/get_next_line/get_next_line_utils_bonus.c

OBJ = $(SRC_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(HEADER_BONUS)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c $(HEADER_BONUS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re