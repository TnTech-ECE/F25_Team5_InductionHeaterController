
## Function of the Subsystem

The printed circuit board (PCB) serves as the foundation for the induction heater controller. It connects every vital subsystem into a package that fits together. It allows communication between all subsystems, whether it be distributing power, processing sensor feedback, or controlling signals under the micro controller. Together, these functions require a layout that has  proper organization of both high power and low power circuitry for a safe and reliable operation. 

For this reason, the PCB subsystem will be divided into two separate boards: the Power PCB, which will provide the circuitry needed to convert the AC power into regulated waveforms to energise the induction coil, and the Terminal I/O PCB, which organises the microcontroller's I/O pins/ports for user interaction and external connections. With this, these boards will provide a structure that is required for the induction heater to operate according to the conceptual design, as well as ensuring that all subsystems remain connected and functional.

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
*Rationale:* UL/IEC compliance; this prevents fire hazards and protects users from catastrophic failures.[5][7]
- **High Voltage Traces Must Be Physically Isolated from User I/O**  
*Rationale:* This is an ethical obligation to ensure safe operation and prevent accidental shock.

### Socio Economic Constraints
- **Use Components that are Available from Multiple Vendors**  
*Rationale:* This will avoid any supply chain risk issues and help with serviceability.
- **Design Should Allow Easy Assembly and Rework**  
*Rationale:* This will make it much easier for the team to assemble as well as aiding any future improvements.


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
| User Input            | Input     | Terminal I/O |  GPIO | User command input |
| Low-Voltage Power Rails | Both      | Both PCBs | Hardwired | 5 V distribution |



## 3D Model of Custom Mechanical Components

![block_diagram](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/f653ad121fec7d93034aed74f4a0c2e8f299e0e8/Reports/images_PCB/component_3d_Model.jpg)

In this screenshot from the PCB Library Component Designer, this 3D model of a 0603 capacitor shows how the the physical part will sit on the PCB. The footprint includes the pads, outline, and spacing so the component can actually be soldered correctly. The '.Designator' text is just the label (like R1, C5, U3) that tells you which part it is when you're assembling or troubleshooting the board. The box around the footprint represents the component's physical boundary, which helps with placement, clearance, and making sure nothing overlaps when everything is routed.



## Buildable Schematic 

Below are the screenshot of the scematics for both the Power PCB and the Terminal I/O board. Separating the schematics into seperate sections makes it much easier to read and understand what each part of the circuit is doing. If everything was crammed into one big page, it would be messy and hard to follow, especially when you're trying to troubleshoot or make changes. Breaking it into pieces lets you keep related components together and clearly label each function. 

![Terminal IO shield board schematic](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/16d979507bc1b6795d6253559fa29859555ce3d3/Reports/images_PCB/terminal_IO_schematic.jpg)
This is a screenshot of the schematic for the Terminal I/O Board. 
The proceeding schematics are from both Austin Ducrest and Aaron Neuharth, just remade within Altium 

![AC conversion schematic](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/AC.jpg)
AC conversion schematic

![DC schematic part 1](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/DC_1.jpg)
DC schematic part 1

![DC schematic part 2](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/DC_2.jpg)
DC schematic part 2

![Safety schematic part 1](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/Safety_1.jpg)



Safety schematic part 1

![Safety schematic part 2](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/Safety_2.jpg)



Safety schematic part 2

![Gate Driver 1](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/gate_driver_1.jpg)
Gate Driver 1

![Gate Driver 2](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/gate_driver_2.jpg)
Gate Driver 2

![Gate Driver 3](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/gate_driver_3.jpg)
Gate Driver 3

![Gate Driver 4](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/gate_driver_4.jpg)
Gate Driver 4

![Root Schematic part 1](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/root_Sch_1.jpg)

Root Schematic part 1

![Root Schematic part 2](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/root_Sch_2.jpg)
Root Schematic part 2

![Root Schematic part 3](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/root_Sch_3.jpg)
Root Schematic part 3
## Printed Circuit Board Layout

