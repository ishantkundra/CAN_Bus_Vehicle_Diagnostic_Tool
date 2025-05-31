# CAN Bus Vehicle Diagnostic Tool

This project reads real-time data (RPM and speed) from a car's OBD-II port via CAN bus and displays it on an OLED screen.

## Features
- Reads PID 0x0C (RPM) and 0x0D (Speed)
- Displays values on I2C OLED (SSD1306)
- Simple CAN message parser

## Hardware Required
- STM32/NXP/Arduino with CAN controller
- MCP2515 CAN Module (SPI)
- OLED Display (SSD1306, I2C)
- OBD-II to DB9 cable

## Files
- `can_reader.c`: Handles CAN communication, parsing, and display logic
- `can.h`, `oled.h`: You must provide low-level drivers or use libraries like `mcp_can_lib` or `STM32Cube HAL`

## Protocol Parsing Logic
### Request
To request a PID (e.g., engine RPM):
```
CAN ID: 0x7DF
Data: [0x02, 0x01, PID, 0x00, 0x00, 0x00, 0x00, 0x00]
```

### Response
Expected from ECU on ID 0x7E8:
- For RPM:
  - Data[2] = A, Data[3] = B
  - RPM = ((A * 256) + B) / 4
- For Speed:
  - Data[2] = A
  - Speed = A km/h

## Notes
- Ensure CAN baud rate matches the car (usually 500kbps)
- Use logic analyzer to debug communication
- Add retry and timeout mechanisms for robustness

## License
MIT