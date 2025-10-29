const WebSocket = require('ws');
const pty = require('node-pty');
const http = require('http');

// Create HTTP server for health checks
const server = http.createServer((req, res) => {
  if (req.url === '/health') {
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ 
      status: 'ok', 
      service: 'minishell-websocket',
      timestamp: new Date().toISOString()
    }));
    return;
  }
  
  res.writeHead(404, { 'Content-Type': 'application/json' });
  res.end(JSON.stringify({ error: 'Not found' }));
});

const PORT = process.env.PORT || 10000;

// Create WebSocket server attached to HTTP server
const wss = new WebSocket.Server({ server });

console.log(`Minishell WebSocket server running on port ${PORT}`);

wss.on('connection', (ws) => {
  let shell;
  let currentDirectory = process.cwd();

  try {
    console.log('New WebSocket connection, spawning minishell...');
    
    // Use the minishell binary we built in the Dockerfile
    shell = pty.spawn('./minishell', [], {
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
      console.log(`Shell exited with code ${exitCode}`);
      if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({
          type: 'exit',
          data: `Shell exited with code ${exitCode}`
        }));
      }
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
        console.error('Error handling message:', error);
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
      console.log('WebSocket connection closed');
      if (shell) {
        shell.kill();
      }
    });

    // Handle errors
    ws.on('error', (error) => {
      console.error('WebSocket error:', error);
    });

    // Send welcome message
    if (ws.readyState === WebSocket.OPEN) {
      ws.send(JSON.stringify({
        type: 'output',
        data: 'Minishell connected successfully!\r\n'
      }));
    }

  } catch (error) {
    console.error('Error spawning shell:', error);
    if (ws.readyState === WebSocket.OPEN) {
      ws.send(JSON.stringify({
        type: 'error',
        data: `Server error: ${error.message}`
      }));
      ws.close();
    }
  }
});

// Start the server
server.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
  console.log(`Health check available at: http://localhost:${PORT}/health`);
});

// Graceful shutdown
process.on('SIGTERM', () => {
  console.log('Received SIGTERM, shutting down gracefully');
  server.close(() => {
    console.log('Server closed');
    process.exit(0);
  });
});