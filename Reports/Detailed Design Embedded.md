# Detailed Design

<!-- This document delineates the objectives of a comprehensive system design. Upon reviewing this design, the reader should have a clear understanding of:

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

*Note: These technical documentation elements are mandatory only when relevant to the particular subsystem. -->



## Function of the Subsystem

<!-- This segment should elucidate the role of the subsystem within the entire system, detailing its intended function, aligned with the conceptual design. -->

The Embedded Subsystem describes the software for the microcontroller, the connections to the microcontroller, and the connections to the sensors, power PCB, etc. The software on the microcontroller will control the heating of the pipe and the water, check for faults within the system, display to the LCD, interpret sensors, and use inputs. The connections will be via terminal blocks on other peripherals and on the shield PCB for the microcontroller.

## Specifications and Constraints

1. This Microcontroller **shall** not exceed 100°C during operation.  [1]  
2. This Microcontroller **shall** control the switching frequency of the power board. [2], [3], [4], [5]  
3. This Microcontroller **shall** output PWM and toggle its pins. [3], [4], [5]  
4. This Microcontroller **shall** interpret sensors.   [6]  
5. This Microcontroller **shall** have Analog-to-Digital converting capabilities.   [6]
6. This Microcontroller **shall** interpret user interface inputs. 
7. This Microcontroller **shall** interface with an LCD. [7]
8. This Microcontroller **shall** have multiple power settings
   
## Overview of Proposed Solution

1. The microcontroller should not exceed 100°C so that the microcontroller does not overheat, requiring the MCU to be replaced.
2. The microcontroller should control the switching frequency of the power board gate drivers. The frequency will control the power level as the frequency shifts away from the resonance frequency of the coil.
3. The Microcontroller will use GPIO pins to interface with peripherals and use PWM signals to control the frequency sent to the gate drivers.
4. The Microcontroller will interpret sensors using SPI, I2C communication protocols, and ADC, then scale the value.
5. The Microcontroller will need ADC to read sensors.
6. The Microcontroller will interpret user input from a rotary encoder and keypad. The rotary encoder will be used to change constants within the system. The keypad will be used to change number values as well, and change the mode of the LCD.
7. The Microcontroller will interface with an LCD to display the current value of sensors, constants, and the current mode. 
8. The Microcontroller will have multiple power settings changed via the user interface which changes the duty cycle of the PWM sent to the  gate drivers

<!-- This section should provide a list of constraints applicable to the subsystem, along with the rationale behind these limitations. For instance, constraints can stem from physics-based limitations or requirements, subsystem prerequisites, standards, ethical considerations, or socio-economic factors.

The team should set specifications for each subsystem. These specifications may require modifications, which must be authorized by the team. It could be necessary to impose additional constraints as further information becomes available.

Every subsystem must incorporate at least one constraint stemming from standards, ethics, or socio-economic factors. -->

<!-- Describe the solution and how it will fulfill the specifications and constraints of this subsystem. -->
## Interface with Other Subsystems
<!--
Provide detailed information about the inputs, outputs, and data transferred to other subsystems. Ensure specificity and thoroughness, clarifying the method of communication and the nature of the data transmitted.
-->
### Inputs:
#### Overview:
 1. 5V DC from the power board
 2. Reading Micro SD Interface
 3. MAX31856 ADC to SPI for the Pipe Thermocouple
 4. MAX31856 ADC to SPI for the IGBT Thermocouple
 5. MCP9600 ADC to I2C for a Thermocouple
 6. AD8495 Amplification of a Thermocouple
 7. Keypad
 8. Rotary Encoder
 9. Flow Sensor
 10. Current Sensor

