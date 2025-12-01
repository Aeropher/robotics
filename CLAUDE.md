# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This repository contains code for **Damson**, a hexapod robot based on the Freenove Hexapod Robot Kit. The robot is a six-legged walker using Arduino and Processing IDEs, controllable via Serial (USB) or Wi-Fi.

### Repository Structure

- `Damson/` - Custom programming for the Damson robot (active development)
  - `arduino/libraries/ProjectDamson/` - Custom Arduino library (forked from Freenove FNHR)
  - `processing/` - Processing control app
  - `PRD.md` - Product requirements and roadmap
- `Reference/` - **READ-ONLY** original Freenove kit source code (do not modify, excluded from git)

## Architecture

### Arduino Library (`Damson/arduino/libraries/ProjectDamson/src/`)

The robot firmware library handles servo control, movement, and communication.

**Key files:**
- `ProjectDamson.h/cpp` - Main robot control class
- `ProjectDamsonBasic.h/cpp` - Servo control, leg positioning, inverse kinematics
- `ProjectDamsonComm.h/cpp` - Communication layer (Serial, Wi-Fi)
- `ProjectDamsonOrders.h` - Protocol command definitions

**Movement System:**
- `Robot::UpdateLegAction()` - Timer-based interpolation (20ms interval via FlexiTimer2)
- Legs move toward goal position in small steps based on `stepDistance`
- Inverse kinematics converts 3D points to servo angles

### Processing Control App (`Damson/processing/`)

Desktop GUI for controlling the robot, built with Processing (Java-based) and ControlP5.

**Key files:**
- `ProcessingApp.pde` - Main sketch with GUI setup and event handling
- `Communication.pde` - Serial and Wi-Fi (TCP client) communication layer
- `ControlRobot.pde` - High-level robot control API wrapping command bytes
- `Orders.pde` - Protocol constants defining command bytes and data format

## Communication Protocol

- Messages framed with `transStart` (128) and `transEnd` (129) bytes
- Order bytes range 0-127: even numbers are requests, odd are responses
- Non-blocking orders (0-63): immediate response
- Blocking orders (64-127): respond with `orderStart`, then `orderDone` on completion
- Data values offset by +64 to keep in 0-127 range

**Connection options:**
- Serial: 115200 baud, auto-detects available ports
- Wi-Fi: TCP client to robot's AP (default SSID: "Damson") at 192.168.4.1:65535

## Running the Processing App

1. Install [Processing IDE](https://processing.org/download)
2. Install the ControlP5 library via Sketch > Import Library > Add Library
3. Open `Damson/processing/ProcessingApp.pde` in Processing
4. Run the sketch

## Robot Control

The robot supports these movement modes:
- Crawl: forward, backward, left, right
- Turn: left, right
- Body twist: 6-axis movement (x/y/z translation + x/y/z rotation)
- Leg calibration: individual leg position adjustment

Keyboard shortcuts (in Control tab): W/A/S/D for movement, Q/E for turning, Z/X/C for mode switching.

## Development Setup (PlatformIO)

The project uses PlatformIO in VS Code for Arduino development. The `platformio.ini` is at the repository root.

**Building and Uploading:**
1. Open the `robotics/` folder in VS Code with PlatformIO extension installed
2. Click checkmark (✓) to build, arrow (→) to upload
3. Robot can be plugged in via USB without battery power for uploading

**Known Behaviors:**
- Startup shake (leg movement) only works when running on battery — does not work while USB is connected
- After uploading, unplug USB and power on with battery to see startup behavior
- Movement functions (`LegMoveToRelatively`, etc.) are disabled when `commFunction=true` — they check `if (!communication.commFunction)` before executing
- To perform movements regardless of comm mode, access `communication.robotAction` directly (see `StartupShake()` implementation)
- Leg numbering is 1-6 (not 0-5) when using `RobotAction` methods
- `LegMoveToRelatively()` calls `WaitUntilFree()` internally, blocking until movement completes
