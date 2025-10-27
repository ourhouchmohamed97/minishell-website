// backend/server.js
const WebSocket = require('ws');
const pty = require('node-pty');
const path = require('path');
const fs = require('fs');

// Use Render's assigned port or fallback to 8080 for local testing
const PORT = process.env.PORT || 8080;

const wss = new WebSocket.Server({ port: PORT });

console.log(`Minishell WebSocket server running on port ${PORT}`);

wss.on('connection', (ws) => {
  let shell;
  const currentDirectory = __dirname;

  try {
    // Spawn minishell using node-pty for proper TTY
    shell = pty.spawn(path.join(currentDirectory, 'minishell'), [], {
      name: 'xterm-color',
      cols: 80,
      rows: 24,
      cwd: currentDirectory,
      env: process.env,
    });

    // Send shell output to client
    shell.onData((data) => {
      ws.send(JSON.stringify({ type: 'output', data }));
    });

    // Handle shell exit
    shell.onExit(({ exitCode }) => {
      ws.send(JSON.stringify({
        type: 'exit',
        data: `Shell exited with code ${exitCode}\r\n`,
      }));
    });

    // Handle messages from client
    ws.on('message', (message) => {
      try {
        const parsed = JSON.parse(message);

        switch (parsed.type) {
          case 'input':
            shell.write(parsed.data);
            break;
          case 'resize':
            shell.resize(parsed.cols, parsed.rows);
            break;
          case 'signal':
            // Handle Ctrl+C, Ctrl+Z, Ctrl+D, etc.
            switch (parsed.signal) {
              case 'SIGINT':
              case 'SIGTSTP':
              case 'SIGQUIT':
                shell.kill(parsed.signal);
                break;
              case 'EOF':
                shell.write('\x04'); // End-of-file (Ctrl+D)
                break;
            }
            break;
          default:
            // Fallback: treat as command
            shell.write(parsed.command + '\n');
            break;
        }
      } catch (err) {
        ws.send(JSON.stringify({
          type: 'error',
          data: 'Invalid message format',
        }));
      }
    });

    // Handle client disconnect
    ws.on('close', () => {
      if (shell) shell.kill();
    });

    // Send welcome message
    ws.send(JSON.stringify({
      type: 'output',
      data: 'Minishell connected successfully!\r\n$ ',
    }));

  } catch (error) {
    ws.send(JSON.stringify({
      type: 'error',
      data: `Server error: ${error.message}`,
    }));
    ws.close();
  }
});
