
## Function of the Subsystem

The printed circuit board (PCB) serves as the foundation for the induction heater controller. It connects every vital subsystem into a package that fits together. It allows communication between all subsystems, whether it be distributing power, processing sensor feedback, or controlling signals under the micro controller. Together, these functions require a layout that has  proper organization of both high power and low power circuitry for a safe and reliable operation. 

For this reason, the PCB subsystem will be divided into two separate boards: the Power PCB, which will provide the circuitry needed to convert the AC power into regulated waveforms to energize the induction coil, and the Terminal I/O PCB, which organizes the microcontroller's I/O pins/ports for user interaction and external connections. With this, these boards will provide a structure that is required for the induction heater to operate according to the conceptual design, as well as making sure that all subsystems remain connected and functional.

## Specifications and Constraints

### Electrical Constraints
- **Maximum Input Voltage: 240 VAC**  
*Rationale:* If 240 VAC is exceeded, this could lead to insulation failure or failure of components. [1]
- **Isolation Between High and Low Voltage Domains > 6.4 mm creepage/clearance**  
*Rationale:* This is a requirement by IPC 2221B[2] and IEC 60950[3]  to prevent arcing and ensure safe operation.
- **Maximum Switching Frequency: < 50 kHz**  
*Rationale:* If any higher frequencies were used, this would increase Electromagnetic Interference and thermal stress.
- **EMI Reduction**  
  *Rationale:* The PCB layout must minimize EMI to prevent interference with other components. [4]
- **Grounding Requirements**  
  *Rationale:* The PCB will need to have a dedicated ground plane to prevent damaging components and reduce noise. [2]
### Thermal Constraints
- **Maximum Microcontroller Temperature: 125*C**  
*Rationale:* These limitations are stated within the Datasheet to prevent MCU failure.[5]
- **Minimum Copper Thickness: 2 oz on Power PCB**  
*Rationale:* This is required to support the high currents and to reduce trace heating.[6]
### Mechanical and Layout Constraints
- **Two Board Architecture (Power PCB + Terminal I/O PCB)**  
*Rationale:* Separating the high power circuits from the lower voltage user circuits helps with safety concerns as well as reducing any Electromagnetic Interference.
- **Minimum Trace Width for Power Paths: > 100 mil**  
*Rationale:* This will ensure sufficient current carrying capacity.[6]
### User Interface & I/O Constraints
- **Keypad and LCD Wired Only Through Terminal I/O PCB**  
*Rationale:* This will help prevent the user interfaces from being exposed to any high voltage noise environments.
- **External Connectors Must Be Reinforced (Screw Terminals)**  
 *Rationale:* This will help prevent any accidental disconnects and improve mechanical stability.
### Safety Constraints (Standards / Ethical Requirements)
- **Fusing Required on AC Line Input**  
*Rationale:* UL/IEC compliance; prevents fire hazards and protects users from catastrophic failures.[5][7]
- **High Voltage Traces Must Be Physically Isolated from User I/O**  
*Rationale:* Ethical obligation to ensure safe operation and prevent accidental shock.

- **System State Indicators (Power, Heat, Fault LEDs) Must Be Accurate**  
*Rationale:* This will reduce any user risks by clearly showing the operating state of the system.
### Socio Economic Constraints
- **Use Components Available from Multiple Vendors**  
*Rationale:* Avoids supply chain risk issues and makes sure of long term serviceability.
- **Design Should Allow Easy Assembly and Rework**  
*Rationale:* Makes it easier for the team to assemble as well as aids for future improvements.


## Overview of Proposed Solution

The proposed solution for the PCB subsystem consists of a two board architecture that is designed to meet all constraints established in the Specifications and Constraints section. As stated within the function of the subsystem section, the PCB subsystem is divided into two boards, the Power PCB  and the Terminal I/O PCB. Separating these sections into two boards confirms that the creepage/clearance requirements are met, helps reduce electromagnetic interference within components, and even aids with organization of components.

The Power PCB solution is to  provide a platform that converts 240 VAC into a regulated waveform used to energize the induction coil, provided with the control electronics required for this operation of inducting heat. The power PCB will include multiple circuit subsystems, this includes rectification, DC bus filtering, inverter switching, gate drivers, power sensing, and protection/safety hardware. In addition to these functions, the PCB will maintain wide power traces, proper copper thickness, and thermal management practices to support high-current operation while remaining below component temperature limits. 

The Terminal I/O PCB will essentially function as a bread board for the microcontroller, but with better organization and sturdiness.The Terminal I/O Board will carry the user interaction by linking the micro controller with the power PCB, as well as be used for external wiring interface. It will allow the connection of the keypad and rotary encoder modules for the user to operate, as well as connecting the LCD screen for the usage of monitoring live data. The Terminal I/O Board safeguards system modularity and keeps noisy high power circuit subsystems physically separated from user connectors and displays, as well as protecting the microcontroller from any faults within the power PCB.

