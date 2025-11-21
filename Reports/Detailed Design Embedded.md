# Detailed Design

This document delineates the objectives of a comprehensive system design. Upon reviewing this design, the reader should have a clear understanding of:

- How the specific subsystem integrates within the broader solution
- The constraints and specifications relevant to the subsystem
- The rationale behind each crucial design decision
- The procedure for constructing the solution

## General Requirements for the Document

The document should include:

- Explanation of the subsystem’s integration within the overall solution
- Detailed specifications and constraints specific to the subsystem
- Synopsis of the suggested solution
- Interfaces to other subsystems
- 3D models of customized mechanical elements*
- A buildable diagram*
- A Printed Circuit Board (PCB) design layout*
- An operational flowchart*
- A comprehensive Bill of Materials (BOM)
- Analysis of crucial design decisions

*Note: These technical documentation elements are mandatory only when relevant to the particular subsystem.



## Function of the Subsystem

This segment should elucidate the role of the subsystem within the entire system, detailing its intended function, aligned with the conceptual design.

The Embedded Subsystem describes the software for the microcontroller, the connections the microcontroller, and the connections to the sensors, power PCB, etc. The software on the microcontroller will control the heating of the pipe and the water, check for faults with in the system, display to the lcd, interpret sensors and use inputs. The connections will be via terminal blocks on other peripherals and on the shield PCB for the microcontroller. 

## Specifications and Constraints
Overview:
 1. This Microcontroller shall not exceed 100°C during operation. [28]
 2. This Microcontroller shall control the switching frequency of the power board. [34], [35], [36], [38]
 3. This Microcontroller shall output PWM and toggle its pins. [35], [36], [38]
 4. This Microcontroller shall interpret sensors. [46]
 5. This Microcontroller shall have Analog-to-Digital converting capabilities. [46]
 6. This Microcontroller shall interpret user interface inputs.
 7. This Microcontroller shall interface with an LCD. [47]
Overview:
 1. The microcontroller should not exceed 100°C so that the microcontroller does not overheat requiring the MCU to be replaced.
 2. The microcontroller should control the switching frequency of the power board gate drivers. The frequency will control the power level as the frequency shifts away from the resonance frequency of the coil.
 3. The Microcontroller will use GPIO pins to interface with peripherals and use PWM signals to control the frequency sent to the gate drivers.
 4. The Microcontroller will interpret sensors using SPI, I2C commutation protocols and ADC then scale the value.
 5. The Microcontroller will need ADC to read sensors.
 6. The Microcontroller will interpret user input from a rotary encoder and keypad. The rotary encoder will be used to change constants within the system. The keypad will be used to change number values as well and change the mode of the lcd.
 7. The Microcontroller will interface with an LCD to display the current value of sensors, constants, and the current mode. 



This section should provide a list of constraints applicable to the subsystem, along with the rationale behind these limitations. For instance, constraints can stem from physics-based limitations or requirements, subsystem prerequisites, standards, ethical considerations, or socio-economic factors.

The team should set specifications for each subsystem. These specifications may require modifications, which must be authorized by the team. It could be necessary to impose additional constraints as further information becomes available.

Every subsystem must incorporate at least one constraint stemming from standards, ethics, or socio-economic factors.


## Overview of Proposed Solution

Describe the solution and how it will fulfill the specifications and constraints of this subsystem.







## Interface with Other Subsystems

Provide detailed information about the inputs, outputs, and data transferred to other subsystems. Ensure specificity and thoroughness, clarifying the method of communication and the nature of the data transmitted.

- **Inputs:**
Overview:
 1. 5V DC from the power board.  
 2. Reading Micro SD Interface
 3. MAX31855 ADC to SPI for a Thermocouple
 4. MAX31855 ADC to SPI for a Thermocouple
 5. MCP9600 ADC to I2C for a Thermocouple
 6. AD8495 Amplification of a Thermocouple.
 7. Keypad
 8. Rotary Encoder
 9. Flow Sensor
 10. Current Sensor
