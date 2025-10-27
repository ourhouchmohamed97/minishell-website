// backend/server.js
const WebSocket = require('ws');
const pty = require('node-pty');
const path = require('path');

const PORT = process.env.PORT || 10000;

const wss = new WebSocket.Server({ port: PORT });
console.log(`Minishell WebSocket server running on port ${PORT}`);

wss.on('connection', (ws) => {
  const shellPath = path.join(__dirname, 'minishell');

  // Spawn minishell with node-pty (provides TTY)
  const shell = pty.spawn(shellPath, [], {
    name: 'xterm-color',
    cols: 80,
    rows: 24,
    cwd: __dirname,
    env: process.env
  });

  // Send output to client
  shell.onData(data => ws.send(JSON.stringify({ type: 'output', data })));

  // Handle commands from client
  ws.on('message', message => {
    try {
      const parsed = JSON.parse(message);
      if (parsed.type === 'input') shell.write(parsed.data);
      else if (parsed.type === 'resize') shell.resize(parsed.cols, parsed.rows);
      else if (parsed.type === 'signal') shell.write(parsed.signal === 'EOF' ? '\x04' : '');
    } catch (err) {
      ws.send(JSON.stringify({ type: 'error', data: 'Invalid message' }));
    }
  });

  ws.on('close', () => shell.kill());
  
  // Welcome message
  ws.send(JSON.stringify({ type: 'output', data: 'Minishell connected!\r\n$ ' }));
});
