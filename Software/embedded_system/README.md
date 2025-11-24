

### Embedded System Firmware

#### What the software does

**NOTE: Currently just testing the peripherals atm and learning for capstone 2. Chat GPT wrote below.**

Controls the induction heater via the NUCLEO-L476RG.  
- (not implemented) Generates PWM (two 20–50 kHz complementary outputs and one 80 kHz output) for the power board.  
- (not implemented) Implements PID temperature regulation (pipe and water loop).  
- (Only MAX6675 ATM) Acquires sensor data: MAX31856 (SPI thermocouples), MCP9600 (I2C thermocouple), AD8495 (analog), current sensor (LEM HO-10p), flow meter (not implemented), rotary encoder, keypad (not implemented), and fault lines(not implemented).  
- (1602a atm) (following not implemented) Drives LCD (NHD-0216CW-AB3) display modes (status, set temperature, set power, fault).  
- (not implemented) Logs system state and vitals to micro SD card.  
- (not implemented) Monitors and enforces safety/fault conditions (over‑temp, over‑current, no‑flow).  
- (not implemented) Provides user input handling (keypad scan matrix + rotary encoder interrupts).  
Reference design details in [Reports/Detailed Design Embedded.md](Reports/Detailed Design Embedded.md) and subsystem specs in [Reports/Conceptual Design.md](Reports/Conceptual Design.md).

#### All dependencies
Software:
- ARM GCC toolchain (gcc-arm-none-eabi) or STM32CubeIDE
- CMake or provided Makefile (pending addition)
- STM32 HAL / LL drivers (L4 series)
- FatFs (SD card)
- Simple PID module (pid.c / pid.h)
- VS Code with Cortex-Debug extension
Hardware:
- NUCLEO-L476RG board
- MAX31856 (SPI)
- MCP9600 (I2C)
- AD8495 analog amplifier
- LEM HO-10p current sensor
- YF-S201 flow meter
- NHD-0216CW-AB3 LCD
- 3x4 membrane keypad
- Rotary encoder (SEN0235)
- Micro SD breakout (3.3 V logic)
Support tooling (optional):
- Reference shifting tool in [Software/editing_tools/README.md](Software/editing_tools/README.md) for documentation maintenance.

#### How to Install
1. Clone repository:
   git clone https://github.com/<your-org>/F25_Team5_InductionHeaterController.git
2. Install ARM GCC or STM32CubeIDE.  
3. Add STM32Cube L4 HAL packages (if using CubeIDE, they are bundled).  
4. In VS Code: install Cortex-Debug extension; configure launch.json for ST-Link.  
5. Place source in Software/embedded_system/src (create directories: drivers/, app/, peripherals/, middleware/).  
6. Copy/link third-party libs (FatFs, PID module) into middleware/.  
7. Generate or write CMakeLists.txt / Makefile for build (will be added—placeholder).  

#### How to run/How to use
1. Wire peripherals per shield pinout (see interface description in [Reports/Detailed Design Embedded.md](Reports/Detailed Design Embedded.md)).  
2. Build firmware:  
   - CMake: mkdir build && cd build && cmake .. && make  
   - CubeIDE: import project and Build.  
3. Flash via ST-Link (make flash or CubeIDE Run).  
4. On boot:
   - LCD enters status mode; press A/B/C to cycle modes; use keypad digits + # to commit temperature/power; * clears fault (when safe).  
5. Adjust configuration constants (e.g. PID gains, fault thresholds) in config.h before build.  
6. Retrieve logs from SD card (CSV or binary format—implementation detail).  
7. Monitor real-time values over SWO/USART (optional debug channel).  

```Status Exit Conditions```: Fault mode engages on any asserted fault line or threshold breach; PWM outputs disabled until cleared with * and all conditions nominal.

```Deployment Notes```: Ensure stable 3.3 V domain for logic peripherals; verify SPI/I2C bus pull-ups; synchronize ADC sampling away from PWM edges (timer-triggered conversions).

```Future Additions```: Add calibration routine, configuration persistence, and automated self-test sequence.

```Troubleshooting```:  
- No LCD text: verify supply voltage (3.3–5 V) and interface mode.  
- Missed flow readings: check pulse pin interrupt configuration (rising edge).  
- SD failures: confirm SPI clock < 18 MHz during init.  

```Security/Safety```:
- Enforce max MCU temperature < 100°C via derate/shutdown logic.  
- Immediately latch faults; avoid rapid restart oscillations (add minimum fault hold time).

```Versioning```:
- Tag releases aligned with major hardware revisions of the shield PCB.

```Documentation```:
- Keep operation notes synchronized with [Reports/Conceptual Design.md](Reports/Conceptual Design.md) and update references using editing tools when adding new citations.