#### Specifics:
 1. 5V DC from the power board. 
 The STM32L476RG needs 5V from the power board to run all the devices [8].  
 2. Reading Micro SD Interface
 A micro SD card interface will be read using SPI 1. SPI 1 MISO on PA7 will receive the data from the SD card. SP1 SCLK will output a clock signal to time the data being received on PA5. The CS Pin, which enables data transfer, is on PB6. The Micro SD interface will be used to grab the old state of the system, including the desired temperature, what mode the LCD is in, etc [8] [9].
 3. MAX31856 ADC to SPI for the Pipe Thermocouple
 MAX31856 ADC to SPI for a Thermocouple will take the < 50 mV signal from the Thermocouple, then amplify the signal. Next, it ADCs the signal and sends that information bit by bit via SPI, in this case, SPI 2. This Thermocouple converter will be for the Pipe temperature. Uses SPI 2 MISO on PB14 to receive the data. The SPI 2 CN pin is on PB12, which enables the data reception. The SPI 2 SCLK is on PB13 to time the data. The Over Temperature Fault signal will be on PA11 interrupt [8] [10]. 
 4. MAX31856 ADC to SPI for the IGBT Thermocouple
 MAX31856 ADC to SPI for a Thermocouple will measure the temperature of the IGBT Temp. Uses SPI 3 MISO on PC11. The SPI 3 SCLK pin is on PC10. The CN pin is on PD2. The over-temperature fault signal will be on the PA12 interrupt. [8] [10].
 5. MCP9600 ADC to I2C for a Thermocouple
 MCP9600 ADC to I2C for a Thermocouple is to measure the Input water Temperature on the power board to make sure it does not exceed 100°C. The I2C 1 SDA is on SB9, which is the data pin, and I2C 1 SCL is the clock pin on PB8 [8] [11] [12].
 6. AD8495 Amplification of a Thermocouple
 AD8495 Amplification of a Thermocouple will amplify the thermocouple voltage so that the board can ADC the voltage to get a temperature. This thermocouple converter will be for the pipe that is being heated. The signal is received by ADC 1 CH14 on PC4. [8] [13].
 7. Keypad
 The Keypad will be checked via a PIN D0-D3 assigned to interrupts. Then D4-D7 will be output pins and will be scanned to determine which button was pressed. The keypad will be able to set values for the system and change the mode of the LCD [8] [14].
 8. Rotary Encoder
 The Rotary Encoder will be able to change values for the system as well, depending on the mode of the LCD. This uses TIM 3 in encoder mode on channels 1 and 2 on interrupts to determine if the encoder has changed position. PC6 is TIM 3 CH1 and PC7 is TIM 3 CH2. The buttons used on PC8 are TBD [8] [15].
 9. Flow Sensor
 The flow sensor will use GPIO Interrupt 3 on PC3. The flow sensor will detect when the water is flowing, then turn on the induction gate driver [8] [16].
 10. Current Sensor
 The current sensor will use ADC 2 CH 3 on PC2. The current sensor will ensure that the electrical current flowing in the system is at a safe value so that components don't explode [17] [8]. The overcurrent fault signal will be on the PA15 interrupt.

### Outputs:
#### Overview:
 1. LCD
 2. PWM signals to Gate Drivers
 3. Writing Micro SD Interface
 4. Setting Pipe Fault Temperature For Amplifier
 5. Setting IGBT Fault Temperature For Amplifier

#### Specifics:
 1. LCD
 The LCD uses GPIO Pins A0-A5. A0, which is PA0, connects to the register Select, RS pin. The read/write pin is tied to ground since the LCD only needs to be written to. A1, which is PA1, connects to the Enable pin, EN pin. A2-A5, which is pins PA4, PB0,PC1, and PC0 respectively, is connected to data pin D4-D7 [8].
 2. PWM signals to Gate Drivers
 PWM signals to Gate Drivers are driven using TIM8 CH3 and TIM8 CH4N to get the same frequency signals, but 180 degrees out of phase. TIM8 CH3n is on PB1 and TIM8 CH4 is on PC9. TIM4 Ch2 on PB8 will send an 80KHz PWM signal to the power board [8].
 3. Writing Micro SD Interface
 Writing Micro SD Interface uses SPI 1 MOSI on PA7 to write data to the micro SD card. Inputs 2. explains the rest of the connection for the SD card interface [8] [9].
 4. Setting Pipe Fault Temperature For Amplifier
 Setting Pipe Fault Temperature uses SPI 2 MOSI on PB15. Inputs 4. explains the rest of the connections for the Pipe Temperature Amplifier [8] [10].
 5. Setting IGBT Fault Temperature For Amplifier
 Setting IGBT Fault Temperature uses SPI 3 MOSI on PC12. Inputs 5. explains the rest of the connections for the IGBT Temperature Amplifier [8] [10].
## Buildable Schematic 

<!-- THe instruction text was commented out not sure what you are referring to. 

Integrate a buildable electrical schematic directly into the document. If the diagram is unreadable or improperly scaled, the supervisor will deny approval. Divide the diagram into sections if the text and components seem too small.

