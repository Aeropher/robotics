#!/usr/bin/env python3
"""
Animation Tester Web Server

A simple web server that bridges a web interface to the robot's serial port.
Click buttons in your browser to trigger animations on Damson.

Usage:
    python server.py [COM_PORT]

Example:
    python server.py COM3
    python server.py /dev/ttyUSB0

Then open http://localhost:8080 in your browser.
"""

import http.server
import json
import os
import sys
import threading
import time
from urllib.parse import parse_qs, urlparse

# Try to import serial, give helpful message if not installed
try:
    import serial
    import serial.tools.list_ports
except ImportError:
    print("Error: pyserial not installed")
    print("Install it with: pip install pyserial")
    sys.exit(1)

# Global serial connection
ser = None
serial_lock = threading.Lock()

def list_serial_ports():
    """List available serial ports."""
    ports = serial.tools.list_ports.comports()
    return [(p.device, p.description) for p in ports]

def connect_serial(port, baud=115200):
    """Connect to serial port."""
    global ser
    try:
        ser = serial.Serial(port, baud, timeout=1)
        time.sleep(2)  # Wait for Arduino to reset
        print(f"Connected to {port}")
        return True
    except Exception as e:
        print(f"Error connecting to {port}: {e}")
        return False

def send_command(cmd):
    """Send a command to the robot."""
    global ser
    if ser is None or not ser.is_open:
        return {"success": False, "error": "Not connected"}

    with serial_lock:
        try:
            ser.write(f"{cmd}\n".encode())
            time.sleep(0.1)
            response = ""
            while ser.in_waiting:
                response += ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
            return {"success": True, "response": response}
        except Exception as e:
            return {"success": False, "error": str(e)}

# HTML page with animation buttons
HTML_PAGE = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Damson Animation Tester</title>
    <style>
        * {
            box-sizing: border-box;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
        }
        body {
            max-width: 900px;
            margin: 0 auto;
            padding: 20px;
            background: #1a1a2e;
            color: #eee;
        }
        h1 {
            text-align: center;
            color: #00d4ff;
            margin-bottom: 10px;
        }
        .status {
            text-align: center;
            padding: 10px;
            margin-bottom: 20px;
            border-radius: 8px;
            background: #16213e;
        }
        .status.connected { border-left: 4px solid #00ff88; }
        .status.disconnected { border-left: 4px solid #ff4444; }
        .section {
            background: #16213e;
            border-radius: 12px;
            padding: 20px;
            margin-bottom: 20px;
        }
        .section h2 {
            margin-top: 0;
            color: #00d4ff;
            font-size: 1.1em;
            text-transform: uppercase;
            letter-spacing: 1px;
            border-bottom: 1px solid #333;
            padding-bottom: 10px;
        }
        .button-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(140px, 1fr));
            gap: 10px;
        }
        button {
            padding: 15px 10px;
            font-size: 14px;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            transition: all 0.2s;
            font-weight: 500;
        }
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 12px rgba(0,0,0,0.3);
        }
        button:active {
            transform: translateY(0);
        }
        button:disabled {
            opacity: 0.5;
            cursor: not-allowed;
            transform: none;
        }
        .subtle button {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        .expressive button {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
            color: white;
        }
        .playful button {
            background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
            color: #1a1a2e;
        }
        .dramatic button {
            background: linear-gradient(135deg, #fa709a 0%, #fee140 100%);
            color: #1a1a2e;
        }
        .control button {
            background: linear-gradient(135deg, #a8edea 0%, #fed6e3 100%);
            color: #1a1a2e;
        }
        #log {
            background: #0f0f1a;
            border-radius: 8px;
            padding: 15px;
            height: 150px;
            overflow-y: auto;
            font-family: 'Monaco', 'Consolas', monospace;
            font-size: 12px;
            white-space: pre-wrap;
        }
        #notes {
            width: 100%;
            height: 100px;
            background: #0f0f1a;
            border: 1px solid #333;
            border-radius: 8px;
            padding: 15px;
            color: #eee;
            font-family: inherit;
            font-size: 14px;
            resize: vertical;
        }
        #notes:focus {
            outline: none;
            border-color: #00d4ff;
        }
        .notes-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 10px;
        }
        .notes-header button {
            padding: 8px 16px;
            font-size: 12px;
        }
    </style>
