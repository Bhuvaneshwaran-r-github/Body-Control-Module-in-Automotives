# 🚗 Automotive Body Control Module (BCM)

A multi-ECU embedded system demonstrating **CAN bus communication** for automotive body control applications, simulating real-world vehicle electronic control units.

---

## 📋 Project Description

This project implements a **Body Control Module (BCM)** system commonly found in modern vehicles. The BCM coordinates various electronic functions like indicators, motors, and gear systems through a distributed network of Electronic Control Units (ECUs) communicating via the **CAN (Controller Area Network)** protocol.

### What I Built:
- **Dashboard ECU**: Central control unit with LCD display that sends commands to other ECUs
- **Gear System ECU**: Controls DC motor for gear shifting (forward/reverse/idle) with safety interlocks
- **Left & Right Indicator ECUs**: Independent nodes that respond to dashboard commands

### Real-World Application:
This architecture mirrors how actual vehicles work - your car's dashboard, engine, transmission, and lights all communicate over CAN bus, making this project directly relevant to automotive embedded systems roles.

---

## 🛠️ Technologies Used

| Category | Technology |
|----------|------------|
| **Microcontroller** | LPC2129 (ARM7TDMI-S) |
| **Protocol** | CAN 2.0B @ 125 kbps |
| **Transceiver** | MCP2551 |
| **IDE** | Keil µVision |
| **Language** | Embedded C |
| **Motor Driver** | L293D H-Bridge |
| **Display** | 16x2 LCD (4-bit mode) |

---

## 📊 System Architecture

```
                         CAN Bus (125 kbps)
    ══════════════════════════════════════════════════════
         │              │              │              │
    ┌────┴────┐    ┌────┴────┐    ┌────┴────┐    ┌────┴────┐
    │Dashboard│    │  Gear   │    │  Left   │    │  Right  │
    │   ECU   │    │ System  │    │Indicator│    │Indicator│
    │ [TX]    │    │  [RX]   │    │  [RX]   │    │  [RX]   │
    └─────────┘    └─────────┘    └─────────┘    └─────────┘
```

---

## 🔑 Key Skills Demonstrated

### Embedded Systems Development
- Low-level register programming on ARM7 microcontroller
- Peripheral configuration (GPIO, CAN, Timers)
- Interrupt handling and real-time constraints

### CAN Bus Protocol Implementation
- CAN frame structure (ID, DLC, Data, CRC)
- Message arbitration and priority handling
- Multi-node network configuration
- Bit timing calculations for 125 kbps

### Multi-Node Communication
- Designed message protocol with unique IDs (0x221, 0x331, 0x293)
- Implemented broadcast communication model
- Created receiver filtering logic

### Hardware Integration
- LCD interfacing in 4-bit mode
- DC motor control with H-bridge driver
- LED indicators with proper current limiting
- CAN transceiver integration

### Safety & Reliability
- Implemented safety interlocks (buzzer warnings)
- State machine for gear transitions
- Debouncing for switch inputs

---

## 📁 Project Structure

| File | Purpose |
|------|---------|
| `Dashboard.c` | Main ECU - LCD display, button inputs, CAN transmit |
| `Gear_system.c` | Motor control with safety logic |
| `left_indicator.c` | Left turn signal ECU |
| `right_indicator.c` | Right turn signal ECU |
| `lcd_fourbit.h` | LCD driver library |

---

## 🎯 What I Learned

- How automotive networks handle real-time communication
- Importance of message prioritization in CAN
- Designing robust embedded systems with error handling
- Hardware-software co-design for reliable operation