The schematic should be relevant to the design and provide ample details necessary for constructing the model. It must be comprehensive so that someone, with no prior knowledge of the design, can easily understand it. Each related component's value and measurement should be clearly mentioned. -->
#### Embedded System Connection Diagram
![Embedded System Connection Diagram](./Embedded_System/Embedded_System_Connection_Diagram.png)

## Printed Circuit Board Layout

<!-- Include a manufacturable printed circuit board layout. -->

The microcontroller will have a terminal shield that the PCB subsystem will design. The shield will have 2 sets of double female headers, 20 long, which will mate to the 2 sets of double male headers on the microcontroller. The female double headers will be soldered to the shield. The left and right sides, which will be 19 pins long, of the double headers will be connected to terminal blocks through copper PCB traces. The rows of terminal blocks are soldered to the opposite side of the PCB as the female headers. Each pin on the headers will have its own terminal block. Both sets of headers will be connected to single-row terminal blocks. The headers are on opposite sides of the Terminal blocks. The headers and Terminal Blocks pins are spaced 2.54 mm apart. In between the inner rows of terminal blocks, there will be 4 rows of 10 for the microcontroller 5V, 3.3V, ground, and analog ground. The output of the sensor ADCS and amplifiers will be Terminal blocked as well.

## Flowcharts

<!-- For sections including a software component, produce a chart that demonstrates the decision-making process of the microcontroller. It should provide an overview of the device's function without exhaustive detail. -->

![LCD Abstracted logic](./Embedded_System/LCD%20abstracted%20logic.drawio.png)

The LCD Abstracted logic Flow chart describes modes for the LCD and the actions that can occur in each mode. The top flow shows the key pad influencing the modes. Whereas, the bottom shows the different modes in action. 


### Proportional Differential Integral Controller Flow Chart
![PID Flowchart](Embedded_System/New_HeatGenerationSubsystem.drawio.png)

The Proportional Differential Integral Flow Chart Controller shows the function of the proportional Differential Integral Controller shows the function of a proportional controller. Depending on the response of the closed-loop proportional controller, the integral and differential components might be needed.

![Delay System Flow Chart](./Embedded_System/delay%20system.drawio.png)

The Delay System Flow Chart explains the function of the logic behind the LCD. The right 3 flows show the 3 different functions that queue runs at different frequencies and life times. The left flow handles the logic which executes the run queue, ticking down delayLeft, and on time it runs the callback, and if the return of run until is true, it removes the delay.
  

![Micro-Tasked LCD System Flow Chart](./Embedded_System/LCD%20logic.drawio.png)

The Micro-Tasked LCD System Flow Chart examines the process by which the LCD runs. The top flow shows the function that queues a byte of data into the circular queue, which could be a command or data that can have a delay. The bottom flow shows the processing of that queue. The program uses a state machine to make sure that the latches have enough time to be detected.


## BOM

<!--
Provide a comprehensive list of all necessary components along with their prices and the total cost of the subsystem. This information should be presented in a tabular format, complete with the manufacturer, part number, distributor, distributor part number, quantity, price, and purchasing website URL. If the component is included in your schematic diagram, ensure inclusion of the component name on the BOM (i.e R1, C45, U4).
-->

