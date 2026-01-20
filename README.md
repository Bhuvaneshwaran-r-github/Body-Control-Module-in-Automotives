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
                              CAN Bus Network (125 kbps)

  ┌──────┐                                                           ┌──────┐
  │ 120Ω │                                                           │ 120Ω │
  │      │                                                           │      │
  └──┬───┘                                                           └───┬──┘
     │         │              │              │              │            │
CAN-H├─────────┼──────────────┼──────────────┼──────────────┼────────────┤
     │         │              │              │              │            │
     │    ┌────┴────┐    ┌────┴────┐    ┌────┴────┐    ┌────┴────┐      │
     │    │ MCP2551 │    │ MCP2551 │    │ MCP2551 │    │ MCP2551 │      │
     │    │Xcvr     │    │Xcvr     │    │Xcvr     │    │Xcvr     │      │
     │    └────┬────┘    └────┬────┘    └────┬────┘    └────┬────┘      │
     │         │              │              │              │            │
CAN-L├─────────┼──────────────┼──────────────┼──────────────┼────────────┤
     │         │              │              │              │            │
  ┌──┴───┐     │              │              │              │       ┌────┴──┐
  │ 120Ω │     │              │              │              │       │ 120Ω  │
  └──────┘     │              │              │              │       └───────┘
               │              │              │              │
          ┌────┴────┐    ┌────┴────┐    ┌────┴────┐    ┌────┴────┐
          │LPC2129  │    │LPC2129  │    │LPC2129  │    │LPC2129  │
          │Dashboard│    │  Gear   │    │  Left   │    │  Right  │
          │   ECU   │    │ System  │    │Indicator│    │Indicator│
          │         │    │   ECU   │    │   ECU   │    │   ECU   │
          │ [LCD]   │    │ [Motor] │    │  [LED]  │    │  [LED]  │
          │[Buttons]│    │[Buzzer] │    │         │    │         │
          └─────────┘    └─────────┘    └─────────┘    └─────────┘
               │              │              │              │
            TX: All       RX: 0x293      RX: 0x221      RX: 0x331
            Messages      Motor Cmd      Left Ind       Right Ind
```

### CAN Bus Physical Layer
- **CAN-H (High)**: Dominant = 3.5V, Recessive = 2.5V
- **CAN-L (Low)**: Dominant = 1.5V, Recessive = 2.5V
- **Differential Voltage**: Dominant = 2V, Recessive = 0V
- **Termination**: 120Ω resistors at both ends of the bus to prevent signal reflections
- **Transceiver**: MCP2551 converts TTL signals from LPC2129 to differential CAN signals

## CAN Message IDs

| ID | Source | Destination | Description | Data |
|----|--------|-------------|-------------|------|
| 0x221 | Dashboard | Left Indicator | Toggle left indicator | 1=ON, 0=OFF |
| 0x331 | Dashboard | Right Indicator | Toggle right indicator | 1=ON, 0=OFF |
| 0x293 | Dashboard | Gear System | Motor control command | 1=CW, 2=ACW, 3=Idle |

## File Structure

| File | Description |
|------|-------------|
| `Dashboard.c` | Main dashboard ECU with LCD and CAN transmit |
| `Gear_system.c` | Motor control ECU with CAN receive |
| `left_indicator.c` | Left indicator ECU |
| `right_indicator.c` | Right indicator ECU |
| `lcd_fourbit.h` | LCD driver header |

## How It Works

1. **Dashboard ECU** monitors switch inputs and broadcasts CAN messages
2. **Indicator ECUs** filter messages by ID and toggle LEDs accordingly
3. **Gear System ECU** receives motor commands and controls L293D driver
4. Safety interlocks prevent conflicting operations (buzzer warning)

## CAN Configuration

- Bit Rate: 125 kbps
- Mode: CAN 2.0B (Extended frames supported)
- Acceptance Filter: Disabled (receives all messages)
- Bus Termination: 120Ω at each end
- Transceiver: MCP2551 (3.3V/5V compatible)
