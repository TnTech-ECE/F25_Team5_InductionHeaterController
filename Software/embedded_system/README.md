

### Embedded System Firmware

#### What the software does

**NOTE: Currently just testing the peripherals atm and learning for capstone 2. Chat GPT wrote below.**

Controls the induction heater via the NUCLEO-L476RG.  
-  Generates PWM (two 20–50 kHz complementary outputs and one 80 kHz output) for the power board.  
- (not implemented) Implements PID temperature regulation (pipe and water loop).  
- (Only MAX6675 ATM) Acquires sensor data: MAX31856 (SPI thermocouples), MCP9600 (I2C thermocouple), AD8495 (analog), current sensor (LEM HO-10p), flow meter (not implemented), rotary encoder, keypad (not implemented), and fault lines(not implemented).  
- (1602a atm) (following not implemented) Drives LCD (NHD-0216CW-AB3) display modes (status, set temperature, set power, fault).  
- Saves system state to micro SD card.  
- Provides user input handling (keypad matrix and rotary encoder interrupts).  
Reference design details in [Reports/Detailed Design Embedded.md](../../Reports/Detailed%20Design%20Embedded.md) and subsystem specs in [Reports/Conceptual Design.md](../../Reports/Conceptual%20Design.md).

#### All dependencies
Software:
- STM32CubeIDE and STM32CubeMX
- STM32 HAL / LL drivers (L4 series)
- FatFs (SD card)
- Simple PID module (pid.c / pid.h)
- VS Code with Cortex-Debug extension
Hardware:
- NUCLEO-L476RG board
- 2 MAX31856 (SPI) and their respective thermocouples
- 2 LEM HO-15np current sensor
- YF-S201 flow meter
- NHD-0216CW-AB3 LCD
- 3x4 membrane keypad
- Rotary encoder (SEN0235)
- Micro SD breakout

#### How to Install
1. Clone repository:
   git clone https://github.com/<your-org>/F25_Team5_InductionHeaterController.git
2. Look at slides 3-9 in [Software/embedded_system/STM32Cube_and_IDE_Setup_from_ece4140.pptx](./STM32Cube_and_IDE_Setup_from_ece4140.pptx) for STM32CubeIDE setup

#### How to run/How to use
1. Wire peripherals per shield pinout (see interface description in [Reports/Detailed Design Embedded.md](Reports/Detailed Design Embedded.md)).   
2. Build firmware:
   - CubeIDE: import project and Build.  
3. Flash via ST-Link (make flash or CubeIDE Run).  
4. On boot:
   LCD displays either 

