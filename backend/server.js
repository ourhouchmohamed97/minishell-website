// backend/server.js
const WebSocket = require('ws');
const pty = require('node-pty');
const path = require('path');
const fs = require('fs');

const wss = new WebSocket.Server({ port: process.env.PORT || 8080 });

console.log('Minishell WebSocket server running on port', process.env.PORT || 8080);

wss.on('connection', (ws) => {
  let shell;
  let currentDirectory = __dirname;

  try {
    // Check if minishell binary exists, fall back to system shell
    let shellPath = './minishell';
    let shellArgs = [];
    
    if (!fs.existsSync(shellPath)) {
      console.log('Minishell not found, using system shell');
      shellPath = process.platform === 'win32' ? 'cmd.exe' : '/bin/bash';
      shellArgs = ['--login'];
    }

    // Use node-pty for proper TTY
    shell = pty.spawn(shellPath, shellArgs, {
      name: 'xterm-color',
      cols: 80,
      rows: 24,
      cwd: currentDirectory,
      env: process.env
    });

    // Send shell output to client
    shell.onData((data) => {
      if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ 
          type: 'output', 
          data: data
        }));
      }
    });

    // Handle shell exit
    shell.onExit(({ exitCode, signal }) => {
      if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({
          type: 'exit',
          data: `Shell exited with code ${exitCode}`
        }));
        ws.close();
      }
    });

    // Handle commands from client
    ws.on('message', (message) => {
      try {
        const parsed = JSON.parse(message);
        
        if (parsed.type === 'input') {
          shell.write(parsed.data);
        } else if (parsed.type === 'resize') {
          shell.resize(parsed.cols, parsed.rows);
        } else if (parsed.type === 'signal') {
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
        }
      } catch (error) {
        if (ws.readyState === WebSocket.OPEN) {
          ws.send(JSON.stringify({
            type: 'error',
            data: 'Invalid message format'
          }));
        }
      }
    });

    // Handle client disconnect
    ws.on('close', () => {
      if (shell) {
        shell.kill();
      }
    });

    // Send welcome message
    if (ws.readyState === WebSocket.OPEN) {
      const welcomeMsg = shellPath === './minishell' 
        ? 'Minishell connected successfully!\r\n'
        : 'System shell connected (minishell not found)\r\n';
      
      ws.send(JSON.stringify({
        type: 'output',
        data: welcomeMsg
      }));
    }

  } catch (error) {
    console.error('Shell spawn error:', error);
    if (ws.readyState === WebSocket.OPEN) {
      ws.send(JSON.stringify({
        type: 'error',
        data: `Server error: ${error.message}`
      }));
      ws.close();
    }
  }
});