This two board solution will satisfy all subsystem specifications,which includes voltage limitation, thermal boundaries, EMI considerations, mechanical spacing rules, and safety requirements. This architecture ensures that each functional block can operate within its defined constraints while maintaining system modularity, manufacturability, and long term reliability.


## Interface with Other Subsystems
PCB aids in the communication with every other subsystem of the induction controller. The two board configuration helps with supporting the interfaces of the microcontroller subsystem, power subsystem, and the controls subsystem.


### 1. Interfaces of the Power PCB

#### 1.1 Inputs
- **240 VAC Mains Input**  
  This is delivered through a shielded connector for the rectification process and the conversion into DC voltage.

- **Sense Input**  
  The signals from the sensors will be routed back to the microcontroller for monitoring of data.

- **Microcontroller Input into Power PCB**  
  Low-voltage signals indicating what's the next state for the Power PCB to enter into (enable gate drivers, emergency stop, etc.)

#### 1.2 Outputs
- **Gate Drive Signals**  
  The PWM signals generated from the gate drivers to control the IGBT switching.

- **Fault Output to Microcontroller**  
  Logic signals triggered by any faults occur, this including overcurrent or over temperature.

- **Power Distribution to Terminal I/O PCB**  
The power PCB will supply a regulated 5 V to the microcontroller for powering the board and voltage to components connected to the terminal I/O board.


### 2. Interfaces of the Terminal I/O PCB

#### 2.1 Inputs
- **User Input Signals**  
  These are read by the microcontroller through the GPIO.

- **External Sensor or Auxiliary Inputs**  
  Additional low voltage sensors connected through the screw terminals.

- **Low Voltage Power from Power PCB**  
The 5 V supply to the microcontroller to power the board and supply voltage to components connected to the Terminal I/O board.

#### 2.2 Outputs
- **LCD Display**  
  I2C/SPI/parallel data lines routed between the microcontroller and display that is connected through the terminal I/O board. 

- **Status Indicator Outputs**  
  LEDs for:  
  - Power  
  - Heating  
  - Faults  

- **Microcontroller Communication Lines**  
  Routes of every user interface data between the Terminal I/O PCB and Power PCB.


### 3. Summary of Data Communication Methods

| Interface / Signal Type | Direction | PCB | Method | Description |
|-------------------------|-----------|-----|--------|-------------|
| AC Main Power           | Input     | Power PCB | Hardwired | 240 VAC input to rectifier |
| PWM Gate Signals        | Output    | Power PCB | Digital PWM | Controls IGBT switching |
| Sensor Feedback (I, V, T) | Input   | Power PCB | Analog to ADC | Real-time measurements |
| Fault Line              | Output    | Power PCB | Digital logic | Signals abnormal conditions |
| LCD Control             | Output    | Terminal I/O | I2C / SPI | Sends display data |
| Keypad Input            | Input     | Terminal I/O | Digital GPIO | User command input |
| Encoder Input           | Input     | Terminal I/O | Quadrature GPIO | User rotary input |
| Status LEDs             | Output    | Terminal I/O | Digital | Indicates system status |
| Low-Voltage Power Rails | Both      | Both PCBs | Hardwired | 5 V distribution |



## 3D Model of Custom Mechanical Components

![block_diagram](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/f653ad121fec7d93034aed74f4a0c2e8f299e0e8/Reports/images_PCB/component_3d_Model.jpg)

In this screenshot from the PCB Library Component Designer, this 3D model of a 0603 capacitor shows how the the physical part will sit on the PCB. The footprint includes the pads, outline, and spacing so the component can actually be soldered correctly. The '.Designator' text is just the label (like R1, C5, U3) that tells you which part it is when you're assembling or troubleshooting the board. The box around the footprint represents the component's physical boundary, which helps with placement, clearance, and making sure nothing overlaps when everything is routed.



## Buildable Schematic 

Below are the screenshot of the scematics for both the Power PCB and the Terminal I/O board. Separating the schematics into seperate sections makes it much easier to read and understand what each part of the circuit is doing. If everything was crammed into one big page, it would be messy and hard to follow, especially when you're trying to troubleshoot or make changes. Breaking it into pieces lets you keep related components together and clearly label each function. **There will be more schematic screenshots to come.**

![block_diagram](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/16d979507bc1b6795d6253559fa29859555ce3d3/Reports/images_PCB/terminal_IO_schematic.jpg)
This is a screenshot of the schematic for the Terminal I/O Board. 