</head>
<body>
    <h1>🤖 Damson Animation Tester</h1>

    <div id="status" class="status disconnected">
        Checking connection...
    </div>

    <div style="display: flex; gap: 10px; margin-bottom: 20px;">
        <button onclick="send('idle off')" style="flex: 1; padding: 20px; font-size: 18px; background: linear-gradient(135deg, #ff416c 0%, #ff4b2b 100%); color: white; font-weight: bold;">
            ⏹ STOP IDLE MODE
        </button>
        <button onclick="send('reset')" style="flex: 1; padding: 20px; font-size: 18px; background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: white; font-weight: bold;">
            ↺ RESET POSITION
        </button>
    </div>

    <div class="section subtle">
        <h2>😌 Subtle / Ambient</h2>
        <div class="button-grid">
            <button onclick="send('breathing')">Breathing</button>
            <button onclick="send('weightshift')">Weight Shift</button>
            <button onclick="send('lookaround')">Look Around</button>
        </div>
    </div>

    <div class="section expressive">
        <h2>😺 Expressive</h2>
        <div class="button-grid">
            <button onclick="send('stretch')">Stretch</button>
            <button onclick="send('shakeoff')">Shake Off</button>
            <button onclick="send('yawn')">Yawn</button>
            <button onclick="send('tapfoot')">Tap Foot</button>
        </div>
    </div>

    <div class="section playful">
        <h2>🎉 Playful</h2>
        <div class="button-grid">
            <button onclick="send('wave')">Wave</button>
            <button onclick="send('dance')">Dance Wiggle</button>
            <button onclick="send('curious')">Curious Peek</button>
            <button onclick="send('bounce')">Happy Bounce</button>
        </div>
    </div>

    <div class="section dramatic">
        <h2>🎭 Dramatic</h2>
        <div class="button-grid">
            <button onclick="send('startle')">Startle</button>
            <button onclick="send('pounce')">Pounce Ready</button>
            <button onclick="send('victory')">Victory Pose</button>
            <button onclick="send('drum')">Drum Fingers</button>
            <button onclick="send('tall')">Stand Tall</button>
            <button onclick="send('liedown')">Lie Down</button>
            <button onclick="send('twitch')">All Leg Twitch</button>
        </div>
    </div>

    <div class="section control">
        <h2>⚙️ Control</h2>
        <div class="button-grid">
            <button onclick="send('reset')">Reset Position</button>
            <button onclick="send('idle on')">Idle ON</button>
            <button onclick="send('idle off')">Idle OFF</button>
            <button onclick="send('help')">Help</button>
        </div>
    </div>

    <div class="section">
        <h2>📝 Notes</h2>
        <div class="notes-header">
            <span>Take notes while testing animations:</span>
            <button onclick="copyNotes()" style="background: #333; color: #fff;">Copy Notes</button>
        </div>
        <textarea id="notes" placeholder="Type your observations here...