Specifics:
 1. 5V DC from the power board. 
   The STM32l476RG needs 5V volts from the power board to run all the devices.  
 2. Reading Micro SD Interface
   A micro SD card interface will be read using SPI 1. SPI 1 MISO on PA7 will receive the data off the SD card. SP1 SCLK will output a clock signal to time the data being received on PA5. The CS Pin which enables data transfer is on on PB6. The Micro SD interface will be used to grab the old state of the system including the desired temperature, what mode the lcd is in, etc.
 3. MAX31855 ADC to SPI for a Thermocouple 
   MAX31855 ADC to SPI for a Thermocouple will take the < 50 mV signal from the Thermocouple then amplify the signal. Next, it ADCs the signal and sends that information bit by bit via SPI in this case SPI 2. This Thermocouple converter will be for the output water temperature. Uses SPI 2 MISO on PB14 to receive the data. The SPI 2 CN pin is on PB15 which enables the data reception. The SPI 2 SCLK is on PB13 to time the data.
 4. MAX31855 ADC to SPI for a Thermocouple
   MAX31855 ADC to SPI for a Thermocouple will measure the temperature of the input water. Uses SPI 3 MISO on PC11. The SPI 3 SCLK pin is on PC10. The CN pin is on PD2.
 5. MCP9600 ADC to I2C for a Thermocouple
   MCP9600 ADC to I2C for a Thermocouple is to measure the IGBT Temperature on the power board to make sure it is not exceeding 100°C. The I2C 1 SDA is on SB9 which is the data pin and I2C 1 SCL is the clock pin on PB8.
 6. AD8495 Amplification of a Thermocouple.
   AD8495 Amplification of a Thermocouple will amplify the thermocouple voltage so that the board can ADC the voltage to get a temperature. This thermocouple converter will be for the pipe that is being heated. The signal be received to ADC 1 CH14 on PC4.
 7. Keypad
   The Keypad will be check via on PIN D0-D3 assigned to interrupts. Then D4-D7 will be scan to determine which button was pressed. The keypad will be able to set values for the system and change the mode of the LCD.
 8. Rotary Encoder
   The Rotary Encoder will be able to change values for the system as well depending on the mode of the lcd. This uses TIM 3 in encoder mode on channels 1 and 2 on interrupts to determine if the encoder has changed position. PC6 is TIM 3 CH1 and PC7 is TIM 3 CH2. the Buttons use on PC8 is TBD.
 9. Flow Sensor
   The flow sensor will use ADC 3 CH 4 on PC3. The flow sensor will detect when the water is flowing then turn of the induction gate driver.
 10. Current Sensor
   The current sensor will use ADC 2 CH 3 on PC2. The current sensor will make electrical current flowing in the system is at a safe value so that components don't explode.

- **Outputs:**
Overview:
 1. LCD
 2. PWM signals to Gate Drivers
 3. Writing Micro SD Interface
 4. System Fault Signal
  
Specifics:
 1. LCD
  The LCD uses GPIO Pins A0-A5. A0 which is PA0, connects to the register Select, RS pin. The read/write pin is tied to ground since the LCD only needs to be written to. A1 which is PA1, connects to the Enable pin, EN pin. A2-A5, which is pins PA4, PB0,PC1, and PC0 respectively, is connected to data pin D4-D7. 
 1. PWM signals to Gate Drivers
   PWM signals to Gate Drivers are driven using TIM8 CH3 and TIM8 CH4N to get the same frequency signals but 180 degrees out of phase. TIM8 CH3n is on PB1 and TIM8 CH4 is on PC9.
 2. Writing Micro SD Interface
   Writing Micro SD Interface uses SPI 1 MOSI on PA7 to write data to micro SD card. Inputs 2. explains the rest of the connection for the SD card interface.
 3. System Fault Signal
   The system fault Signal signal will be a GPIO output on PC5
## Buildable Schematic 

Integrate a buildable electrical schematic directly into the document. If the diagram is unreadable or improperly scaled, the supervisor will deny approval. Divide the diagram into sections if the text and components seem too small.

The schematic should be relevant to the design and provide ample details necessary for constructing the model. It must be comprehensive so that someone, with no prior knowledge of the design, can easily understand it. Each related component's value and measurement should be clearly mentioned.
![Embedded System Connection Diagram](./Embedded_System/Embedded_System_Connection_Diagram.png)

## Printed Circuit Board Layout

Include a manufacturable printed circuit board layout.

The microcontroller will have a terminal shield that the PCB subsystem will design. The shield will have 2 sets of double female headers 19 long which will mate to the 2 sets of double male headers on the microcontroller. The female double headers will be soldered to the shield. The left and right side, which will be 19 pins long, of the double headers will be connected to terminal blocks through copper PCB traces. The rows terminal blocks are soldered to the opposite side of the pcb as the female headers. Each pin on the headers will have its own terminal block. Both sets of the headers will be connect to single row terminal blocks. The headers are on opposite sides to the Terminal blocks. The headers and Terminal Blocks pins are spaced 2.54 mm apart. In between the inner rows of terminal blocks, there will be 4 rows of 10 for the microcontroller 5V, 3.3V, ground, and analog ground. The output of the sensor ADCS and amimplfiers will be Terminal blocked as well.

## Flowchart