![Power PCB board](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/power%20PCB%20board%20screenshot.jpg)

Power PCB board

![Power PCB board with routes ](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/power%20PCB%20board%20routes%20screenshot.jpg)

Power PCB board with Routes

![IO shield Board ](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/IO%20shield%20Board%20screenshot.jpg)

IO shield Board

![IO shield Board with Routes ](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/Detail_Design_PCB/Reports/images_PCB/IO%20shield%20Board%20routes%20screenshot.jpg)

IO shield Board with Routes

## Flowchart
![block_diagram](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/8fe1f0c4b769cce4be1279822ff243f337da463b/Reports/images_PCB/block%20diagram.jpg)

This Block diagram gives a visual representation of how the set up of the two boards will be connected to each other. The Power PCB handles the high-power side and connects directly to the induction coil, while the Terminal I/O board sits between the power stage and the microcontroller. The microcontroller communicates through the Terminal I/O board to control the Power PCB, and interfaces with the user controlled peripherals as well as display data on the display interface. 


## BOM

 The  Terminal IO board is designed for a 4 layer stack up for the dedicated power, signal, and ground planes. The rough maximum board size is around 5 × 4 inches, with 1 oz copper on both the inside and outside layers fwith a 1.6mm thickness to help with strength and thermal management. Based on these specs, the estimated manufacturing cost from JLCPCB comes out to about **$47.38**.[8]

 **Terminal IO shield Board Components**
