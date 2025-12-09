# Damson Hexapod Robot - Product Requirements Document

## Project Overview

Damson is a hexapod robot based on the Freenove Hexapod Robot Kit. This PRD documents the robot's capabilities, development roadmap, and exploration projects.

### Vision

Create a smooth, organic-moving hexapod robot inspired by projects like [MakeYourPet](https://www.makeyourpet.com/). Long-term goals include training gaits in NVIDIA Isaac Sim and adding autonomous behaviors.

### Constraints

- **Phase 1**: No additional hardware purchases - tethered via long USB-C cable
- **Phase 2**: Add sensors for collision detection and autonomy
- **Phase 3**: 3D printed parts for customization and sensor mounts

---

## Development Roadmap

### Phase 1: Software Only (Tethered USB)

No new hardware required. Focus on movement quality and behaviors.

| Priority | Feature | Description |
|----------|---------|-------------|
| P0 | **Smooth Movement** | Replace jerky servo movements with interpolated, organic motion |
| P0 | **Animation System** | Create reusable animation sequences (idle, greet, curious, etc.) |
| P1 | **Random Explore Mode** | Autonomous wandering with random direction changes |
| P1 | **Custom Gaits** | Smoother walking patterns beyond stock 2/4/6-step |
| P2 | **Personality Behaviors** | Idle fidgeting, "looking around", stretch animations |

#### Smooth Movement - Technical Approach

The stock firmware moves servos directly to target positions, causing jerkiness. Solutions:

1. **Servo interpolation** - Break movements into small steps with easing curves
2. **Bezier/spline paths** - Smooth leg trajectories instead of linear moves
3. **Velocity limiting** - Cap maximum servo speed changes
4. **Overlap movements** - Start next movement before current completes

#### Animation System - Concept

```

---

See also: Engineering Notes for implementation details and learnings: `Damson/ENGINEERING_NOTES.md`
Animation = sequence of keyframes
Keyframe = {
  time_offset_ms,
  body_position (x, y, z),
  body_rotation (pitch, roll, yaw),
  leg_positions[6] (optional per-leg overrides)
}
```

Example animations:

- **Idle** - Subtle weight shifting, occasional leg adjustments
- **Alert** - Rise up, look around (body rotation)
- **Greet** - Wave one front leg
- **Stretch** - Extend legs one at a time
- **Sleep** - Slowly lower body, tuck legs

#### Random Explore Mode - Concept

```
loop:
  pick random direction (forward, left, right, turn)
  pick random duration (1-5 seconds)
  execute movement
  pause randomly (0-2 seconds)
  occasionally play idle animation
```

### Phase 2: Add Sensors

Purchase components for collision detection and spatial awareness.

| Component | Purpose | Est. Cost |
|-----------|---------|-----------|
| HC-SR04 Ultrasonic (x2-3) | Front/side obstacle detection | $5 |
| IR Proximity Sensors (x4-6) | Close-range collision, edge detection | $8 |
| MPU6050 IMU | Body tilt sensing, fall detection | $5 |
| Bumper/Contact Switches | Physical collision backup | $3 |

Phase 2 Features:

- Obstacle avoidance during explore mode
- Edge detection (don't walk off tables)
- Self-righting after tips
- Terrain adaptation (stay level on slopes)

### Phase 3: 3D Printed Parts

| Part | Purpose |
|------|---------|
| Sensor mounts | Hold ultrasonic/IR sensors at proper angles |
| Body shell | Aesthetic cover, protection |
| Leg guards | Protect servos from impacts |
| Camera mount | For future computer vision |
| Cable management | Tidy wiring |

### Future: Isaac Sim Training

- **Status**: Awaiting model files from Freenove
- **Goal**: Train walking gaits in simulation, transfer to real robot
- **Benefits**: Optimize for smoothness, efficiency, terrain handling
- **Requirements**: URDF/MJCF model of hexapod with accurate dimensions and servo limits

---

## Reference Projects & Open Source Resources

### MakeYourPet Hexapod (Inspiration)

- **Main repo**: [MakeYourPet/hexapod](https://github.com/MakeYourPet/hexapod) - 3D print files, wiring diagrams
- **Firmware**: [EddieCarrera/chica-servo2040-simpleDriver](https://github.com/EddieCarrera/chica-servo2040-simpleDriver) - C++ for RP2040
- **Platform**: Pimoroni Servo2040 (RP2040-based), controlled via Android app "Chica"
- **Note**: Different hardware (RP2040 vs Arduino Mega), but good reference for smooth servo control using PIOs

### Arduino-Based Hexapod Projects (More Relevant to Freenove)

| Project | Smooth Movement Approach | Notes |
|---------|-------------------------|-------|
| [VorpalHexapod](https://github.com/vorpalrobotics/VorpalHexapod) | Servo interpolation between gamepad packets | Uses Adafruit PWM servo driver, well documented |
| [ArduinoHexapod (etienne-p)](https://github.com/etienne-p/ArduinoHexapod) | Central Pattern Generator (CPG) with Kuramoto oscillators | Smooth gait transitions emerge from coupled oscillators |
| [Hexapod_v4 (JiroRobotics)](https://github.com/JiroRobotics/Hexapod_v4) | Extensive documentation, IMU integration | Uses Adafruit PWM driver + BMI270 IMU |
| [Oscar Liang's Hexapod](https://oscarliang.com/arduino-hexapod-robot/) | Fast trig functions, millis()-based timing | Avoids delay(), uses non-blocking code for smooth motion |

### Key Techniques for Smooth Movement

1. **Servo Interpolation** (Vorpal approach)
   - Break target position into small steps
   - Update servos every N milliseconds toward target
   - Calculate intermediate positions between command updates

2. **Central Pattern Generators** (etienne-p approach)
   - Use coupled oscillators (Kuramoto model) to generate servo signals
   - Gait transitions emerge naturally from phase synchronization
   - More complex but biologically inspired

3. **Non-blocking Timing** (Oscar Liang approach)
   - Replace `delay()` with `millis()` checks
   - Allows continuous servo updates while waiting
   - Fast trig functions (10x faster than standard C)

4. **PIO-based Servo Control** (MakeYourPet/RP2040 approach)
   - Microsecond-precision PWM using programmable I/O
   - Not directly applicable to Arduino Mega but shows importance of timing precision

### Recommended Starting Point for Damson

Based on our Arduino Mega 2560 platform, the **Vorpal Hexapod** approach is most applicable:

1. Implement servo position interpolation in the FNHR library
2. Use non-blocking timing with `millis()` instead of `delay()`
3. Add easing functions (ease-in-out) for organic movement
4. Consider CPG approach later for more advanced gait generation

---

## Technologies

| Component | Technology |
|-----------|------------|
| Microcontroller | Arduino Mega 2560 (ATmega2560) |
| Programming Language | Arduino C/C++, Processing (Java) |
| Actuators | 18 servo motors (3 joints per leg × 6 legs) |
| Wireless Communication | ESP8266 Wi-Fi module, NRF24L01+ 2.4GHz RF |
| Wired Communication | USB Serial (115200 baud) |
| Control Interface | Processing IDE with ControlP5 GUI library |
| Persistent Storage | EEPROM (calibration data) |
| Timer System | FlexiTimer2 (20ms servo update rate) |

---

## Part 1: Robot Capabilities (Without Remote)

### 1.1 Movement

#### Walking Gaits

- **Forward/Backward** - Linear crawling motion
- **Left/Right** - Lateral (sideways) movement
- **Turn Left/Right** - In-place rotation
- **Combined Crawl** - Diagonal movement with simultaneous rotation via `Crawl(x, y, angle)`

#### Gait Patterns (Action Groups)

| Group | Steps per Cycle | Characteristics |
|-------|-----------------|-----------------|
| 1 | 2-step | Fastest, most dynamic |
| 2 | 4-step | Balanced speed/stability |
| 3 | 6-step | Slowest, most stable |

#### Body Control (Legs Stay Planted)

- **Height Adjustment** - Raise/lower body (0-45 range)
- **3D Translation** - Shift body in X/Y/Z (±30 units)
- **3D Rotation** - Pitch/roll/yaw (±10 degrees)
- **Twist** - Combined translation + rotation in one command

#### Individual Leg Control

- Move any of 6 legs independently in 3D space
- Relative positioning (delta X, Y, Z from current position)
- Useful for calibration and custom gaits

#### Speed Control

- Adjustable action speed (1-100 scale)
- Affects all locomotion commands

### 1.2 Power States

| State | Description |
|-------|-------------|
| Active | Fully powered, responsive to commands |
| Sleep | Low-power, legs relaxed |
| Switch | Toggle between active/sleep |

### 1.3 Hardware I/O

#### Available I/O Pins (8 total)

| Pin | Label |
|-----|-------|
| 20 | Digital |
| 21 | Digital |
| A0 | Analog/Digital |
| A1 | Analog/Digital |
| 15 | Digital |
| 14 | Digital |
| 2 | Digital |
| 3 | Digital |

Each pin can be set HIGH/LOW for controlling external devices (LEDs, relays, sensors, etc.)

#### Power Management

- 3 independent power groups for servo load distribution
- Real-time battery voltage monitoring via analog pin A7
- Voltage thresholds: ~6.5V on, ~5.5V auto-off

#### Status LED

- Pin 13 LED for connection/operation status

### 1.4 Sensors (Built-in)

| Sensor | Pin | Function |
|--------|-----|----------|
| Voltage Monitor | A7 | Battery level (25-sample averaging) |
| Servo Position | - | Query current angle of any servo |

### 1.5 Communication Options

| Method | Details |
|--------|---------|
| USB Serial | 115200 baud, auto-detects ports |
| Wi-Fi | ESP8266 AP mode, IP: 192.168.4.1, Port: 65535 |
| RF24 | 2.4GHz, channel 125, 1Mbps, 5-byte address |

Default Wi-Fi: SSID="Freenove Hexapod Robot", Password="Freenove"

### 1.6 Calibration System

- **Installation State** - Initial servo zeroing
- **Calibration State** - Fine-tune servo offsets per leg
- **Boot State** - Ready position
- **Verify** - Confirm calibration accuracy
- All calibration persisted to EEPROM

---

## Part 2: Fun Exploration Projects for Robotics Beginners

### Beginner Projects (No Hardware Mods)

#### 1. Dance Choreographer

Create a sequence of movements to make Damson "dance" to music.

- **Skills learned**: Sequencing commands, timing, loops
- **Use**: Body twist, height changes, turns
- **Challenge**: Sync movements to a beat

#### 2. Obstacle Course Navigator

Program Damson to follow a predetermined path through an obstacle course.

- **Skills learned**: Path planning, movement precision
- **Use**: Forward, turn, lateral movements
- **Challenge**: Complete course in shortest time

#### 3. Gait Explorer

Experiment with different gait patterns and speeds.

- **Skills learned**: Understanding locomotion, gait cycles
- **Use**: Action groups 1-3, speed control
- **Challenge**: Find optimal gait for different surfaces (carpet, tile, grass)

#### 4. Body Language Communicator

Make Damson express "emotions" through body movements.

- **Skills learned**: Creative programming, state machines
- **Use**: Body rotation, height, leg positioning
- **Examples**: "Happy" (bouncing), "Curious" (lean forward), "Tired" (droop low)

#### 5. Follow the Leader

Control Damson in real-time using keyboard input to navigate around your home.

- **Skills learned**: Real-time control, Processing GUI
- **Use**: All movement commands via keyboard shortcuts
- **Challenge**: Navigate between rooms, under tables

### Intermediate Projects (Simple Add-ons)

#### 6. Light Show Bot

Add LEDs to the I/O ports and create light patterns synchronized with movement.

- **Hardware**: LEDs + resistors on pins 20, 21, A0, A1, etc.
- **Skills learned**: Digital I/O, PWM concepts
- **Challenge**: Create "mood lighting" that changes with activity

#### 7. Sound Reactive Walker

Add a sound sensor and make Damson respond to claps or music.

- **Hardware**: Sound sensor module on analog input
- **Skills learned**: Analog input, thresholds, triggers
- **Challenge**: Walk forward on one clap, turn on two claps

#### 8. Boundary Patrol

Add IR sensors to detect edges (table edge, room boundary) and stay within bounds.

- **Hardware**: IR proximity sensors
- **Skills learned**: Sensor integration, conditional logic
- **Challenge**: Patrol a table without falling off

#### 9. Battery Monitor Display

Add an LED bar or small OLED to show battery level visually.

- **Hardware**: LED bar graph or I2C OLED display
- **Skills learned**: Voltage reading, visual feedback, I2C (for OLED)
- **Challenge**: Different LED patterns for charge levels

#### 10. Remote Control via Phone

Create a simple phone app or web interface to control Damson over Wi-Fi.

- **Hardware**: None (uses built-in ESP8266)
- **Skills learned**: Network programming, TCP sockets, UI design
- **Challenge**: Build a virtual joystick interface

### Advanced Projects (More Complex)

#### 11. Terrain Adaptation

Add tilt/accelerometer sensor and adjust body angle to stay level on slopes.

- **Hardware**: MPU6050 or similar IMU
- **Skills learned**: IMU data, real-time feedback loops, PID concepts
- **Challenge**: Walk up a ramp while keeping body level

#### 12. Autonomous Explorer

Add ultrasonic sensors for obstacle detection and create autonomous navigation.

- **Hardware**: HC-SR04 ultrasonic sensors (front, sides)
- **Skills learned**: Distance sensing, decision trees, autonomous behavior
- **Challenge**: Explore a room without bumping into anything

#### 13. Follow Me Bot

Use an IR beacon or Bluetooth signal strength to follow a person.

- **Hardware**: IR receiver + transmitter beacon, or Bluetooth module
- **Skills learned**: Signal tracking, proportional control
- **Challenge**: Maintain consistent following distance

#### 14. Custom Gait Designer

Create entirely new walking patterns beyond the built-in 2/4/6-step gaits.

- **Skills learned**: Inverse kinematics basics, leg coordination, timing
- **Challenge**: Design a "gallop" or "wave" gait

#### 15. Voice Command Control

Add a voice recognition module to control Damson with spoken commands.

- **Hardware**: Voice recognition module (e.g., Elechouse V3)
- **Skills learned**: Voice recognition, command parsing
- **Commands**: "Damson, forward", "Damson, dance", "Damson, sleep"

#### 16. Multi-Robot Coordination

If you have multiple hexapods, make them move in formation.

- **Hardware**: Second hexapod, RF24 communication between them
- **Skills learned**: Robot-to-robot communication, synchronization
- **Challenge**: March in a line, mirror movements

---

## Appendix: Quick Reference

### Keyboard Shortcuts (Processing App - Control Tab)

| Key | Action |
|-----|--------|
| W | Forward |
| S | Backward |
| A | Left |
| D | Right |
| Q | Turn Left |
| E | Turn Right |
| Z | Activate |
| X | Switch Mode |
| C | Deactivate (Sleep) |

### Communication Protocol Summary

- Frame: `[128] [order] [data...] [129]`
- Orders 0-63: Non-blocking (immediate response)
- Orders 64-127: Blocking (responds with start, then done)
- Data values: Offset by +64 to stay in 0-127 range