For sections including a software component, produce a chart that demonstrates the decision-making process of the microcontroller. It should provide an overview of the device's function without exhaustive detail.



![Delay System Flow Chart](./Embedded_System/delay%20system.drawio.png)
  
  

![Micro-Tasked LCD System Flow Chart](./Embedded_System/LCD%20logic.drawio.png)
  


## BOM

Provide a comprehensive list of all necessary components along with their prices and the total cost of the subsystem. This information should be presented in a tabular format, complete with the manufacturer, part number, distributor, distributor part number, quantity, price, and purchasing website URL. If the component is included in your schematic diagram, ensure inclusion of the component name on the BOM (i.e R1, C45, U4).

| Component Name                               | Component Id      | Cost   | Operating Voltage (V) | Max Operating Current (mA) | Power Cost (mW)  | Amount | Total Power (mW) | Total Cost ($) |
| -------------------------------------------- | ----------------- | ------ | --------------------- | -------------------------- | ---------------- | ------ | ---------------- | -------------- |
| STM32Microcontroller [1] [923]               | NUCLEO-L476RG     | $14.52 | 5-12                  | 500                        | 2500 (at 5 V)    | 1      | 2500.0           | $14.52         |
| Adafruit SPI Thermocouple Amplifier  [4]     | MAX31855 Breakout | $14.95 | 3.3                   | 1.5                        | 4.95             | 2      | 9.9              | $29.9          |
| Adafruit I2C  Thermocouple Amplifier [5] [6] | MCP9600 Breakout  | $15.95 | 2.7-5                 | 2.5                        | 8.25 (at 3.3 V)  | 1      | 8.25             | $15.95         |
| Analog Output  Thermocouple Amplifier [7]    | AD8495 Breakout   | $11.95 | 2.7-36                | 0.180                      | 0.594 (at 3.3 V) | 1      | 0.594            | $11.95         |
| Keypad [8] [9#]                              | 3844              | $5.95  | 3.3                   | 33                         | 108.9            | 1      | 108.9            | $5.95          |
| Rotery Encoder [1569] [15610]                | SEN0235           | $2.90  | 5V                    | 10                         | 50               | 1      | 50               | $2.90          |
| LCD [9] [10]                                 | NHD-0216CW-AB3    | $30.87 | 3.3-5                 | 135                        | 675  (at 5 V)    | 1      | 135.0            | $30.87         |
| Adafruit Micro SD Card Interface [12] [13]   | N/A               | $3.50  | 3.3                   | 150                        | 495              | 1      | 495.0            | $3.50          |
| Lem Electric Current Sensor [14] [15]        | Lem HO-10p        | $12.75 | 5                     | 25                         | 125              | 1      | 125.0            | $12.75         |
| Liquid Flow Meter 1/2" [16] [17]             | YF-S201           | $9.95  | 5-18                  | 15                         | 85               | 1      | 85               | $9.95          |
| Total ( electricals not incuding MCU)        | N/A               | N/A    | N/A                   | 203.53 mA (at 5V)          |                  | N/A    | 1017.64          | $138.24        |
## Analysis

Deliver a full and relevant analysis of the design demonstrating that it should meet the constraints and accomplish the intended function. This analysis should be comprehensive and well articulated for persuasiveness.

## References

All sources that have contributed to the detailed design and are not considered common knowledge should be duly cited, incorporating multiple references.

[1] https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf
[2] https://www.nxp.com/docs/en/application-note/AN1259.pdf
[3] https://www.mouser.com/pdfdocs/alphawire-Understanding-Shielded-Cable.pdf?srsltid=AfmBOoppfOhI17DhVwhIefDsZUTN9mPuMLCOyps0Y1D29tUU9E0vIf34
[28] Enisz, K., G. Kohlrusz, D. Fodor, and L. Kovacs. “Degradation Analysis of DC-Link Aluminum Electrolytic Capacitors Operating in PWM Power Converters.” Power Engineering and Electrical Engineering, vol. 18, no. 2, 2020, https://www.researchgate.net/publication/342538837_Degradation_Analysis_of_DC-Link_Aluminium_Electrolytic_Capacitors_Operating_in_PWM_Power_Converters
[34] “IGBTs for Induction Heaters.” Power Electronics News, 2023, https://www.powerelectronicsnews.com/igbts-for-induction-heaters/

[35] Arrow Electronics. What's the Right Switch for You? When to Use Si MOSFETs, IGBTs, and SiC Devices. Arrow Whitepaper, 2023. PDF, https://static4.arrow.com/-/media/Arrow/Files/Pdf/Arrow-IGBT7-Whitepaper.pdf

[36] “MOSFET vs. IGBT: Key Differences.” AllPCB, 2022, https://www.allpcb.com/allelectrohub/mosfet-vs-igbt-key-differences
[38] “IGBT vs. MOSFET: Choosing the Right Semiconductor for High-Power Applications.” JSW XDH, 2021, https://www.jswxdh.com/IGBT-Vs-MOSFET-Choosing-The-Right-Semiconductor-for-High-Power-Applications-id40319716.html
[46] IPC Association Connecting Electronics Industries, "IPC-2222: Sectional Design Standard for Rigid Organic Printed Boards," IPC, Bannockburn, IL, 2013.
[47] IPC Association Connecting Electronics Industries, "IPC-7351B: Generic Requirements for Surface Mount Design and Land Pattern Standard," IPC, Bannockburn, IL, 2010.

[923] https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-L476RG/5347711

[4] https://cdn-shop.adafruit.com/datasheets/MAX31855.pdf
[5] https://cdn-learn.adafruit.com/downloads/pdf/adafruit-mcp9600-i2c-thermocouple-amplifier.pdf
[6] https://cdn.sparkfun.com/assets/a/b/e/5/5/MCP9600-Data-Sheet-DS20005426D.pdf
[7] https://cdn-shop.adafruit.com/datasheets/AD8494_8495_8496_8497.pdf

[8] https://www.digikey.com/en/products/detail/adafruit-industries-llc/3844/9561536?gclsrc=aw.ds&gad_source=1&gad_campaignid=20243136172&gbraid=0AAAAADrbLliZiCrl6hqJBIzBNlEDIPphk&gclid=CjwKCAiAuIDJBhBoEiwAxhgyFuTC3eNILxZ_V3RNUjOgWIFA-pF-Hi-mRqvYBmpmRT8wH40hcAgOmBoCBowQAvD_BwE
[9#] https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/761/3844_Web.pdf?_gl=1*c3szw3*_up*MQ..*_gs*MQ..&gclid=CjwKCAiAuIDJBhBoEiwAxhgyFuTC3eNILxZ_V3RNUjOgWIFA-pF-Hi-mRqvYBmpmRT8wH40hcAgOmBoCBowQAvD_BwE&gclsrc=aw.ds&gbraid=0AAAAADrbLliZiCrl6hqJBIzBNlEDIPphk

[1569] https://www.digikey.com/en/products/detail/dfrobot/SEN0235/7597223?gclsrc=aw.ds&gad_source=1&gad_campaignid=20232005509&gbraid=0AAAAADrbLlhsazWyskHiDysKW0ob--vDy&gclid=CjwKCAiAuIDJBhBoEiwAxhgyFtZPUlC4xZpkvQJvUF6UWXIUL2IbGHd4vTePPCjh1KaX-CtTKIio9hoCYugQAvD_BwE
[15610] https://raw.githubusercontent.com/Arduinolibrary/DFRobot_SEN0235_EC11_Rotary_Encoder_Module/master/EC11I-152%20Datasheet.pdf
[9] https://www.digikey.com/en/products/detail/newhaven-display-intl/NHD-0216CW-AB3/5022941
[10] https://newhavendisplay.com/content/specs/NHD-0216CW-AB3.pdf

[12] https://www.adafruit.com/product/4682
[13] https://cdn-learn.adafruit.com/downloads/pdf/adafruit-microsd-spi-sdio.pdf

[14] https://www.digikey.com/en/products/detail/lem-usa-inc/HO-10-P/4990653?gclsrc=aw.ds&gad_source=1&gad_campaignid=20232005509&gbraid=0AAAAADrbLlhsazWyskHiDysKW0ob--vDy&gclid=CjwKCAiAuIDJBhBoEiwAxhgyFuoWIV3V7RL287N4IRsOCQjxiz_g5lzgpNZoOd94cXDA_auuLb5DhRoCegMQAvD_BwE
[15] https://www.lem.com/sites/default/files/products_datasheets/ho_6_10_25-p_series.pdf

[16] https://www.adafruit.com/product/828
[17] https://cdn-shop.adafruit.com/product-files/828/C898+datasheet.pdf

[89383983298398] https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/761/3844_Web.pdf?_gl=1*xtrx4b*_up*MQ..*_gs*MQ..&gclid=CjwKCAiAuIDJBhBoEiwAxhgyFuTC3eNILxZ_V3RNUjOgWIFA-pF-Hi-mRqvYBmpmRT8wH40hcAgOmBoCBowQAvD_BwE&gclsrc=aw.ds&gbraid=0AAAAADrbLliZiCrl6hqJBIzBNlEDIPphk

[38723872378328] http://file.gifar.com.tw/gifar/character/spec/GFC1602AK-BNFA-JP_Ver.C.pdf