Example:
- breathing: feels natural, maybe slow down exhale slightly
- wave: leg height good, wave count could be 4 instead of 3
- startle: crouch is too subtle, try 30mm instead of 20mm"></textarea>
    </div>

    <div class="section">
        <h2>📋 Log</h2>
        <div id="log"></div>
    </div>

    <script>
        const log = document.getElementById('log');
        const status = document.getElementById('status');
        let isConnected = false;

        function addLog(msg) {
            const time = new Date().toLocaleTimeString();
            log.textContent += `[${time}] ${msg}\\n`;
            log.scrollTop = log.scrollHeight;
        }

        async function checkStatus() {
            try {
                const res = await fetch('/status');
                const data = await res.json();
                isConnected = data.connected;
                status.className = 'status ' + (isConnected ? 'connected' : 'disconnected');
                status.textContent = isConnected
                    ? `✓ Connected to ${data.port}`
                    : '✗ Not connected - check serial port';
            } catch (e) {
                status.className = 'status disconnected';
                status.textContent = '✗ Server not responding';
            }
        }

        async function send(cmd) {
            if (!isConnected) {
                addLog(`Cannot send "${cmd}" - not connected`);
                return;
            }

            // Disable all buttons while command runs
            const buttons = document.querySelectorAll('button');
            buttons.forEach(b => b.disabled = true);

            addLog(`Sending: ${cmd}`);

            try {
                const res = await fetch('/send?cmd=' + encodeURIComponent(cmd));
                const data = await res.json();
                if (data.success) {
                    if (data.response) {
                        addLog(data.response.trim());
                    }
                } else {
                    addLog(`Error: ${data.error}`);
                }
            } catch (e) {
                addLog(`Error: ${e.message}`);
            }

            // Re-enable buttons
            buttons.forEach(b => b.disabled = false);
        }

        function copyNotes() {
            const notes = document.getElementById('notes');
            notes.select();
            document.execCommand('copy');
            addLog('Notes copied to clipboard');
        }

        // Check status on load and periodically
        checkStatus();
        setInterval(checkStatus, 5000);

        addLog('Animation Tester ready');
        addLog('Click any button to trigger an animation');
    </script>
</body>
</html>
"""

class AnimationHandler(http.server.BaseHTTPRequestHandler):
    """HTTP request handler for animation tester."""

    def log_message(self, format, *args):
        """Suppress default logging."""
        pass

    def do_GET(self):
        parsed = urlparse(self.path)

        if parsed.path == '/' or parsed.path == '/index.html':
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.end_headers()
            self.wfile.write(HTML_PAGE.encode())

        elif parsed.path == '/status':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            connected = ser is not None and ser.is_open
            port = ser.port if connected else None
            self.wfile.write(json.dumps({
                "connected": connected,
                "port": port
            }).encode())

        elif parsed.path == '/send':
            params = parse_qs(parsed.query)
            cmd = params.get('cmd', [''])[0]

            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()

            if cmd:
                result = send_command(cmd)
                self.wfile.write(json.dumps(result).encode())
            else:
                self.wfile.write(json.dumps({
                    "success": False,
                    "error": "No command specified"
                }).encode())
        else:
            self.send_response(404)
            self.end_headers()

def main():
    port_arg = sys.argv[1] if len(sys.argv) > 1 else None

    print("=" * 50)
    print("  Damson Animation Tester")
    print("=" * 50)
    print()

    # List available ports
    ports = list_serial_ports()
    if ports:
        print("Available serial ports:")
        for i, (device, desc) in enumerate(ports):
            print(f"  [{i}] {device} - {desc}")
        print()
    else:
        print("No serial ports found!")
        print()

    # Determine which port to use
    if port_arg:
        port = port_arg
    elif ports:
        # Try to auto-detect Arduino
        arduino_port = None
        for device, desc in ports:
            if 'Arduino' in desc or 'CH340' in desc or 'USB' in desc:
                arduino_port = device
                break

        if arduino_port:
            port = arduino_port
            print(f"Auto-detected: {port}")
        else:
            port = ports[0][0]
            print(f"Using first port: {port}")
    else:
        print("No serial port available. Please connect the robot.")
        print("Usage: python server.py [COM_PORT]")
        sys.exit(1)

    # Connect to serial port
    print(f"Connecting to {port}...")
    if not connect_serial(port):
        print("Failed to connect. Check the port and try again.")
        sys.exit(1)

    # Start web server
    server_port = 8888
    server = http.server.HTTPServer(('', server_port), AnimationHandler)

    print()
    print("=" * 50)
    print(f"  Open http://localhost:{server_port} in your browser")
    print("=" * 50)
    print()
    print("Press Ctrl+C to stop")
    print()

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down...")
        if ser:
            ser.close()

if __name__ == '__main__':
    main()
