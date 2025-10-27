// components/Terminal.tsx
'use client';

import { useState, useRef, useEffect, useCallback } from 'react';
import { Terminal as XTerm } from 'xterm';
import { FitAddon } from '@xterm/addon-fit';
import 'xterm/css/xterm.css';

const MAX_RECONNECT_ATTEMPTS = 5;
const RECONNECT_DELAY = 2000;
const TERMINAL_INIT_DELAY = 100;
const RESIZE_DEBOUNCE_DELAY = 300;

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
  const resizeTimeoutRef = useRef<NodeJS.Timeout | null>(null);

  // Extract terminal configuration
  const TERMINAL_CONFIG = {
    cursorBlink: true,
    theme: {
      background: '#0f172a',
      foreground: '#e2e8f0',
      cursor: '#e2e8f0',
      cursorAccent: '#0f172a',
      selectionBackground: '#1e293b',
      black: '#000000',
      red: '#ef4444',
      green: '#10b981',
      yellow: '#f59e0b',
      blue: '#3b82f6',
      magenta: '#ec4899',
      cyan: '#06b6d4',
      white: '#e2e8f0',
      brightBlack: '#475569',
      brightRed: '#f87171',
      brightGreen: '#34d399',
      brightYellow: '#fbbf24',
      brightBlue: '#60a5fa',
      brightMagenta: '#f472b6',
      brightCyan: '#22d3ee',
      brightWhite: '#f1f5f9'
    },
    fontSize: 15,
    fontFamily: '"Fira Code", "Cascadia Code", monospace',
    fontWeight: '400' as const,
    lineHeight: 1.2,
    scrollback: 10000,
    allowTransparency: false,
  };

  // Initialize terminal
  useEffect(() => {
    if (typeof window === 'undefined' || !terminalRef.current || xtermRef.current) return;

    const timeout = setTimeout(() => {
      try {
        const terminal = new XTerm(TERMINAL_CONFIG);
        const fitAddon = new FitAddon();
        terminal.loadAddon(fitAddon);
        
        if (terminalRef.current) {
          terminal.open(terminalRef.current);
          
          setTimeout(() => {
            try {
              fitAddon.fit();
              setIsTerminalReady(true);
            } catch (error) {
              console.error('Error fitting terminal:', error);
              setTimeout(() => {
                try {
                  fitAddon.fit();
                  setIsTerminalReady(true);
                } catch (retryError) {
                  console.error('Retry fitting failed:', retryError);
                  setIsTerminalReady(true);
                }
              }, 100);
            }
          }, 150);
        }

        xtermRef.current = terminal;
        fitAddonRef.current = fitAddon;

        // Handle terminal input
        terminal.onData((data) => {
          if (wsRef.current?.readyState === WebSocket.OPEN) {
            wsRef.current.send(JSON.stringify({ type: 'input', data }));
          }
        });

        // Handle special keys
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

      } catch (error) {
        console.error('Error initializing terminal:', error);
      }
    }, TERMINAL_INIT_DELAY);

    return () => {
      clearTimeout(timeout);
      if (xtermRef.current) {
        try {
          xtermRef.current.dispose();
        } catch (error) {
          console.error('Error disposing terminal:', error);
        }
        xtermRef.current = null;
      }
    };
  }, []);

  // Parse WebSocket message
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

  // Handle WebSocket connection
  const connectWebSocket = useCallback(() => {
    if (wsRef.current?.readyState === WebSocket.OPEN || !isTerminalReady) return;

    const websocket = new WebSocket('ws://localhost:8080');
    const terminal = xtermRef.current;
    
    websocket.onopen = () => {
      setIsConnected(true);
      setConnectionError('');
      setReconnectAttempts(0);
      setIsReconnecting(false);
    };

    websocket.onmessage = (event) => {
      if (!terminal) return;
      try {
        handleWebSocketMessage(JSON.parse(event.data), terminal);
      } catch (error) {
        console.error('Error parsing WebSocket message:', error);
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

  // Connect/disconnect WebSocket
  useEffect(() => {
    if (isTerminalReady) {
      connectWebSocket();
    }
    
    return () => {
      if (reconnectTimeoutRef.current) clearTimeout(reconnectTimeoutRef.current);
      if (wsRef.current) wsRef.current.close();
    };
  }, [connectWebSocket, isTerminalReady]);

  // Handle window resize
  useEffect(() => {
    if (!isTerminalReady) return;

    const handleResize = () => {
      if (resizeTimeoutRef.current) clearTimeout(resizeTimeoutRef.current);

      resizeTimeoutRef.current = setTimeout(() => {
        const { fitAddon: addon, terminal, ws } = {
          fitAddon: fitAddonRef.current,
          terminal: xtermRef.current,
          ws: wsRef.current
        };

        if (addon && terminal && terminalRef.current) {
          try {
            addon.fit();
            if (ws?.readyState === WebSocket.OPEN) {
              ws.send(JSON.stringify({ type: 'resize', cols: terminal.cols, rows: terminal.rows }));
            }
          } catch (error) {
            console.error('Error during resize:', error);
          }
        }
      }, 50);
    };

    window.addEventListener('resize', handleResize);
    
    // Initial fit
    setTimeout(handleResize, RESIZE_DEBOUNCE_DELAY);
    
    return () => {
      window.removeEventListener('resize', handleResize);
      if (resizeTimeoutRef.current) clearTimeout(resizeTimeoutRef.current);
    };
  }, [isTerminalReady]);

  const handleRetryConnection = () => {
    setConnectionError('');
    setReconnectAttempts(0);
    setIsReconnecting(false);
    if (reconnectTimeoutRef.current) clearTimeout(reconnectTimeoutRef.current);
    connectWebSocket();
  };

  return (
    <div className="w-full max-w-5xl mx-auto p-4">
      <div className="bg-slate-900 rounded-lg shadow-xl overflow-hidden border border-slate-700">
        
        {/* Header */}
        <div className="bg-gradient-to-r from-[#0f0f70] via-[#131380] to-[#0f0f70] px-6 py-4 border-b border-blue-500/50">
          <div className="flex items-center justify-between">
            <div className="flex items-center space-x-3">
              {/* Connection Indicator */}
              <div className="relative">
                <div className={`w-3 h-3 rounded-full shadow-lg transition-colors duration-300 ${
                  isConnected ? 'bg-green-400' : isReconnecting ? 'bg-yellow-400' : 'bg-red-400'
                }`}>
                  {isConnected && <div className="absolute inset-0 bg-green-400 rounded-full animate-ping opacity-75"></div>}
                  {isReconnecting && <div className="absolute inset-0 bg-yellow-400 rounded-full animate-pulse"></div>}
                </div>
              </div>
              
              {/* Terminal Info */}
              <div className="flex items-center space-x-2">
                <div className="w-2 h-2 bg-cyan-300 rounded-full"></div>
                <span className="text-white font-mono text-sm font-bold">minishell</span>
                <span className="text-blue-200 text-sm">•</span>
                <span className="text-green-300 text-sm font-mono">v1.0</span>
              </div>
            </div>

            {/* Window Controls */}
            <div className="flex space-x-2">
              <div className="w-3 h-3 bg-yellow-400 rounded-full hover:bg-yellow-300 cursor-pointer transition-colors"></div>
              <div className="w-3 h-3 bg-red-400 rounded-full hover:bg-red-300 cursor-pointer transition-colors"></div>
            </div>
          </div>
        </div>

        {/* Terminal Area */}
        <div className="w-full h-96 bg-[#0f172a] p-4 relative" ref={terminalRef} />

        {/* Status Bar */}
        <div className="bg-[#0f0f70]/80 border-t border-blue-500/50 px-6 py-3">
          <div className="flex items-center justify-between text-xs text-blue-200 font-mono">
            <span>👨🏻‍💻 minishell-web</span>
            <span>{isConnected ? 'Ready for commands' : isReconnecting ? `Reconnecting (${reconnectAttempts})` : 'Disconnected'}</span>
          </div>
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
              <p className="text-blue-300 text-sm mb-6">Maximum reconnection attempts reached</p>
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