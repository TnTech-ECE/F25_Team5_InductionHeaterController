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

The overall functionality of the Power Subsystem stems from the idea that every other subsystem cannot function without it. The goal of this system is to an provide adequate, sustainable, and reliable source of power to every other subsystem of this design. Still, there are challenges that can arise. Every electronic system is different. Which means no matter what you do, there will be some sort of difference. In an electrical systems case, voltage and current levels will differ based on load impedance/resistance, as well as other things. This power system design will provide DC Power Rails for all of the different needs of the different systems, regardless of the difference between the systems.

## Specifications and Constraints

This section should provide a list of constraints applicable to the subsystem, along with the rationale behind these limitations. For instance, constraints can stem from physics-based limitations or requirements, subsystem prerequisites, standards, ethical considerations, or socio-economic factors.

The team should set specifications for each subsystem. These specifications may require modifications, which must be authorized by the team. It could be necessary to impose additional constraints as further information becomes available.

Every subsystem must incorporate at least one constraint stemming from standards, ethics, or socio-economic factors.


**format this:

***Specifications
1. The Power System shall encompass safe and proper Ports between all subsections of the project.
2. The Power shall be able to take in a rectified DC bus and properly convert to each loads specified voltage need.
3. The Power shall be placed in a design such that all I/O ports are properly mapped and subsection Ports are clear and efficient.
4. The Power System shall ensure the power is properly filtered to ensure stable voltage levels.

***Constraints 
1. EMI compliance
2. 240 VAC US standard AC main supply -- And why we chose this
3. Ensuring compliance with Lochinvar’s customer specifications and relevant NEC and IEEE safety standards. 


## Overview of Proposed Solution

Describe the solution and how it will fulfill the specifications and constraints of this subsystem.


## Interface with Other Subsystems

Provide detailed information about the inputs, outputs, and data transferred to other subsystems. Ensure specificity and thoroughness, clarifying the method of communication and the nature of the data transmitted.

The Power subsystem will interface with the Safety P&C and Microcontroller subsystems.

##### Microcontroller system interfacing

The Microcontroller will transmit a PWM high and low  input signal through PCB tracing to the  2 IR2184 Gate Driver IC's IN Pins. From there, the Power systems' circuits will effectively amplify and use the signal provided to drive the gates of the H-bridge configuration on the DC main portion of the power board. 

See the Buildable Schematic section for more info on how the signal logic is configured to eventually drive the IGBT gates. 

###### Safety P&C system interfacing

The Safety and P&C system will recieve control signals which will then be used to apply control logic to the system for proper safety shutdown sequences when issues arise with voltage and current instability.


## 3D Model of Custom Mechanical Components

Should there be mechanical elements, display diverse views of the necessary 3D models within the document. Ensure the image's readability and appropriate scaling. Offer explanations as required.


## Buildable Schematic 

Integrate a buildable electrical schematic directly into the document. If the diagram is unreadable or improperly scaled, the supervisor will deny approval. Divide the diagram into sections if the text and components seem too small.

The schematic should be relevant to the design and provide ample details necessary for constructing the model. It must be comprehensive so that someone, with no prior knowledge of the design, can easily understand it. Each related component's value and measurement should be clearly mentioned.


## Printed Circuit Board Layout

Include a manufacturable printed circuit board layout.


## BOM

Provide a comprehensive list of all necessary components along with their prices and the total cost of the subsystem. This information should be presented in a tabular format, complete with the manufacturer, part number, distributor, distributor part number, quantity, price, and purchasing website URL. If the component is included in your schematic diagram, ensure inclusion of the component name on the BOM (i.e R1, C45, U4).

## Analysis

Deliver a full and relevant analysis of the design demonstrating that it should meet the constraints and accomplish the intended function. This analysis should be comprehensive and well articulated for persuasiveness.

## References

All sources that have contributed to the detailed design and are not considered common knowledge should be duly cited, incorporating multiple references.
