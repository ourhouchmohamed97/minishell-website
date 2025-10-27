// backend/server.js
const WebSocket = require('ws');
const pty = require('node-pty'); // ← ADD THIS
const path = require('path');
const fs = require('fs');

const wss = new WebSocket.Server({ port: 8080 });

console.log('Minishell WebSocket server running on port 8080');

wss.on('connection', (ws) => {
  let shell;
  let currentDirectory = __dirname;

  try {
    // Use node-pty instead of child_process.spawn for proper TTY
    shell = pty.spawn('./minishell', [], {
      name: 'xterm-color',
      cols: 80,
      rows: 24,
      cwd: currentDirectory,
      env: process.env
    });

    // Send shell output to client
    shell.onData((data) => {
      ws.send(JSON.stringify({ 
        type: 'output', 
        data: data
      }));
    });

    // Handle shell exit
    shell.onExit(({ exitCode, signal }) => {
      ws.send(JSON.stringify({
        type: 'exit',
        data: `Shell exited with code ${exitCode}`
      }));
    });

    // Handle commands and signals from client
    ws.on('message', (message) => {
      try {
        const parsed = JSON.parse(message);
        
        if (parsed.type === 'input') {
          shell.write(parsed.data);
        } else if (parsed.type === 'resize') {
          shell.resize(parsed.cols, parsed.rows);
        } else if (parsed.type === 'signal') {
          // Handle signals (Ctrl+C, Ctrl+D, etc.)
          switch(parsed.signal) {
            case 'SIGINT':
              shell.kill('SIGINT');
              break;
            case 'SIGTSTP':
              shell.kill('SIGTSTP');
              break;
            case 'SIGQUIT': 
              shell.kill('SIGQUIT');
              break;
            case 'EOF':
              shell.write('\x04');
              break;
          }
        } else {
          shell.write(parsed.command + '\n');
        }
      } catch (error) {
        ws.send(JSON.stringify({
          type: 'error',
          data: 'Invalid message format'
        }));
      }
    });

    // Handle client disconnect
    ws.on('close', () => {
      if (shell) {
        shell.kill();
      }
    });

    // Send welcome message
    ws.send(JSON.stringify({
      type: 'output',
      data: 'Minishell connected successfully!\r\n'
    }));

  } catch (error) {
    ws.send(JSON.stringify({
      type: 'error',
      data: `Server error: ${error.message}`
    }));
    ws.close();
  }
});