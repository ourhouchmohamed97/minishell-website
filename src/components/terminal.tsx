// components/Terminal.tsx
'use client';

import { useState, useRef, useEffect, useCallback } from 'react';
import { Terminal as XTerm } from 'xterm';
import { FitAddon } from '@xterm/addon-fit';
import 'xterm/css/xterm.css';

const MAX_RECONNECT_ATTEMPTS = 5;
const RECONNECT_DELAY = 2000;
const TERMINAL_INIT_DELAY = 100;

export default function Terminal() {
  const [isConnected, setIsConnected] = useState(false);
  const [connectionError, setConnectionError] = useState<string>('');
  const [reconnectAttempts, setReconnectAttempts] = useState(0);
  const [isReconnecting, setIsReconnecting] = useState(false);
  const [isTerminalReady, setIsTerminalReady] = useState(false);

  const wsRef = useRef<WebSocket | null>(null);
  const terminalRef = useRef<HTMLDivElement>(null);
  const xtermRef = useRef<XTerm | null>(null);
  const fitAddonRef = useRef<FitAddon | null>(null);
  const reconnectTimeoutRef = useRef<NodeJS.Timeout | null>(null);

  // Terminal config
  const TERMINAL_CONFIG = {
    cursorBlink: true,
    theme: {
      background: '#0f172a',
      foreground: '#e2e8f0',
      cursor: '#e2e8f0',
      selectionBackground: '#1e293b'
    },
    fontSize: 15,
    fontFamily: '"Fira Code", "Cascadia Code", monospace',
    lineHeight: 1.2,
    scrollback: 10000,
    allowTransparency: false,
  };

  // Initialize terminal
  useEffect(() => {
    if (!terminalRef.current || xtermRef.current) return;

    const timeout = setTimeout(() => {
      const terminal = new XTerm(TERMINAL_CONFIG);
      const fitAddon = new FitAddon();
      terminal.loadAddon(fitAddon);
      fitAddon.fit();

      xtermRef.current = terminal;
      fitAddonRef.current = fitAddon;
      setIsTerminalReady(true);

      // Handle input
      terminal.onData((data) => {
        if (wsRef.current?.readyState === WebSocket.OPEN) {
          wsRef.current.send(JSON.stringify({ type: 'input', data }));
        }
      });

      // Custom key events (Ctrl+C, Ctrl+D, etc.)
      terminal.attachCustomKeyEventHandler((event) => {
        if (event.type !== 'keydown' || !event.ctrlKey) return true;
        if (wsRef.current?.readyState !== WebSocket.OPEN) return true;

        switch (event.code) {
          case 'KeyC':
            wsRef.current.send(JSON.stringify({ type: 'signal', signal: 'SIGINT' }));
            return false;
          case 'KeyD':
            wsRef.current.send(JSON.stringify({ type: 'signal', signal: 'EOF' }));
            return false;
          case 'KeyL':
            terminal.clear();
            return false;
          default:
            return true;
        }
      });

    }, TERMINAL_INIT_DELAY);

    return () => {
      clearTimeout(timeout);
      if (xtermRef.current) xtermRef.current.dispose();
      xtermRef.current = null;
    };
  }, []);

  // Handle WebSocket messages
  const handleWebSocketMessage = useCallback((data: any, terminal: XTerm) => {
    switch (data.type) {
      case 'output':
        terminal.write(data.data);
        break;
      case 'error':
        terminal.write(`\x1b[1;31m${data.data}\x1b[0m`);
        break;
      case 'exit':
        terminal.write(`\r\n\x1b[1;33m${data.data}\x1b[0m\r\n`);
        break;
    }
  }, []);

  // Connect WebSocket
  const connectWebSocket = useCallback(() => {
    if (wsRef.current?.readyState === WebSocket.OPEN || !isTerminalReady) return;

    const websocket = new WebSocket('wss://minishell-website.onrender.com'); // ← Update this URL

    websocket.onopen = () => {
      setIsConnected(true);
      setConnectionError('');
      setReconnectAttempts(0);
      setIsReconnecting(false);
    };

    websocket.onmessage = (event) => {
      const terminal = xtermRef.current;
      if (!terminal) return;
      try {
        handleWebSocketMessage(JSON.parse(event.data), terminal);
      } catch (err) {
        console.error('Error parsing WS message:', err);
      }
    };

    websocket.onclose = (event) => {
      setIsConnected(false);
      wsRef.current = null;

      if (!event.wasClean && reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        setIsReconnecting(true);
        reconnectTimeoutRef.current = setTimeout(() => {
          setReconnectAttempts(prev => prev + 1);
          connectWebSocket();
        }, RECONNECT_DELAY);
      } else if (!event.wasClean) {
        setConnectionError('Connection failed');
        setIsReconnecting(false);
      }
    };

    websocket.onerror = () => {
      setIsConnected(false);
    };

    wsRef.current = websocket;
  }, [reconnectAttempts, isTerminalReady, handleWebSocketMessage]);

  useEffect(() => {
    if (isTerminalReady) connectWebSocket();

    return () => {
      if (reconnectTimeoutRef.current) clearTimeout(reconnectTimeoutRef.current);
      if (wsRef.current) wsRef.current.close();
    };
  }, [isTerminalReady, connectWebSocket]);

  // Resize terminal on window resize
  useEffect(() => {
    const handleResize = () => {
      const terminal = xtermRef.current;
      const fitAddon = fitAddonRef.current;
      if (terminal && fitAddon && wsRef.current?.readyState === WebSocket.OPEN) {
        fitAddon.fit();
        wsRef.current.send(JSON.stringify({ type: 'resize', cols: terminal.cols, rows: terminal.rows }));
      }
    };
    window.addEventListener('resize', handleResize);
    return () => window.removeEventListener('resize', handleResize);
  }, []);

  // Retry connection
  const handleRetryConnection = () => {
    setConnectionError('');
    setReconnectAttempts(0);
    setIsReconnecting(false);
    connectWebSocket();
  };

  return (
    <div className="w-full max-w-5xl mx-auto p-4">
      <div className="bg-slate-900 rounded-lg shadow-xl overflow-hidden border border-slate-700">
        {/* Header */}
        <div className="bg-gradient-to-r from-[#0f0f70] via-[#131380] to-[#0f0f70] px-6 py-4 border-b border-blue-500/50 flex items-center justify-between">
          <div className="flex items-center space-x-3">
            <div className={`w-3 h-3 rounded-full shadow-lg ${
              isConnected ? 'bg-green-400' : isReconnecting ? 'bg-yellow-400' : 'bg-red-400'
            }`}></div>
            <span className="text-white font-mono text-sm font-bold">minishell</span>
          </div>
        </div>

        {/* Terminal Area */}
        <div className="w-full h-96 bg-[#0f172a] p-4" ref={terminalRef} />

        {/* Status Bar */}
        <div className="bg-[#0f0f70]/80 border-t border-blue-500/50 px-6 py-2 text-xs text-blue-200 font-mono flex justify-between">
          <span>👨🏻‍💻 minishell-web</span>
          <span>{isConnected ? 'Ready for commands' : isReconnecting ? `Reconnecting (${reconnectAttempts})` : 'Disconnected'}</span>
        </div>

        {/* Error Overlay */}
        {connectionError && (
          <div className="absolute inset-0 bg-[#111184]/95 backdrop-blur-sm flex items-center justify-center rounded-lg z-10">
            <div className="text-center p-8 max-w-md">
              <div className="w-16 h-16 bg-red-500/20 rounded-full flex items-center justify-center mx-auto mb-4 animate-pulse">
                <span className="text-2xl">⚠️</span>
              </div>
              <h3 className="text-xl font-bold text-white mb-2">Connection Lost</h3>
              <p className="text-blue-200 mb-2">{connectionError}</p>
              <button 
                onClick={handleRetryConnection}
                className="px-6 py-3 bg-gradient-to-r from-cyan-500 to-blue-500 hover:from-cyan-600 hover:to-blue-600 rounded-lg text-white font-semibold shadow-lg transition-all duration-200 transform hover:scale-105 active:scale-95"
              >
                🔄 Reconnect
              </button>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}