| Components         | Manufacturer | Part#           | Distributor | Dist. Part#      | Qty. | Price | Link |
| ------------------ | ------------ | --------------- | ----------- | ---------------- | ---- | ----- | ---- |
| J1,J10,J13,J16     | Samtec       | SSW-110-06-G-S  | Digikey     | SSW-110-06-G-S-ND | 4   | $8.52   | [digikey.com](https://www.digikey.com/en/products/detail/samtec-inc/SSW-110-06-G-S/7877530)
| J2,J5,J6,J9,J11,J12,J14,J15 | On Shore Technology | OSTVN10A150 | Digikey | ED10567-ND | 8 | $29.76 | [digikey.com](https://www.digikey.com/en/products/detail/on-shore-technology-inc/ostvn10a150/1588870) |
| J3,J4              | Sullins Connector Solutions | PPPC202LFBN-RC | Digikey | S7123-ND | 2 | $3.92 | [digikey.com](https://www.digikey.com/en/products/detail/sullins-connector-solutions/pppc202lfbn-rc/810259) |
| ------------------- | ------------ | --------------- | ----------- | ---------------- | ---- | ----- | ---- |
| **Total:** | | | | | | **$42.20** | |

 The  Power board is designed for a 6 layer stack up for the dedicated power, signal, and ground planes. The rough maximum board size is around 12 × 12 inches, with 2 oz copper on both the inside and outside layers for a toal of 4oz of copper, aswell as 2mm thickness to help with strength and thermal management. there will also be edge plating to support EMC performance and grounding. Based on these specs, the estimated manufacturing cost from JLCPCB comes out to about **$376.46**.[8]

 **Power Board Components**
| Components         | Manufacturer | Part#           | Distributor | Dist. Part#      | Qty. | Price | Link |
| ------------------ | ------------ | --------------- | ----------- | ---------------- | ---- | ----- | ---- |
| B1                 | KEMET        | SC-30-E100      | Digikey     | 399-SC-30-E100-ND | 1   | $15.13 | [digikey.com](https://www.digikey.com/en/products/detail/kemet/SC-30-E100/10385065) |
| C1,C3,C5,C7,C9,C11,C13,C15,C17,C19,C21,C22,C28,C29,C30,C31,C36,C37,C38,C39 | Murata Electronics | RDER71E104K0P1H03B | Digikey | 490-8809-ND | 20 | $5.40 | [digikey.com](https://www.digikey.com/en/products/detail/murata-electronics/RDER71E104K0P1H03B/4770964) |
| C2,C4,C6,C8,C10,C12,C14,C16,C18,C20,C23,C47,C48,C50,C55,C68 | Panasonic Electronic Components | ECA-1EM100 | Digikey | P5148-ND | 16 | $2.32 | [digikey.com](https://www.digikey.com/en/products/detail/panasonic-electronic-components/ECA-1EM100/245007) |
| C24,C26,C32,C34    | TDK Corporation | FG18X7R1E105KRT06 | Digikey | 445-173261-1-ND | 4 | $1.20 | [digikey.com](https://www.digikey.com/en/products/detail/tdk-corporation/FG18X7R1E105KRT06/5802875) |
| C25,C27,C33,C35    | ROHM Semiconductor | KTD250B107M80A0B00 | Mouser | 511-KTD250B107M80A0B00CT-ND | 4 | $46.60 | [mouser.com](https://www.mouser.com/ProductDetail/Chemi-Con/KTD250B107M80A0B00?qs=YQnJFR48xcDR2vQIc5p%2FHg%3D%3D&srsltid=AfmBOoqyxxk7kuU38xjR_rxIa0Q1O5qtc2bZE9aHthkMnr-nPpf-SEqL) |
| C40,C41,C58,C59,C60,C61 | Panasonic Electronic Components | ECW-FE2J104PD | Digikey | P124888-ND | 6 | $0.89 | [digikey.com](https://www.digikey.com/en/products/detail/panasonic-electronic-components/ECW-FE2J104PD/10292125) |
| C42                | EPCOS - TDK Electronics | B32676E6205K000 | Digikey | 495-2967-ND | 1 | $3.49 | [digikey.com](https://www.digikey.com/en/products/detail/epcos-tdk-electronics/B32676E6205K000/1277731) |
| C43                | Vishay / Sprague | TVA1703 | Mouser Electronics | 75-TVA1703 | 1 | $7.81 | [mouser.com](https://www.mouser.com/ProductDetail/Vishay-Sprague/TVA1703?qs=Jc0vmPlqnSzO64EtNsmyOw%3D%3D) |
| C44,C45,C46        | Nichicon | UVR2WR47MPD1TA | Mouser Electronics | 647-UVR2WR47MPD1TA | 3 | $0.81 | [mouser.com](https://www.mouser.com/ProductDetail/Nichicon/UVR2WR47MPD1TA?qs=sGAEpiMZZMvwFf0viD3Y3bXvgfG2gej7dC5wqpdmRvg7M7%252BgfNUw5Q%3D%3D) |
| C49                | Murata Electronics | RHS7J2H152J2M2H01A | Digikey | EF2225-ND | 1 | $1.01 | [mouser.com](https://www.mouser.com/ProductDetail/Murata-Electronics/RHS7J2H152J2M2H01A?qs=xhbEVWpZdWf0OAcKZ3HSkA%3D%3D) |
| C51                | KEMET | ECQ-E2225KF | Digikey | 399-R60MF3220AA30KCT-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/detail/panasonic-electronic-components/ECQ-E2225KF/56504?gclsrc=aw.ds&gad_source=1&gad_campaignid=120565755&gbraid=0AAAAADrbLlhVFA4kdfKtYjLTu0_u02k_z&gclid=CjwKCAiAqKbMBhBmEiwAZ3UboHY70d4FeZLD8wrnPCVvqRdf6d-axy_DXUqKA9Gkq7ldYvKHpf8kIhoC-FUQAvD_BwE) |
| C52                | KEMET | R46KI322050M1K | Digikey | 399-R46KI322050M1KCT-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/film-capacitors/62) |
| C53,C62            | TDK Corporation | FG18X5R1E225KRT06 | Digikey | 445-173253-1-ND | 2 | $0.64 | [digikey.com](https://www.digikey.com/en/products/detail/tdk-corporation/FG18X5R1E225KRT06/5802867) |
| C54                | KEMET | C322C471K3G5TA | Digikey | 399-C322C471K3G5TA-ND | 1 | $0.38 | [digikey.com](https://www.digikey.com/en/products/detail/kemet/C322C471K3G5TA/6656585) |
| C64,C65,C66,C67    | WIMA | MKP10-.22/630/5P22 | Mouser Electronics | 505-MKP10-.226305P22 | 4 | $7.52 | [mouser.com](https://www.mouser.com/ProductDetail/WIMA/MKP10-22-630-5P22?qs=FsJ%2FjdNESOVRKe%2FLwIXG3Q%3D%3D) |
| D6                 | Littelfuse | S4X8ES | Digikey | S4X8ES-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/thyristors-scrs/114) |
| D7,D8,D9,D10       | ON Semiconductor | MUR160 | Digikey | MUR160-TPMSCT-ND | 4 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/diodes-rectifiers-single/280) |
| F1,F2              | Littelfuse | 01240061H | Digikey | F9278-ND | 2 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/fuse-holders/152) |
| FR1                | Panjit | GBJ3510 | Digikey | 1655-GBJ3510-ND | 1 | $2.27 | [digikey.com](https://www.digikey.com/en/products/detail/panjit-international-inc/GBJ3510/16526896) |
| G1,G2,G3,G4        | Texas Instruments | UCC21750DW | Mouser Electronics | 595-UCC21750DWR | 4 | $18.20 | [mouser.com](https://www.mouser.com/c/semiconductors/power-management-ics/galvanically-isolated-gate-drivers/?q=UCC21750) |
| K1                 | Panasonic | ANT11SECQE | Digikey | PB1051-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/toggle-switches/199) |
| L1                 | Pulse Electronics | BFSI001211094R7M05 | Digikey | 553-BFSI001211094R7M05CT-ND | 1 | $1.27 | [digikey.com](https://www.digikey.com/en/products/detail/pulse-electronics/BFSI001211094R7M05/16510826) |
| PS1                | MEAN WELL | MPM-10-15 | Mouser Electronics | 709-MPM10-15 | 1 | $10.10 | [mouser.com](https://www.mouser.com/ProductDetail/MEAN-WELL/MPM-10-15?qs=xhbEVWpZdWesTikav2ic2A%3D%3D) |
| PWR1,PWR2          | Eaton | YK8810303000G | Digikey | 281-YK8810303000G-ND | 2 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/terminal-blocks-barrier-blocks/370) |
| Q1,Q2,Q3,Q4        | ROHM | RGW80TS65DGC13 | Mouser Electronics | 755-RGW80TS65DGC13 | 4 | $24.96 | [mouser.com](https://www.mouser.com/ProductDetail/ROHM-Semiconductor/RGW80TS65DGC13?qs=dbcCsuKDzFUOUanxGPiR5w%3D%3D) |
| Q6                 | ON Semiconductor | 2N3904BU | Digikey | 2N3904BU-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/transistors-bipolar-bjt-single/276) |
| R1,R2,R3,R4,R5,R6,R11,R12,R13,R14,R15,R16 | Stackpole Electronics | CF14JT10K0 | Digikey | CF14JT10K0CT-ND | 12 | $0.50 | [digikey.com](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT10K0/1741265) |
| R7,R8,R17,R18      | Yageo | CFR-25JT-52-5R | Digikey | 5.0XBK-ND | 4 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/through-hole-resistors/53) |
| R9,R10,R19,R20,R25,R26,R27,R28 | Stackpole Electronics | CF14JT10R0 | Digikey | CF14JT10R0CT-ND | 8 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/through-hole-resistors/53) |
| R21                | Stackpole Electronics | CF12JT1M00 | Digikey | CF12JT1M00CT-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/through-hole-resistors/53) |
| R22                | Yageo | RSF-50JT-52-330K | Mouser Electronics | 603-RSF50SJT-52-330K | 1 | $0.18 | [mouser.com](https://www.mouser.com/ProductDetail/YAGEO/RSF50SJT-52-330K?qs=sGAEpiMZZMtlubZbdhIBIMMVXX%252BgggzkwMlyWq3j6Pk%3D) |
| R23                | Vishay Dale | RN65E5003FB14 | Digikey | RN65E5003FB14CT-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/through-hole-resistors/53) |
| R24                | Ohmite | WNE5R0FET | Digikey | WNE5R0FETCT-ND | 1 | $2.27 | [digikey.com](https://www.digikey.com/en/products/detail/ohmite/WNE5R0FET/3114558) |
| R31,R32,R33,R34,R35,R36,R38 | Vishay Dale | CRCW080510K0FKEA | Digikey | 541-10KCCCT-ND | 7 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/chip-resistor-surface-mount/52) |
| R37                | Stackpole Electronics | CF14JT1K00 | Digikey | CF14JT1K00CT-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/through-hole-resistors/53) |
| TB1,TB2,TB4        | On Shore Technology | OSTVN06A150 | Digikey | ED10564-ND | 3 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/terminal-blocks-headers-plugs-and-sockets/314) |
| TB3                | On Shore Technology | OSTVN02A150 | Digikey | ED10560-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/terminal-blocks-headers-plugs-and-sockets/314) |
| U1,U2,U3,U4        | Murata | MGJ2D151505SC | Mouser Electronics | 580-MGJ2D151505SC | 4 | $33.84 | [mouser.com](https://www.mouser.com/ProductDetail/Murata-Power-Solutions/MGJ2D151505SC?qs=xLDY6iXSiQYIfXYEkf8RzA%3D%3D) |
| U5                 | Mornsun America | LDE60-20B12 | Digikey | 2725-LDE60-20B12-ND | 1 | $8.86 | [digikey.com](https://www.digikey.com/en/products/detail/mornsun-america-llc/LDE60-20B12/13168172) |
| U7                 | Texas Instruments | LM2940IMP-12/NOPB | Digikey | 296-LM2940IMP-12/NOPBCT-ND | 1 | TBD | [digikey.com](https://www.digikey.com/en/products/filter/pmic-voltage-regulators-linear/699) |
| ------------------- | ------------ | --------------- | ----------- | ---------------- | ---- | ----- | ---- |
| **Total:** | | | | | | **~$165-195** | |

|-------------------|--------------|------------------|-------------|-----------|-------|-------|--------------|
|**Total:** | | | | | | **$179.35** | |


## Analysis

The two-board PCB setup should meet safety and functionality requirements, as it separates the high-power circuitry from the low-voltage control circuitry. Keeping the Power PCB separate from the microcontroller and user interface components reduces electromagnetic interference and makes it easier to meet the spacing rules listed earlier, especially the creepage and clearance limits for high-voltage applications.

From a thermal and electrical standpoint, the Power PCB is designed to handle the high current going through the inverter and bus lines. Using 2 oz copper and wide power traces helps reduce trace heating and keeps components like the rectifier diodes, MOSFETs/IGBTs, and power-sensing parts from overheating. Air gaps and isolation gaps will also be implemented to help prevent flashover or surface tracking [9]. This will also help the microcontroller stay cooler since it's placed on the Terminal I/O PCB, which is physically away from the heat generating parts of the power stage. This helps the microcontroller stay within its temperature rating and reduces the chance of noise coupling into the control signals.

Grounding and signal routing also play a big role in making the system work correctly. A dedicated ground plane on both PCBs gives signals a solid return path and helps prevent noise issues like voltage spikes or incorrect ADC readings. Sensor signals such as current, voltage, and temperature feedback are routed on the Terminal I/O PCB instead of the Power PCB, so they don't get affected by the fast switching from the inverter stage. EMI shielding tape will also be placed around the Terminal I/O board to help reduce any noise coming off the Power PCB from interfering with the microcontroller. This improves the accuracy of the measurements that the microcontroller depends on. Test points will also be added on both boards so that key measurements can be checked during debugging and verification. This makes it easier to check sensor readings, gate signals, and power rails without having to probe directly on small components, reducing the chance of damaging the board during testing.

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