## Printed Circuit Board Layout

Still being worked on


## Flowchart
![block_diagram](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/8fe1f0c4b769cce4be1279822ff243f337da463b/Reports/images_PCB/block%20diagram.jpg)

This Block diagram gives a visual representation of how the set up of the two boards will be connected to each other. The Power PCB handles the high-power side and connects directly to the induction coil, while the Terminal I/O board sits between the power stage and the microcontroller. The microcontroller communicates through the Terminal I/O board to control the Power PCB, and interfaces with the user controlled peripherals as well as display data on the display interface. 


## BOM

Since the board is yet to be finalized, these values are a rough estimate. The  board will likely end up being a 4 layer stack up for the dedicated power and ground planes. The rough maximum board size is around 12 × 10 inches, with 2 oz copper on both the inside and outside layers and a 2 mm thickness to help with strength and thermal management. Based on these specs, the estimated manufacturing cost from JLCPCB comes out to about $203.66.[8]

## Analysis

The two board PCB setup should meet safety and functionality requirements since it separates the high power circuitry from the low-voltage control circuitry. Keeping the Power PCB separated from the microcontroller and user interface components  reduces electromagnetic interference and makes it easier to meet the spacing rules listed earlier, especially the creepage and clearance limits for high voltage. 

From a thermal and electrical standpoint, the Power PCB is designed to handle the high current going through the inverter and bus lines. Using 2 oz copper and wide power traces helps reduce trace heating and keeps components like the rectifier diodes, MOSFETs/IGBTs, and power-sensing parts from overheating. Air gaps and isolation gaps will also be implemented to help prevent flashover or surface tracking [9]. This will also help the microcontroller stay cooler since it's placed on the Terminal I/O PCB, which is physically away from the heat generating parts of the power stage. This helps the microcontroller stay within its temperature rating and reduces the chance of noise coupling into the control signals. 

Grounding and signal routing also play a big role in making the system work correctly. A dedicated ground plane on both PCBs gives signals a solid return path and helps prevent noise issues like voltage spikes or incorrect ADC readings. Sensor signals such as current, voltage, and temperature feedback are routed on the Terminal I/O PCB instead of the Power PCB so they don't get affected by the fast switching from the inverter stage. EMI shielding tape will also be placed around the Terminal I/O board to help reduce any noise coming off the Power PCB from interfering with the microcontroller. This improves the accuracy of the measurements the microcontroller depends on. Test points will also be added on both boards so that key measurements can be checked during debugging and verification. This makes it easier to check sensor readings, gate signals, and power rails without having to probe directly on small components, reducing the chance of damaging the board during testing.

The Terminal I/O PCB also helps keep the user interface safe and organized. Since the keypad, LCD, and status LEDs only connect to the low voltage board, the user never interacts with anything close to high voltage. This also keeps the boards easier to build and troubleshoot because each board has its own purpose. With this, based on the electrical, thermal, grounding, and safety considerations, the proposed PCB design should meet the constraints and allow the induction heater to function as intended.


## References

[1]National Fire Protection Association, "NFPA 70: National Electrical Code," Section 210, Branch Circuits, 2023

[2]IPC, "IPC-2221B: Generic Standard on Printed Board Design," Table 6-1 and Table 6-2, 2012

[3]International Electrotechnical Commission, "IEC 60950-1: Information Technology Equipment - Safety - Part 1: General Requirements," Annex G, Table 2Q, 2005.

[4]International Electrotechnical Commission, "IEC 61000-6-3: Electromagnetic Compatibility (EMC) - Part 6-3: Generic Standards - Emission Standard for Residential, Commercial and Light-Industrial Environments," 2020.

[5]STMicroelectronics, "AN5036: Guidelines for Thermal Management on STM32 Applications," Application Note, 2020. [Online]. Available: https://www.st.com/resource/en/application_note/an5036-guidelines-for-thermal-management-on-stm32-applications-stmicroelectronics.pdf

[6]IPC, "IPC-2152: Standard for Determining Current Carrying Capacity in Printed Board Design," 2009.

[7]Underwriters Laboratories, "UL 248-14: Low-Voltage Fuses - Part 14: Supplemental Fuses," 2010.

[8] JLCPCB, “Online PCB Quotation Tool,” 2025. Accessed: Dec. 2, 2025. [Online]. Available: https://cart.jlcpcb.com/quote?stencilLayer=2&stencilWidth=100&stencilLength=100&stencilCounts=5&spm=Jlcpcb.Homepage.1010

[9] Texas Instruments, Designing for High Voltage: PCB Layout Guidelines, Application Report SLVA987, 2021. Available: https://www.ti.com/lit/an/slva987/slva987.pdf

