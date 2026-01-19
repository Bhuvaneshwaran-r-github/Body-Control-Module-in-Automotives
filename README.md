# Body Control Module in Automotives

An automotive Body Control Module (BCM) simulation using LPC2129 microcontroller with CAN bus communication for controlling vehicle indicators, motors, and gear systems.

## Features

- **CAN 2.0B Communication**: 125 kbps CAN bus for inter-ECU messaging
- **Dashboard ECU**: Central control with LCD display
- **Indicator Control**: Left and right turn indicators
- **Motor/Gear Control**: Forward, reverse, and idle gear positions
- **Multi-ECU Architecture**: Distributed control system simulation

## Hardware Requirements

- LPC2129 microcontroller boards (multiple for full simulation)
- CAN transceiver modules (e.g., MCP2551)
- 16x2 LCD display
- LEDs for indicators
- DC motor with L293D driver
- Buzzer for warnings
- Push buttons for input

## Software Prerequisites

- Keil µVision IDE
- LPC21xx header files
- Flash Magic or similar programmer

## Building the Project

1. Open the project in Keil µVision
2. Configure target settings for LPC2129
3. Build each ECU file separately
4. Flash to respective microcontroller boards

## System Architecture

```
┌─────────────┐     CAN Bus     ┌─────────────────┐
│  Dashboard  │◄───────────────►│  Gear System    │
│    ECU      │                 │      ECU        │
└─────────────┘                 └─────────────────┘
       │                               │
       │         CAN Bus               │
       ▼                               ▼
┌─────────────┐                 ┌─────────────────┐
│    Left     │◄───────────────►│     Right       │
│  Indicator  │                 │   Indicator     │
└─────────────┘                 └─────────────────┘
```

## CAN Message IDs

| ID | Description | Data |
|----|-------------|------|
| 0x221 | Left Indicator | ON/OFF state |
| 0x331 | Right Indicator | ON/OFF state |
| 0x293 | Motor Control | 1=CW, 2=ACW, 3=Idle |

## File Structure

| File | Description |
|------|-------------|
| `Dashboard.c` | Main dashboard ECU with LCD and CAN transmit |
| `Gear_system.c` | Motor control ECU with CAN receive |
| `left_indicator.c` | Left indicator ECU |
| `right_indicator.c` | Right indicator ECU |
| `lcd_fourbit.h` | LCD driver header |

## How It Works

1. **Dashboard ECU** monitors switch inputs and sends CAN messages
2. **Indicator ECUs** receive messages and toggle LEDs
3. **Gear System ECU** receives motor commands and controls L293D driver
4. Safety interlocks prevent conflicting operations (buzzer warning)

## CAN Configuration

- Bit Rate: 125 kbps
- Mode: CAN 2.0B (Extended frames supported)
- Acceptance Filter: Disabled (receives all messages)
