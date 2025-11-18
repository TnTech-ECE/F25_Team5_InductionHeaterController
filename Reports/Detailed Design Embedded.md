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

The Embededded Subsystem describes the software for the microcontroller, the connections the microcontoller, and the connections to the sensors, power PCB, etc.

## Specifications and Constraints

This Microcontroller shall not exceed 100°C during operation. [28]
This Microcontroller shall control the switching frequency of the power board. [34], [35], [36], [38]
This Microcontroller shall output PWM and toggle its pins. [35], [36], [38]
This Microcontroller shall interpret sensors. [46]
This Microcontroller shall have Analog-to-Digital converting capabilities. [46]
This Microcontroller shall interpret user interface inputs.
This Microcontroller shall interface with an LCD. [47]



This section should provide a list of constraints applicable to the subsystem, along with the rationale behind these limitations. For instance, constraints can stem from physics-based limitations or requirements, subsystem prerequisites, standards, ethical considerations, or socio-economic factors.

The team should set specifications for each subsystem. These specifications may require modifications, which must be authorized by the team. It could be necessary to impose additional constraints as further information becomes available.

Every subsystem must incorporate at least one constraint stemming from standards, ethics, or socio-economic factors.


## Overview of Proposed Solution

Describe the solution and how it will fulfill the specifications and constraints of this subsystem.







## Interface with Other Subsystems

Provide detailed information about the inputs, outputs, and data transferred to other subsystems. Ensure specificity and thoroughness, clarifying the method of communication and the nature of the data transmitted.

- **Inputs:**
  1. 5V DC from the power board.
  2. Reading Micro SD Interface
  3. MAX31855 Thermocouple ADC to SPI
  4. MAX31855 ADC to SPI for a Thermocouple
  5. MCP9600 ADC to I2C for a Thermocouple
  6. AD8495 Amplify
  7. Keypad
  8. Rotary Encoder
  9. Flow Sensor
  10. Current Sensor
 
  - User input commands from the HMI (LCD buttons or keypad)  

- **Outputs:**
  1. LCD
  2. PWM signals to Gate Drivers
  3. Writing Micro SD Reader
  4. 


## Buildable Schematic 

Integrate a buildable electrical schematic directly into the document. If the diagram is unreadable or improperly scaled, the supervisor will deny approval. Divide the diagram into sections if the text and components seem too small.

The schematic should be relevant to the design and provide ample details necessary for constructing the model. It must be comprehensive so that someone, with no prior knowledge of the design, can easily understand it. Each related component's value and measurement should be clearly mentioned.
![Embedded System Connection Diagram](./Embedded_System/Embedded_System_Connection_Diagram.png)

## Printed Circuit Board Layout

Include a manufacturable printed circuit board layout.

The microcontroller will have a terminal shield that the PCB subsystem will design. The shield will have 2 sets of double female headers 19 long which will mate to the 2 sets of double male headers on the microcontroller. The female double headers will be soldered to the shield. The left and right side, which will be 19 pins long, of the double headers will be connected to terminal blocks through copper PCB traces. The rows terminal blocks are soldered to the opposite side of the pcb as the female headers. Each pin on the headers will have its own terminal block. Both sets of the headers will be connect to single row terminal blocks. The headers are on opposite sides to the Terminal blocks. The headers and Terminal Blocks pins are spaced 2.54 mm apart. In between the inner rows of terminal blocks, there will be 4 rows of 10 for the microcontroller 5V, 3.3V, ground, and analog ground. The output of the sensor ADCS and amimplfiers will be Terminal blocked as well.

## Flowchart

For sections including a software component, produce a chart that demonstrates the decision-making process of the microcontroller. It should provide an overview of the device's function without exhaustive detail.

## BOM

Provide a comprehensive list of all necessary components along with their prices and the total cost of the subsystem. This information should be presented in a tabular format, complete with the manufacturer, part number, distributor, distributor part number, quantity, price, and purchasing website URL. If the component is included in your schematic diagram, ensure inclusion of the component name on the BOM (i.e R1, C45, U4).

## Analysis

Deliver a full and relevant analysis of the design demonstrating that it should meet the constraints and accomplish the intended function. This analysis should be comprehensive and well articulated for persuasiveness.

## References

All sources that have contributed to the detailed design and are not considered common knowledge should be duly cited, incorporating multiple references.

[28] Enisz, K., G. Kohlrusz, D. Fodor, and L. Kovacs. “Degradation Analysis of DC-Link Aluminium Electrolytic Capacitors Operating in PWM Power Converters.” Power Engineering and Electrical Engineering, vol. 18, no. 2, 2020, https://www.researchgate.net/publication/342538837_Degradation_Analysis_of_DC-Link_Aluminium_Electrolytic_Capacitors_Operating_in_PWM_Power_Converters
[34] “IGBTs for Induction Heaters.” Power Electronics News, 2023, https://www.powerelectronicsnews.com/igbts-for-induction-heaters/

[35] Arrow Electronics. What's the Right Switch for You? When to Use Si MOSFETs, IGBTs, and SiC Devices. Arrow Whitepaper, 2023. PDF, https://static4.arrow.com/-/media/Arrow/Files/Pdf/Arrow-IGBT7-Whitepaper.pdf

[36] “MOSFET vs. IGBT: Key Differences.” AllPCB, 2022, https://www.allpcb.com/allelectrohub/mosfet-vs-igbt-key-differences
[38] “IGBT vs. MOSFET: Choosing the Right Semiconductor for High-Power Applications.” JSW XDH, 2021, https://www.jswxdh.com/IGBT-Vs-MOSFET-Choosing-The-Right-Semiconductor-for-High-Power-Applications-id40319716.html
[46] IPC Association Connecting Electronics Industries, "IPC-2222: Sectional Design Standard for Rigid Organic Printed Boards," IPC, Bannockburn, IL, 2013.
[47] IPC Association Connecting Electronics Industries, "IPC-7351B: Generic Requirements for Surface Mount Design and Land Pattern Standard," IPC, Bannockburn, IL, 2010.