| Component Name                                               | Component Id      | Cost                  | Operating Voltage (V) | Max Operating Current (mA) | Power Cost (mW)  | Amount | Total Power (mW) | Total Cost ($) | Product Link                                                                                           |
| ------------------------------------------------------------ | ----------------- | --------------------- | --------------------- | -------------------------- | ---------------- | ------ | ---------------- | -------------- | ------------------------------------------------------------------------------------------------------ |
| STM32Microcontroller [8] [19]                                | NUCLEO-L476RG     | $14.52                | 5-12                  | 500                        | 2500 (at 5 V)    | 1      | 2500.0           | $14.52         | [digikey.com](https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-L476RG/5347711)     |
| Adafruit SPI Thermocouple Amplifier [10] [20]                | MAX31856 Breakout | $17.50                | 3.3                   | 1.5                        | 4.95             | 2      | 9.9              | $35.12         | [adafruit.com](https://www.adafruit.com/product/3263)                                                  |
| Adafruit I2C Thermocouple Amplifier (optional)[12] [11] [21] | MCP9600 Breakout  | $15.95                | 2.7-5                 | 2.5                        | 8.25 (at 3.3 V)  | 1      | 8.25             | $15.95         | [adafruit.com](https://www.adafruit.com/product/4101)                                                  |
| Analog Output Thermocouple Amplifier (optional)[13] [22]     | AD8495 Breakout   | $11.95                | 2.7-36                | 0.180                      | 0.594 (at 3.3 V) | 1      | 0.594            | $11.95         | [adafruit.com](https://www.adafruit.com/product/1727)                                                  |
| Keypad [23] [14]                                             | 3844              | $5.95                 | 3.3                   | 33                         | 108.9            | 1      | 108.9            | $5.95          | [digikey.com](https://www.digikey.com/en/products/detail/adafruit-industries-llc/3844/9561536)         |
| Rotary Encoder [24] [15]                                     | SEN0235           | $2.90                 | 5V                    | 10                         | 50               | 1      | 50               | $2.90          | [digikey.com](https://www.digikey.com/en/products/detail/dfrobot/SEN0235/7597223)                      |
| LCD [25] [18]                                                | NHD-0216CW-AB3    | $30.87                | 3.3-5                 | 135                        | 675 (at 5 V)     | 1      | 135.0            | $30.87         | [digikey.com](https://www.digikey.com/en/products/detail/newhaven-display-intl/NHD-0216CW-AB3/5022941) |
| Adafruit Micro SD Card Interface [26] [9]                    | N/A               | $3.50                 | 3.3                   | 150                        | 495              | 1      | 495.0            | $3.50          | [adafruit.com](https://www.adafruit.com/product/4682)                                                  |
| Lem Electric Current Sensor [27] [17]                        | HO 25-NPPR        | UNKNOWN (emailed LEM) | 5                     | 25                         | 125              | 1      | 125.0            | UNKNOWN        | [lem.com](https://www.lem.com/en/product-list/ho-25nppr)                                               |
| Liquid Flow Meter 1/2" [28] [16]                             | YF-S201           | $9.95                 | 5-18                  | 15                         | 85               | 1      | 85               | $9.95          | [adafruit.com](https://www.adafruit.com/product/828)                                                   |
| Total (electricals not including MCU or                      | N/A               | N/A                   | N/A                   | 203.5 mA (at 5V) < 500 mA  |                  | N/A    | 1008.8           | $122.81        | N/A                                                                                                    |
| ADC/I2C Thermocouple AMP as optional)                        |                   |                       |                       |                            |                  |        |                  |                |                                                                                                        |
## Analysis

<!-- Deliver a full and relevant analysis of the design demonstrating that it should meet the constraints and accomplish the intended function. This analysis should be comprehensive and well articulated for persuasiveness. -->

#### Microcontroller (Constraint 5.)
NUCLEO-L476RG is the microcontroller used since it is what the team is most comfortable with. Also, the MCU fits the teams needs of being low power and having GPIO, ADC, TIMERS, 80MHZ max clock, I2C, SPI, and PWM capabilities. The MCU will have a shield placed on the MCU's double male headers, connecting all the MCU’s pins to terminal blocks. Terminal blocks are used to allow for more flexibility if something needs to be added, removed, or moved. With a PCB that hard-connects all the pins, the PCB would need to be reordered if something changed [8]. 

#### Sensor and Fault Interfacing (Constraint 4. and 3.)
Reading the sensors, temperature sensors will be polled with PID logic, as the temperature sensors do not need to be as fast. The shut off temperatures for the objects the sensors are measuring can be programmed on the amplifier signal directly, a fault after the temperature passes a certain threshold for the MCP9600 and MAX31856 via which is threshold is configured via I2C and SPI, respectively [10] [11]. The direct wiring of fault signals from the amplifier would speed up an issue, disabling the circuit. Due to the majority of the pins on the microcontroller being used, the thermocouples will need to be amplified in different ways. There are only 3 ADC, 3 SPI, and 3 I2C [8]. Since the system has 6 interfaces/sensors, they have to be spread across the ways of interfacing. The thermocouple that is reading the pipe can have the amplifier setup to fault pass a certain temperature. The direct fault signal will speed up the disabling of the system, bypassing SPI interfacing speeds. To set the threshold fault temperature, SPI MOSI will be used. The IGBT termocouples on the power board will have a similar capability. The IGBT will be set up similarly to the pipe amplifier. The fault for the IGBT and the pipe thermocouple will be directly be wired to disable power to the induction circuit and be connected to two pins on interrupts which change the LCD to fault mode display that it was either due to IGBT or the Pipe. The IGBT and Pipe will be amplified and ADCed via a MAX31856 via SPI 3 and SPI 2, respectively, both configured to full duplex master, meaning the MCU can input from and to the output device. The output water temp will be amplified via an AD8495 and ADC 1, as ADC reads a lot faster than I2C. The input water temperature will be amplified via an MCP9600 via I2C as it does not need to read as fast as the output, since it will change at a minimum of every hour.

#### Keypad and User Interface (Constraint 6. and 7.)
The keypad will be used to set the desired temperature and interface with the system. The key A-C will set 3/4 modes of the LCD. The First 3 are the display temp mode (A key), the set temp (B key), and the set power level (C key). The display temp mode displays the desired temperature or power level, the pipe temperature, the output and input water temperature, and the IGBT temperature. Basically, a system vitals mode. The second mode allows the user to change the desired temperature via the keypad or the rotary encoder. The temperature is entered when the user presses pound on the keypad. The same operation is done for the third mode, but for the power level. The 4th mode is the fault mode, it occurs when the system has a fault via overcurrent in the current transducer or over over-temperature fault from any MAX31856 amplifier. Faults are only cleared via the star key. The keypad and rotary encoder will work off of interrupts. D0-D3 interrupts and TIM 3 configured to encoder mode with interrupts [8].

#### Micro SD Card Interface (Constraint 4.)
Adafruit Micro SD Card Interface will be used to save the state of the system and log the system's values over time. The Micro SD Interface is used over ROM as the Interface is easier viewable on a computer for displaying data. The Interface will use SPI 1 configured to full duplex master and set up to use DMA so that the thread is not blocked while saving with the SD card [9]. The interface will log the system pipe temperature, IGBT temperature, Flow Rate, system source current, and possibly the water input and output temperatures with timestamps. The system state will be a saved C struct and will not be readable without a program parsing the data. [8]

#### Liquid Flow Meter (Constraint 4.)
The Liquid Flow Meter will determine if the system is sending power. Having a flow meter to control the sending of power is standard in most tankless water heaters. The system will use a GPIO Interrupts to count the pulses per second, which is 7.5 times more than the flow rate in Liters per minute. [16] The GPIO Interrupts will need to be debounced to make sure that a sprit of noise did not falsely trigger the interrupt. To debounce the interrupt, the system will use runIntervalUntil at 10ms for 100ms, check that the flow is greater than 1 L/min which checks if the signal is continuous. If the signal is continuous, then the system will start sending power to the coil based on the PID controller.  [8]

#### Current Transducer (Constraint 4.)
The LEM current transducer, LEM HO-10P, will use ADC 2. The value from ADC will be scaled with a constant value to show the real current value since the ADC will return a value from 0 to 4096 and is proportional to 0 to 5V. This value will be displayed on the LCD in the temperature display mode. It will also be logged to the SD Card. The fault signal will set the LCD into fault mode display that the fault is from the current transducer. 

#### Power Board Interfacing (Constraint 2.)
The Microcontroller will have to interface with the power board by sending 20-50 kHz PWM signals. Two that are 180 degrees out of phase which can be done via a negated timer alternate pin. The power board also needs an 80KHz PWM signal. The MCU will also receive 5V from a regulator on the power board. The duty cycle of these signal will be changed to increase or reduce the power sent to the coil. 0% being no power and 50% being maximum power. Also, the further away from the resonant frequency the PWM signal is, the less power the pipe will receive from the coil. [8]

#### Safety, Ethics, and Broader Impacts. (Constraint 1.)
The safety concerns are that the microcontroller could over heat. The microcontroller has a thermistor to make sure it does not exceed 100°C and will shutoff if it detects a temperature greater than 100°C [1]. The microcontroller will be in a compartment in the housing, and the digital wires will be insulated to prevent shock, even though 5V tends not to be hazardous. The amplifier for the pipe thermocouple will be outside the housing, but be in its own electrical box so that the cold junction compensation can have a similar ambient temperature as the termocouples. The ethical concerns are that if a user could set any temperature or the PID constants issues could arise. If an LCD mode is added for setting the PID constants, the mode will be password protected to prevent an unauthorized user from setting the values. If any temperature could be set to any value, the system would heat forever, causing the pipe to melt or the water to flash boil, creating a bomb. The temperature setting for the pipe will need to be capped below that which would transfer too much energy causing boiling water, as if the water boils in the pipe, a pipe bomb could be created. If the pipe heats too much, the pipe could melt, or the fittings could also melt. The proportional and integral can be set too high, causing massive overshoot and could cause the system to be unstable, which is similar to if the temperature settings were set too high. The boarder impacts are that in the temperature control will reduce the power consumption over time by only supplying the minimum amount of power to the coil to maintain the desired temperature. The recyclability of the microcontroller shield PCB could be used for other projects to allow for more secure connections but still is still adaptable. 

#### Proportional Integral Differential Controller
The proportional integral differential controller will be implemented on the microcontroller ran about a 100ms interval. The interval will be around 100 ms, as that is the conversion time for the MAX31856, which the pipe will use [10]. The first conversion takes around 200ms, so that will be to be accounted for in the setup of the thermocouple. The proportional component will have a constant that we can change in code, which affects the proportional gain. The proportional gain determines how fast the system reaches the target value. The integral term also has a configurable constant which the term reduces the residual error, increasing the accuracy. The term also configurable smooths out the response and reduces overshoot caused by the integral and differential terms. [29] The controller will be implemented on the microcontroller to get the pipe temperature to a desired value and keep it within a small range around the desired temperature. The goal is within 5 degrees Celsius. Depending on the tuning process, all terms or just the proportional term might be needed. [8]  

#### Micro-Task Delay System
The purpose of the delay system is not to busy wait for a long duration. The system checks every millisecond, counting down from the delay value. This allows other processes to run between the millisecond checks/runs. The contrast is HAL_Delay(), which uses a while loop checking how many ticks have past while is why the system is micro-tasked. Run interval is to repeatedly run a task at a specific interval. The run timeout delays for a specific amount of time then runs the task. The run-interval-until function will run a task-specific interval until the until-callback returns true. [8]

#### Micro-Tasked LCD  System
The LCD system is micro-tasked, as the LCD displaying information is not as important as, for example, the PID controller. Writing data and commands to the LCD will queue to instructions in a circular queue so that reading the next instruction and adding the the queue is O(1) time. The queue runs on a 200-microsecond interval, so it can write a char per 1 ms. The micro-tasked system allows for other tasks to run between the LCD writes so that the processor is more efficiently used. HAL_Delay() is also removed from the system to avoid any busy waiting which this can happen as a uint is stored in the queue. The most significant 7 bits are after the LCD instruction delay from 0-127 ms. The delay is so that the LCD can process the instruction until the next command. The Delay is set based on the LCD data sheet per command execution time [18]. The 9th bit controls whether the instruction is data or a command. Then the 8 least significant bits are the data. [8]

#### Interrupts vs Polling 
Interrupts will be used over polling for user inputs as actions for inputs will only need to take up the thread when the users interacts with an interface. The keypad is for buttons to control the lcd and set values. A rotary controller will also be used to set values. The fault display signals from the sensors will be on interrupts. For less important display and setting of inputs, they will be on Interrupts, so CPU time is not wasted. [8]

<!--
chat
// ...existing code...
Adafruit Micro SD Card Interface will be used to save the state of the system and log the system vitals over time. The Micro SD Interface is used over ROM as the Interface is easier viewable on a computer for displaying data. The Interface will use SPI 1 configured to full duplex master. [9]
// ...existing code...
Adafruit Micro SD Card Interface will be used to save the state of the system and log the system vitals over time. The breakout is formatted with FatFs so log files can be written as CSV snapshots (timestamp, operating mode, temperatures, current, flow rate, fault codes) at 1 Hz during normal operation and immediately upon any fault interrupt. At boot the firmware checks for a configuration file that stores the last commanded temperature, power level, and LCD mode so those values can be restored if power is lost mid-run. SPI1 runs in full-duplex master mode with a DMA-backed double buffer to keep the logging task non-blocking; chip select on `PB6` is abstracted through the [`logging::sd_card`](Software/embedded_system/src/logging/sd_card.cpp) driver so higher-level tasks only queue structured records. A card-detect line on `PA4` forces the system into a safe “no logging” mode with warning banner on the LCD if the SD card is removed, preventing silent data loss. Every file is closed and flushed after each write burst to minimize corruption risk in the event of an unexpected shutdown. [9]
// ...existing code...
Liquid Flow Meter will determine if the system is sending power. Having a flow meter to control the sending of power is standard in most tankless water heaters. The system will use a GPIO Interrupts to count the pulses per second which is 7.5 times more than the flow rate in Liters per minute. [16] [8]
// ...existing code...
Liquid Flow Meter will determine if the system is sending power. Having a flow meter to control the sending of power is standard in most tankless water heaters. Each rising edge on `PC3` increments a hardware counter serviced by EXTI3; pulses are accumulated over a 250 ms window and converted to flow using $Q_{\text{LPM}}=\frac{N_{\text{pulses}}}{7.5}$. A four-sample moving average filters turbulence-induced jitter, while an adaptive timeout flags “no-flow” after 500 ms with zero pulses, immediately inhibiting the gate-driver enable. Calibration constants can be adjusted in `flow_meter_config.json` on the SD card to compensate for viscosity changes. The ISR also debounces the reed output, discarding intervals shorter than 2 ms to avoid double counts caused by mechanical chatter. [16] [8]
// ...existing code...
The Microcontroller will have to interface will the power board by sending 20-50Khz PWM signals. Two that are 180 degrees out of phase which can be done via a negated timer alternate pin. The power board also needs a 80Khz PWM signal. The MCU will also receive 5V from a regulator on the power board. [8]
// ...existing code...
The Microcontroller will have to interface with the power board by sending 20–50 kHz PWM signals that drive the full-bridge through the isolated gate driver. TIM8 is configured in center-aligned mode with complementary outputs on `TIM8_CH3` (`PB1`) and `TIM8_CH3N` (`PA7`) plus programmable dead time, guaranteeing the two legs remain exactly 180° out of phase while protecting against shoot-through. Frequency trimming is executed via ARR updates synchronized to the update event so the firmware can sweep away from resonance without glitching the duty cycle. A secondary carrier on `TIM4_CH2` (`PB8`) generates the fixed 80 kHz excitation required by the resonant tank pre-charge circuitry; its enable line is ANDed with the hardware fault input (BKIN) so any latched fault immediately blanks all PWM. The MCU receives regulated 5 V from the power board through the shield backplane, where local LDOs derive the 3.3 V analog plane and reference for ADC measurements; star-ground routing keeps the logic return separate from the high-current switching ground to reduce injected noise. [8]
// ...existing code...
-->
## References

[1] K. Enisz, G. Kohlrusz, D. Fodor, and L. Kovacs, "Degradation Analysis of DC-Link Aluminum Electrolytic Capacitors Operating in PWM Power Converters," Power Engineering and Electrical Engineering, vol. 18, no. 2, 2020. Available: https://www.researchgate.net/publication/342538837_Degradation_Analysis_of_DC-Link_Aluminium_Electrolytic_Capacitors_Operating_in_PWM_Power_Converters (Accessed: Nov. 23, 2025).

[2] Power Electronics News, "IGBTs for Induction Heaters," 2023. Available: https://www.powerelectronicsnews.com/igbts-for-induction-heaters/ (Accessed: Nov. 23, 2025).

[3] Arrow Electronics, "What’s the Right Switch for You? When to Use Si MOSFETs, IGBTs, and SiC Devices," Whitepaper, 2023. Available: https://static4.arrow.com/-/media/Arrow/Files/Pdf/Arrow-IGBT7-Whitepaper.pdf (Accessed: Nov. 23, 2025).

[4] AllPCB, "MOSFET vs. IGBT: Key Differences," 2022. Available: https://www.allpcb.com/allelectrohub/mosfet-vs-igbt-key-differences (Accessed: Nov. 23, 2025).

[5] JSW XDH, "IGBT vs. MOSFET: Choosing the Right Semiconductor for High-Power Applications," 2021. Available: https://www.jswxdh.com/IGBT-Vs-MOSFET-Choosing-The-Right-Semiconductor-for-High-Power-Applications-id40319716.html (Accessed: Nov. 23, 2025).

[6] IPC, "IPC-2222: Sectional Design Standard for Rigid Organic Printed Boards," 2013.

[7] IPC, "IPC-7351B: Generic Requirements for Surface Mount Design and Land Pattern Standard," 2010.

[8] STMicroelectronics, "STM32 Nucleo-64 boards (MB1136) user manual (UM1724)," PDF, Available: https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf (Accessed: Nov. 23, 2025).

[9] Adafruit, "MicroSD SPI/SDIO Library Guide," PDF, Available: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-microsd-spi-sdio.pdf (Accessed: Nov. 23, 2025).

[10] Analog Devices, "MAX31856 Cold-Junction Compensated Thermocouple-to-Digital Converter," Datasheet, Available: https://www.analog.com/media/en/technical-documentation/data-sheets/MAX31856.pdf (Accessed: Nov. 23, 2025).

[11] Microchip, "MCP9600 Thermocouple EMF Converter with Cold-Junction Compensation," Datasheet, Available: https://cdn.sparkfun.com/assets/a/b/e/5/5/MCP9600-Data-Sheet-DS20005426D.pdf (Accessed: Nov. 23, 2025).

[12] Adafruit, "MCP9600 I2C Thermocouple Amplifier," Guide, Available: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-mcp9600-i2c-thermocouple-amplifier.pdf (Accessed: Nov. 23, 2025).


[13] Analog Devices, "AD8494/AD8495/AD8496/AD8497 Thermocouple Amplifiers," Datasheet, Available: https://cdn-shop.adafruit.com/datasheets/AD8494_8495_8496_8497.pdf (Accessed: Nov. 23, 2025).


[14] Adafruit, "3844 Membrane Keypad Technical Docs," PDF, Available: https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/761/3844_Web.pdf (Accessed: Nov. 23, 2025).

[15] DFRobot, "EC11 Rotary Encoder Module Datasheet," PDF, Available: https://raw.githubusercontent.com/Arduinolibrary/DFRobot_SEN0235_EC11_Rotary_Encoder_Module/master/EC11I-152%20Datasheet.pdf (Accessed: Nov. 23, 2025).

[16] Adafruit, "YF-S201 Flow Meter Datasheet," PDF, Available: https://cdn-shop.adafruit.com/product-files/828/C898+datasheet.pdf (Accessed: Nov. 23, 2025).

[17] LEM, "HO 6/10/25-P Series Datasheet," PDF, Available: https://www.lem.com/sites/default/files/products_datasheets/ho_25-nppr.pdf (Accessed: Nov. 23, 2025).

[18] Newhaven Display, "NHD-0216CW-AB3 Specification," PDF, Available: https://newhavendisplay.com/content/specs/NHD-0216CW-AB3.pdf (Accessed: Nov. 23, 2025).

[19] STMicroelectronics, "NUCLEO-L476RG Product Page," Available: https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-L476RG/5347711 (Accessed: Nov. 23, 2025).

[20] Adafruit Industries, “MAX31856 Thermocouple Amplifier Breakout,” Product Page, 2025. Available: https://www.adafruit.com/product/3263 (Accessed: Nov. 23, 2025).

[21] Adafruit Industries, “MCP9600 Thermocouple Amplifier Breakout,” Product Page, 2025. Available: https://www.adafruit.com/product/4101 (Accessed: Nov. 23, 2025).

[22] Adafruit Industries, “AD8495 Thermocouple Amplifier Breakout,” Product Page, 2025. Available: https://www.adafruit.com/product/1727 (Accessed: Nov. 23, 2025).

[23] Adafruit, "Membrane Keypad - 3x4," Product Page, Available: https://www.digikey.com/en/products/detail/adafruit-industries-llc/3844/9561536 (Accessed: Nov. 23, 2025).

[24] DFRobot, "SEN0235 EC11 Rotary Encoder Module," Product Page, Available: https://www.digikey.com/en/products/detail/dfrobot/SEN0235/7597223 (Accessed: Nov. 23, 2025).

[25] Newhaven Display, "NHD-0216CW-AB3 16x2 OLED Module," Product Page, Available: https://www.digikey.com/en/products/detail/newhaven-display-intl/NHD-0216CW-AB3/5022941 (Accessed: Nov. 23, 2025).

[26] Adafruit, "MicroSD Card Breakout Board+," Product Page, Available: https://www.adafruit.com/product/4682 (Accessed: Nov. 23, 2025).

[27] LEM, "HO 25-NPPR" Product Page, Available: https://www.lem.com/en/product-list/ho-25nppr (Accessed: Nov. 23, 2025). 

[28] Adafruit, "Liquid Flow Meter - 1/2\"," Product Page, Available: https://www.adafruit.com/product/828 (Accessed: Nov. 23, 2025).

[29] LibreTexts, “PID Tuning via Classical Methods,” Chemical Process Dynamics and Controls, 2023. Available: https://eng.libretexts.org/Bookshelves/Industrial_and_Systems_Engineering/Chemical_Process_Dynamics_and_Controls_(Woolf)/09:_Proportional-Integral-Derivative_(PID)_Control/9.03:_PID_Tuning_via_Classical_Methods (Accessed: Nov. 23, 2025).
