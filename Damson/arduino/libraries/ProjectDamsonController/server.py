#!/usr/bin/env python3
"""
Damson Web Controller

A web server that bridges a web interface to the robot's serial port.
Includes animation testing and motor testing functionality.

Usage:
    python server.py [COM_PORT]

Example:
    python server.py COM3
    python server.py /dev/ttyUSB0

Then open http://localhost:8888 in your browser.
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

def disconnect_serial():
    """Disconnect from serial port."""
    global ser
    if ser is not None and ser.is_open:
        ser.close()
        print("Disconnected from serial port")
        return True
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

# HTML page with tabbed interface
HTML_PAGE = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Damson Web Controller</title>
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
            border-radius: 8px;
            background: #16213e;
        }
        .status.connected { border-left: 4px solid #00ff88; }
        .status.disconnected { border-left: 4px solid #ff4444; }

        /* Global top bar */
        .top-bar {
            background: #0f0f1a;
            border-radius: 12px;
            padding: 15px;
            margin-bottom: 20px;
        }
        .connection-row {
            display: flex;
            gap: 10px;
            margin-bottom: 15px;
            align-items: center;
        }
        .global-buttons {
            display: flex;
            gap: 10px;
        }

        /* Tab bar */
        .tab-bar {
            display: flex;
            gap: 5px;
            margin-bottom: 20px;
            background: #0f0f1a;
            padding: 5px;
            border-radius: 12px;
        }
        .tab-btn {
            flex: 1;
            padding: 15px 20px;
            font-size: 16px;
            font-weight: bold;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            background: transparent;
            color: #888;
            transition: all 0.2s;
        }
        .tab-btn:hover {
            background: #16213e;
            color: #ccc;
        }
        .tab-btn.active {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        .tab-content {
            display: none;
        }
        .tab-content.active {
            display: block;
        }

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

        /* Motor testing styles */
        .leg-btn {
            width: 40px;
            height: 40px;
            border-radius: 50%;
            background: #333;
            color: #888;
            font-weight: bold;
            font-size: 16px;
            border: 2px solid #444;
            cursor: pointer;
            transition: all 0.2s;
            padding: 0;
        }
        .leg-btn:hover {
            background: #444;
            color: #fff;
            transform: scale(1.1);
        }
        .leg-btn.leg-selected {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border-color: #00d4ff;
            box-shadow: 0 0 10px rgba(0, 212, 255, 0.5);
        }

        .joint-btn {
            flex: 1;
            padding: 12px 15px;
            background: #333;
            color: #888;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            font-weight: 500;
            transition: all 0.2s;
        }
        .joint-btn:hover {
            background: #444;
            color: #ccc;
        }
        .joint-btn.joint-selected {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }

        .preset-btn {
            flex: 1;
            padding: 12px;
            background: #333;
            color: #aaa;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            font-weight: 500;
            font-size: 14px;
            transition: all 0.2s;
        }
        .preset-btn:hover {
            background: #444;
            color: #fff;
        }
        .preset-btn.preset-center {
            background: #16213e;
            border: 1px solid #00d4ff;
            color: #00d4ff;
        }

        .pos-row {
            font-size: 12px;
            padding: 4px 0;
            font-family: 'Monaco', 'Consolas', monospace;
        }
        .pos-label {
            color: #888;
        }
        .pos-values {
            color: #00d4ff;
        }
    </style>
</head>
<body>
    <h1>Damson Web Controller</h1>

    <!-- Global Top Bar -->
    <div class="top-bar">
        <div class="connection-row">
            <div id="status" class="status disconnected" style="flex: 0 0 auto; padding: 8px 15px;">
                Checking...
            </div>
            <select id="portSelect" style="flex: 1; padding: 10px; font-size: 14px; background: #16213e; color: #eee; border: 1px solid #333; border-radius: 8px;">
                <option value="">Loading ports...</option>
            </select>
            <button id="connectBtn" onclick="toggleConnection()" style="padding: 10px 25px; font-size: 14px; background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: white; font-weight: bold;">
                Connect
            </button>
        </div>
        <div class="global-buttons">
            <button onclick="send('idle off')" style="flex: 1; padding: 15px; font-size: 16px; background: linear-gradient(135deg, #ff416c 0%, #ff4b2b 100%); color: white; font-weight: bold;">
                STOP IDLE
            </button>
            <button onclick="send('reset')" style="flex: 1; padding: 15px; font-size: 16px; background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: white; font-weight: bold;">
                RESET POSITION
            </button>
        </div>
    </div>

    <!-- Tab Bar -->
    <div class="tab-bar">
        <button class="tab-btn active" onclick="switchTab('animations')">Animations</button>
        <button class="tab-btn" onclick="switchTab('motor')">Motor Testing</button>
    </div>

    <!-- Animations Tab -->
    <div id="tab-animations" class="tab-content active">
        <div class="section subtle">
            <h2>Subtle / Ambient</h2>
            <div class="button-grid">
                <button onclick="send('breathing')">Breathing</button>
                <button onclick="send('weightshift')">Weight Shift</button>
                <button onclick="send('lookaround')">Look Around</button>
            </div>
        </div>

        <div class="section expressive">
            <h2>Expressive</h2>
            <div class="button-grid">
                <button onclick="send('stretch')">Stretch</button>
                <button onclick="send('shakeoff')">Shake Off</button>
                <button onclick="send('yawn')">Yawn</button>
                <button onclick="send('tapfoot')">Tap Foot</button>
            </div>
        </div>

        <div class="section playful">
            <h2>Playful</h2>
            <div class="button-grid">
                <button onclick="send('wave')">Wave</button>
                <button onclick="send('dance')">Dance Wiggle</button>
                <button onclick="send('curious')">Curious Peek</button>
                <button onclick="send('bounce')">Happy Bounce</button>
            </div>
        </div>

        <div class="section dramatic">
            <h2>Dramatic</h2>
            <div class="button-grid">
                <button onclick="send('startle')">Startle</button>
                <button onclick="send('pounce')">Pounce Ready</button>
                <button onclick="send('victory')">Victory Pose</button>
                <button onclick="send('drum')">Drum Fingers</button>
                <button onclick="send('tall')">Stand Tall</button>
                <button onclick="send('liedown')">Lie Down</button>
                <button onclick="send('twitch')">All Leg Twitch</button>
                <button onclick="send('defensive')">Defensive Crouch</button>
            </div>
        </div>

        <div class="section" style="background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);">
            <h2>Walk - Default Gait (Tripod)</h2>
            <div class="button-grid">
                <button onclick="send('deffb')" style="background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%); color: #1a1a2e;">Forward / Back</button>
                <button onclick="send('defbf')" style="background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%); color: #1a1a2e;">Back / Forward</button>
                <button onclick="send('deflr')" style="background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%); color: #1a1a2e;">Left / Right</button>
                <button onclick="send('defrl')" style="background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%); color: #1a1a2e;">Right / Left</button>
                <button onclick="send('deftlr')" style="background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%); color: #1a1a2e;">Turn L / R</button>
                <button onclick="send('deftrl')" style="background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%); color: #1a1a2e;">Turn R / L</button>
            </div>
        </div>

        <div class="section" style="background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);">
            <h2>Walk - Wave Gait (One Leg)</h2>
            <div class="button-grid">
                <button onclick="send('wavefb')" style="background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: #1a1a2e;">Forward / Back</button>
                <button onclick="send('wavebf')" style="background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: #1a1a2e;">Back / Forward</button>
                <button onclick="send('wavelr')" style="background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: #1a1a2e;">Left / Right</button>
                <button onclick="send('waverl')" style="background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: #1a1a2e;">Right / Left</button>
                <button onclick="send('wavetlr')" style="background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: #1a1a2e;">Turn L / R</button>
                <button onclick="send('wavetrl')" style="background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: #1a1a2e;">Turn R / L</button>
            </div>
        </div>

        <div class="section" style="background: linear-gradient(135deg, #232526 0%, #414345 100%); border: 2px solid #ff6b6b;">
            <h2>Actions</h2>
            <div class="button-grid">
                <button onclick="send('jump50')" style="background: linear-gradient(135deg, #f5af19 0%, #f12711 100%); color: white; font-weight: bold;">Jump 50%</button>
                <button onclick="send('jump75')" style="background: linear-gradient(135deg, #ff416c 0%, #ff4b2b 100%); color: white; font-weight: bold;">Jump 75%</button>
                <button onclick="send('jump100')" style="background: linear-gradient(135deg, #8E2DE2 0%, #4A00E0 100%); color: white; font-weight: bold; font-size: 16px;">JUMP 100%!</button>
            </div>
        </div>

        <div class="section control">
            <h2>Control</h2>
            <div class="button-grid">
                <button onclick="send('reset')">Reset Position</button>
                <button onclick="send('idle on')">Idle ON</button>
                <button onclick="send('idle off')">Idle OFF</button>
                <button onclick="send('help')">Help</button>
            </div>
        </div>

        <div class="section">
            <h2>Notes</h2>
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
    </div>

    <!-- Motor Testing Tab -->
    <div id="tab-motor" class="tab-content">
        <div class="section" style="background: linear-gradient(135deg, #1a1a2e 0%, #2d132c 100%); border: 2px solid #ff9800;">
            <h2>Motor Testing <span style="font-size: 0.7em; color: #ff9800;">(Bypasses Limits!)</span></h2>
            <p style="color: #ff9800; font-size: 12px; margin: 0 0 15px 0;">Move slowly and watch for binding. Values are servo angles (0-180).</p>

            <div style="display: flex; gap: 30px; flex-wrap: wrap;">
                <!-- Left side: Leg selector diagram -->
                <div style="flex: 0 0 auto;">
                    <label style="display: block; margin-bottom: 10px; font-size: 12px; color: #aaa; text-align: center;">SELECT LEG</label>
                    <div style="position: relative; width: 200px; height: 240px; background: #0f0f1a; border-radius: 12px; padding: 10px;">
                        <!-- FRONT label -->
                        <div style="text-align: center; color: #666; font-size: 11px; margin-bottom: 5px;">FRONT</div>

                        <!-- Hexapod body -->
                        <div style="position: relative; width: 100%; height: 180px;">
                            <!-- Body outline -->
                            <div style="position: absolute; left: 50%; top: 50%; transform: translate(-50%, -50%); width: 60px; height: 140px; background: #16213e; border-radius: 30px; border: 2px solid #333;"></div>

                            <!-- Leg buttons - Right side (1, 2, 3) -->
                            <button onclick="selectLeg(1)" id="legBtn1" class="leg-btn leg-selected" style="position: absolute; left: 15px; top: 15px;">1</button>
                            <button onclick="selectLeg(2)" id="legBtn2" class="leg-btn" style="position: absolute; left: 5px; top: 75px;">2</button>
                            <button onclick="selectLeg(3)" id="legBtn3" class="leg-btn" style="position: absolute; left: 15px; top: 135px;">3</button>

                            <!-- Leg buttons - Left side (4, 5, 6) -->
                            <button onclick="selectLeg(4)" id="legBtn4" class="leg-btn" style="position: absolute; right: 15px; top: 15px;">4</button>
                            <button onclick="selectLeg(5)" id="legBtn5" class="leg-btn" style="position: absolute; right: 5px; top: 75px;">5</button>
                            <button onclick="selectLeg(6)" id="legBtn6" class="leg-btn" style="position: absolute; right: 15px; top: 135px;">6</button>
                        </div>

                        <!-- REAR label -->
                        <div style="text-align: center; color: #666; font-size: 11px;">REAR</div>
                    </div>
                </div>

                <!-- Right side: Joint selector and angle control -->
                <div style="flex: 1; min-width: 280px;">
                    <!-- Joint selector tabs -->
                    <label style="display: block; margin-bottom: 10px; font-size: 12px; color: #aaa;">SELECT JOINT</label>
                    <div style="display: flex; gap: 5px; margin-bottom: 20px;">
                        <button onclick="selectJoint(0)" id="jointBtn0" class="joint-btn joint-selected">Hip (A)</button>
                        <button onclick="selectJoint(1)" id="jointBtn1" class="joint-btn">Femur (B)</button>
                        <button onclick="selectJoint(2)" id="jointBtn2" class="joint-btn">Tibia (C)</button>
                    </div>

                    <!-- Current selection display -->
                    <div id="selectionDisplay" style="background: #0f0f1a; border-radius: 8px; padding: 10px 15px; margin-bottom: 20px; text-align: center;">
                        <span style="color: #888; font-size: 12px;">CONTROLLING</span><br>
                        <span style="color: #00d4ff; font-size: 18px; font-weight: bold;">Leg 1 - Hip (A)</span>
                    </div>

                    <!-- Angle control -->
                    <div style="display: flex; align-items: center; gap: 15px; margin-bottom: 15px;">
                        <div style="display: flex; align-items: center; gap: 8px; flex: 1;">
                            <input type="number" id="motorAngle" value="90" min="0" max="180" style="width: 100px; padding: 15px; font-size: 32px; text-align: center; background: #0f0f1a; color: #00d4ff; border: 2px solid #333; border-radius: 8px;">
                            <span style="color: #888; font-size: 14px;">deg</span>
                            <div style="display: flex; flex-direction: column; gap: 4px;">
                                <button onclick="adjustAngle(1)" style="width: 50px; height: 35px; font-size: 18px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 0;">+</button>
                                <button onclick="adjustAngle(-1)" style="width: 50px; height: 35px; font-size: 18px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 0;">-</button>
                            </div>
                        </div>
                        <div>
                            <label style="display: block; margin-bottom: 5px; font-size: 11px; color: #666;">STEP</label>
                            <select id="motorIncrement" style="padding: 8px; font-size: 14px; background: #16213e; color: #eee; border: 1px solid #333; border-radius: 6px;">
                                <option value="1">1</option>
                                <option value="5" selected>5</option>
                                <option value="10">10</option>
                            </select>
                        </div>
                    </div>

                    <!-- Preset angle buttons -->
                    <div style="display: flex; gap: 8px;">
                        <button onclick="setAngle(0)" class="preset-btn">0</button>
                        <button onclick="setAngle(45)" class="preset-btn">45</button>
                        <button onclick="setAngle(90)" class="preset-btn preset-center">90</button>
                        <button onclick="setAngle(135)" class="preset-btn">135</button>
                        <button onclick="setAngle(180)" class="preset-btn">180</button>
                    </div>
                </div>
            </div>
        </div>

        <!-- Live position display -->
        <div class="section">
            <h2>Current Positions <button onclick="syncFromReset()" style="float: right; padding: 8px 15px; font-size: 12px; background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); color: white;">Sync from Reset</button></h2>
            <div style="display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px;">
                <div style="background: #0f0f1a; border-radius: 8px; padding: 15px;">
                    <div style="color: #888; font-size: 11px; margin-bottom: 5px;">RIGHT SIDE</div>
                    <div id="pos-leg1" class="pos-row"><span class="pos-label">Leg 1 (FR):</span> <span class="pos-values">--</span></div>
                    <div id="pos-leg2" class="pos-row"><span class="pos-label">Leg 2 (MR):</span> <span class="pos-values">--</span></div>
                    <div id="pos-leg3" class="pos-row"><span class="pos-label">Leg 3 (RR):</span> <span class="pos-values">--</span></div>
                </div>
                <div style="background: #0f0f1a; border-radius: 8px; padding: 15px;">
                    <div style="color: #888; font-size: 11px; margin-bottom: 5px;">LEFT SIDE</div>
                    <div id="pos-leg4" class="pos-row"><span class="pos-label">Leg 4 (FL):</span> <span class="pos-values">--</span></div>
                    <div id="pos-leg5" class="pos-row"><span class="pos-label">Leg 5 (ML):</span> <span class="pos-values">--</span></div>
                    <div id="pos-leg6" class="pos-row"><span class="pos-label">Leg 6 (RL):</span> <span class="pos-values">--</span></div>
                </div>
                <div style="background: #0f0f1a; border-radius: 8px; padding: 15px;">
                    <div style="color: #888; font-size: 11px; margin-bottom: 5px;">JOINT KEY</div>
                    <div style="color: #667eea; font-size: 12px;">H = Hip (A)</div>
                    <div style="color: #f093fb; font-size: 12px;">F = Femur (B)</div>
                    <div style="color: #4facfe; font-size: 12px;">T = Tibia (C)</div>
                </div>
            </div>
        </div>

        <div class="section">
            <h2>Motor Test Notes</h2>
            <div class="notes-header">
                <span>Record servo limits as you test:</span>
                <button onclick="copyMotorNotes()" style="background: #333; color: #fff;">Copy Notes</button>
            </div>
            <textarea id="motorNotes" style="width: 100%; height: 150px; background: #0f0f1a; border: 1px solid #333; border-radius: 8px; padding: 15px; color: #eee; font-family: inherit; font-size: 14px; resize: vertical;" placeholder="Record your findings here...

Example:
Leg 1 Hip (A): min=25, max=155
Leg 1 Femur (B): min=30, max=150
Leg 1 Tibia (C): min=20, max=160 (constrained when femur > 120)"></textarea>
        </div>
    </div>

    <!-- Shared Log Section -->
    <div class="section">
        <h2>Log</h2>
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

        function switchTab(tabName) {
            // Update tab buttons
            document.querySelectorAll('.tab-btn').forEach(btn => {
                btn.classList.remove('active');
            });
            event.target.classList.add('active');

            // Update tab content
            document.querySelectorAll('.tab-content').forEach(content => {
                content.classList.remove('active');
            });
            document.getElementById('tab-' + tabName).classList.add('active');
        }

        async function checkStatus() {
            try {
                const res = await fetch('/status');
                const data = await res.json();
                isConnected = data.connected;
                status.className = 'status ' + (isConnected ? 'connected' : 'disconnected');
                status.textContent = isConnected
                    ? `Connected: ${data.port}`
                    : 'Not connected';
            } catch (e) {
                status.className = 'status disconnected';
                status.textContent = 'Server error';
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

        function copyMotorNotes() {
            const notes = document.getElementById('motorNotes');
            notes.select();
            document.execCommand('copy');
            addLog('Motor notes copied to clipboard');
        }

        // Motor testing state
        let selectedLeg = 1;
        let selectedJoint = 0;
        const jointNames = ['Hip (A)', 'Femur (B)', 'Tibia (C)'];

        // Default/reset position servo angles (from robot calibration)
        // These are approximate - actual values depend on calibration offsets
        const defaultAngles = {
            1: { hip: 90, femur: 90, tibia: 90 },
            2: { hip: 90, femur: 90, tibia: 90 },
            3: { hip: 90, femur: 90, tibia: 90 },
            4: { hip: 90, femur: 90, tibia: 90 },
            5: { hip: 90, femur: 90, tibia: 90 },
            6: { hip: 90, femur: 90, tibia: 90 }
        };

        const legPositions = {
            1: { hip: '--', femur: '--', tibia: '--' },
            2: { hip: '--', femur: '--', tibia: '--' },
            3: { hip: '--', femur: '--', tibia: '--' },
            4: { hip: '--', femur: '--', tibia: '--' },
            5: { hip: '--', femur: '--', tibia: '--' },
            6: { hip: '--', femur: '--', tibia: '--' }
        };

        function syncFromReset() {
            // Reset robot to known position and sync UI
            send('reset');
            // Set all positions to default after reset
            for (let leg = 1; leg <= 6; leg++) {
                legPositions[leg] = { ...defaultAngles[leg] };
                updatePositionDisplayForLeg(leg);
            }
            // Set angle input to 90 (center)
            document.getElementById('motorAngle').value = 90;
            addLog('Synced positions from reset state');
        }

        function updatePositionDisplayForLeg(leg) {
            const posEl = document.getElementById('pos-leg' + leg);
            if (posEl) {
                const p = legPositions[leg];
                posEl.querySelector('.pos-values').innerHTML =
                    `<span style="color:#667eea">H:${p.hip}</span> ` +
                    `<span style="color:#f093fb">F:${p.femur}</span> ` +
                    `<span style="color:#4facfe">T:${p.tibia}</span>`;
            }
        }

        function selectLeg(leg) {
            selectedLeg = leg;
            // Update button styles
            for (let i = 1; i <= 6; i++) {
                const btn = document.getElementById('legBtn' + i);
                btn.classList.toggle('leg-selected', i === leg);
            }
            updateSelectionDisplay();
            updateAngleFromCurrentPosition();
        }

        function selectJoint(joint) {
            selectedJoint = joint;
            // Update button styles
            for (let i = 0; i <= 2; i++) {
                const btn = document.getElementById('jointBtn' + i);
                btn.classList.toggle('joint-selected', i === joint);
            }
            updateSelectionDisplay();
            updateAngleFromCurrentPosition();
        }

        function updateAngleFromCurrentPosition() {
            // Update angle input to show current known position for selected leg/joint
            const jointKey = ['hip', 'femur', 'tibia'][selectedJoint];
            const currentAngle = legPositions[selectedLeg][jointKey];
            if (currentAngle !== '--') {
                document.getElementById('motorAngle').value = currentAngle;
            }
        }

        function updateSelectionDisplay() {
            const display = document.getElementById('selectionDisplay');
            display.innerHTML = `<span style="color: #888; font-size: 12px;">CONTROLLING</span><br>
                <span style="color: #00d4ff; font-size: 18px; font-weight: bold;">Leg ${selectedLeg} - ${jointNames[selectedJoint]}</span>`;
        }

        function updatePositionDisplay(leg, joint, angle) {
            const jointKey = ['hip', 'femur', 'tibia'][joint];
            legPositions[leg][jointKey] = angle;
            updatePositionDisplayForLeg(leg);
        }

        // Motor testing functions
        function adjustAngle(direction) {
            const angleInput = document.getElementById('motorAngle');
            const increment = parseInt(document.getElementById('motorIncrement').value);
            let angle = parseInt(angleInput.value) + (direction * increment);
            angle = Math.max(0, Math.min(180, angle));
            angleInput.value = angle;
            sendServo();  // Send immediately
        }

        function setAngle(angle) {
            document.getElementById('motorAngle').value = angle;
            sendServo();  // Send immediately
        }

        function sendServo() {
            const angle = document.getElementById('motorAngle').value;
            const cmd = `servo ${selectedLeg} ${selectedJoint} ${angle}`;
            send(cmd);
            updatePositionDisplay(selectedLeg, selectedJoint, angle);
        }

        async function loadPorts() {
            try {
                const res = await fetch('/ports');
                const data = await res.json();
                const select = document.getElementById('portSelect');
                select.innerHTML = '';
                if (data.ports.length === 0) {
                    select.innerHTML = '<option value="">No ports found</option>';
                } else {
                    data.ports.forEach(p => {
                        const opt = document.createElement('option');
                        opt.value = p.device;
                        opt.textContent = `${p.device} - ${p.description}`;
                        select.appendChild(opt);
                    });
                }
            } catch (e) {
                console.error('Failed to load ports:', e);
            }
        }

        async function toggleConnection() {
            const btn = document.getElementById('connectBtn');
            btn.disabled = true;

            if (isConnected) {
                // Disconnect
                addLog('Disconnecting...');
                try {
                    const res = await fetch('/disconnect');
                    const data = await res.json();
                    if (data.success) {
                        addLog('Disconnected');
                    } else {
                        addLog('Disconnect failed: ' + data.error);
                    }
                } catch (e) {
                    addLog('Error: ' + e.message);
                }
            } else {
                // Connect
                const port = document.getElementById('portSelect').value;
                if (!port) {
                    addLog('Please select a port');
                    btn.disabled = false;
                    return;
                }
                addLog('Connecting to ' + port + '...');
                try {
                    const res = await fetch('/connect?port=' + encodeURIComponent(port));
                    const data = await res.json();
                    if (data.success) {
                        addLog('Connected to ' + port);
                    } else {
                        addLog('Connect failed: ' + data.error);
                    }
                } catch (e) {
                    addLog('Error: ' + e.message);
                }
            }

            btn.disabled = false;
            checkStatus();
        }

        function updateConnectButton() {
            const btn = document.getElementById('connectBtn');
            const select = document.getElementById('portSelect');
            if (isConnected) {
                btn.textContent = 'Disconnect';
                btn.style.background = 'linear-gradient(135deg, #ff416c 0%, #ff4b2b 100%)';
                select.disabled = true;
            } else {
                btn.textContent = 'Connect';
                btn.style.background = 'linear-gradient(135deg, #11998e 0%, #38ef7d 100%)';
                select.disabled = false;
            }
        }

        // Override checkStatus to also update button
        const originalCheckStatus = checkStatus;
        checkStatus = async function() {
            await originalCheckStatus();
            updateConnectButton();
        };

        // Check status on load and periodically
        loadPorts();
        checkStatus();
        setInterval(checkStatus, 5000);

        addLog('Damson Web Controller ready');
        addLog('Select a port and click Connect');
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

        elif parsed.path == '/ports':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            ports = list_serial_ports()
            self.wfile.write(json.dumps({
                "ports": [{"device": d, "description": desc} for d, desc in ports]
            }).encode())

        elif parsed.path == '/connect':
            params = parse_qs(parsed.query)
            port = params.get('port', [''])[0]

            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()

            if port:
                # Disconnect first if already connected
                disconnect_serial()
                success = connect_serial(port)
                self.wfile.write(json.dumps({
                    "success": success,
                    "error": None if success else "Failed to connect"
                }).encode())
            else:
                self.wfile.write(json.dumps({
                    "success": False,
                    "error": "No port specified"
                }).encode())

        elif parsed.path == '/disconnect':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            success = disconnect_serial()
            self.wfile.write(json.dumps({
                "success": True,
                "error": None
            }).encode())

        else:
            self.send_response(404)
            self.end_headers()

def main():
    port_arg = sys.argv[1] if len(sys.argv) > 1 else None

    print("=" * 50)
    print("  Damson Web Controller")
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
