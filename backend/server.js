// backend/server.js
const WebSocket = require('ws');
const pty = require('node-pty');
const path = require('path');
const fs = require('fs');

// Render provides PORT environment variable; fallback for local dev
const PORT = process.env.PORT || 8080;

// Path to your minishell binary
const SHELL_PATH = path.join(__dirname, 'minishell');

// Ensure the minishell binary exists and is executable
if (!fs.existsSync(SHELL_PATH)) {
  console.error(`Error: minishell binary not found at ${SHELL_PATH}`);
  process.exit(1);
}

try {
  fs.accessSync(SHELL_PATH, fs.constants.X_OK);
} catch {
  console.error(`Error: minishell binary is not executable. Run chmod +x minishell`);
  process.exit(1);
}

// Create WebSocket server
const wss = new WebSocket.Server({ port: PORT }, () => {
  console.log(`Minishell WebSocket server running on port ${PORT}`);
});

wss.on('connection', (ws) => {
  console.log('Client connected');
  let shell;

  try {
    shell = pty.spawn(SHELL_PATH, [], {
      name: 'xterm-color',
      cols: 80,
      rows: 24,
      cwd: process.cwd(),
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
    ws.on('message', (msg) => {
      try {
        const parsed = JSON.parse(msg);

        switch (parsed.type) {
          case 'input':
            shell.write(parsed.data);
            break;
          case 'resize':
            shell.resize(parsed.cols, parsed.rows);
            break;
          case 'signal':
            switch (parsed.signal) {
              case 'SIGINT':
              case 'SIGTSTP':
              case 'SIGQUIT':
                shell.kill(parsed.signal);
                break;
              case 'EOF':
                shell.write('\x04');
                break;
            }
            break;
          default:
            // fallback
            if (parsed.command) shell.write(parsed.command + '\n');
            break;
        }
      } catch (err) {
        ws.send(JSON.stringify({ type: 'error', data: 'Invalid message format' }));
      }
    });

    ws.on('close', () => {
      console.log('Client disconnected');
      if (shell) shell.kill();
    });

    ws.send(JSON.stringify({ type: 'output', data: 'Minishell connected successfully!\r\n$ ' }));
  } catch (err) {
    console.error('Error spawning shell:', err);
    ws.send(JSON.stringify({ type: 'error', data: `Server error: ${err.message}` }));
    ws.close();
  }
});

wss.on('error', (err) => {
  console.error('WebSocket server error:', err);
});
