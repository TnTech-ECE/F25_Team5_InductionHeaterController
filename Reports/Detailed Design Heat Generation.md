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

The system the team will be implementing can be best understood using a control block diagram: 

![alt text](<Control_Block_Diagram.drawio (1).png>)

For our system: 
 - R(s) = user desired power
 - Y(s) = temperature of the pipe
 - G(s) = dynamics of the induction heating 
 - H(s) = dynamics of the thermocouple sensor
 - Gc(s) = compensation to be implemented to ensure specifications are met
 - summing junction = signals to and from microcontroller

### Control System 

Lochinvar has supplied the team with an OMEO SK-IH18G23T induction cooker [2]. This induction cooker is designed to be used with smooth, flat bottom base cookware. The cooker utilizes an open loop control system operating based on user selected power and time settings. This cooker has no feedback to know the actual temperature of the part heated, but it is able to predict the temperature from the power selected by the user. The cooker is preprogrammed to 10 temperatures of 120°F to 460°F correlating to 180 Watts to 1800 Watts [3]. The OMEO SK-IH18G23T, though the heating itself is open loop, contains sensors providing feedback to protect the cooker. Notably it contains a thermocouple to measure the surface temperature of the induction cooker and an IGBT sensor placed underneath the heatsink to ensure the PCB was not getting too hot. The surface temperature is kept below 280°F while the PCB is kept under 105°F [3]. Ideally, the cooker should remain relatively cool while the part is being heated. If the cooker's surface temperature or the IGBT's temperature rises too high, the controller produces an error code and stops heating [3]. The cooker's heat sink absorbs much of the heat, but if the heat sink fails the controller's components will fail due to overheating and may fail violently at risk of causing harm to operators.


OMEO PCB with Heatsink:
![alt text](<Induction PCB with Heatsink.jpeg>)


OMEO PCB without  Heatsink:
![alt text](<Induction PCB without Heatsink.jpeg>)




The team's system will require a thermocouple sensor to meet customer specifications and ensure that a the induction heater controller produces a temperature rise on the metal. The team shall implement a closed loop control system to ensure that customer specifications are met in an accurate and consistent manner. Open loop control is better for cost efficiency primarily by eliminating the cost of a sensor measuring the part heated, but measuring the part heated is a requirement for this project. 

For this system, the critical component to be implemented will be the compensator and the summing junction to ensure that the temperature output is accurate to the user's input and remains at a safe level. This can be accomplished using hardware, software, or a mix of both. Deciding which to use primarily depends on balancing the need for speed and reliability using hardware like physical relays [9] versus using software to increase reprogrammability and reduce lead times by reducing number of parts ordered. Typically industrial settings implement their control using digital software for this reason to be able to improve performance of a system without having to order multiple resistor and capacitor components for a hardware solution, while power substations will use physical relays to handle higher voltage requirements and prevent fatal errors. 

#### Heat Generation Controls 
Heat Generation Controls will be the controls implemented to meet customer specifications. 

These controls will require: 
- ability to increase and decrease temperature of pipe 
- ability to determine user's desired temp from the user inputted power level
- temperature sensor to ensure actual temp approximates desired temp
- reprogrammability to ensure controller meets specifications despite noise of system dynamics 

This can be attained using: 
- ##### PID Control implemented using a microcontroller 
PID control [10]is compensation solution that is the most comprehensive of compensators available. PID by definition is a Proportional, Integral, and Derivative control that allows for the reduction of error, noise, and chattering of signals respectively. This is achieved primarily by adjusting the values of and ratios of constants Kp, Ki, and Kd associated with each PID parameter. These constants can be calculated using control theory if the system specifications are known and the transfer functions of the plant and sensor are known. These can be determined experimentally, but the constants are usually set in a more ad hoc manner to save time. In both cases the PID control is usually fine tuned by adjusting the constants as needed to get the best response. PID can be implemented using software or hardware, but it is typically done using software. Software allows for reprogrammability, and can be implemented easily using a microcontroller. Hardware PID involves longer lead times to receive multiple parts and would be difficult to fine tune without even longer lead times. For this system, the proportional control will be critical to remove the error signal between the user desired temperature and the actual temperature of the pipe. The other parameters will not be as critical, but they will still be useful to ensure customer specifications are met. 

Using a microcontroller has the additional benefit that the microcontroller will be able to send and receive signals to / from the system. A microcontroller would be able to receive information from the sensor's actual temperature and user's desired temperature and send information to that the system needs to adjust to match actual temp to desired temp. Implementing this control in a microcontroller reduces the need for additional components requiring additional connections to communicate with each other. 

There are less comprehensive compensators available such as phase lead and phase lag, and PID can be reduced to P and so on as needed to reduce costs if needed. However, by implementing this control using software in a microcontroller the team should not incur any additional cost by implementing PID. 

- #### Heat Generation
This subsystem is responsible for ensuring the induction heater is able to produce accurate temperature outputs based on user-specified power inputs. This subsystem ensures that customer specifications are met accurately and reliably utilizing closed loop feedback.
<!--  -->
1. The heat generation subsystem shall measure the initial temperature of the bar stock using a thermocouple and store this temperature using a microcontroller.
2.  The heat generation subsystem shall ensure magnetic isolation is obtained by checking for over- and under- heating throughout the length of the bar stock using thermocouple(s).
3. The heat generation subsystem shall increase the power delivered to the induction coil if the thermocouple senses temperatures below the desired temperature output based on user specified power input.
4. The heat generation subsystem shall reduce the power delivered to the induction coil if the thermocouple senses temperatures exceeding the desired temperature output based on user specified power input.
5. The heat generation subsystem shall accurately measure the final temperature of the bar stock using thermocouple and store this temperature using a microcontroller.
6. The heat generation subsystem shall not heat the circular bar stock to a temperature exceeding 1200°F (649°C) as per IEEE recommended max maintenance temperature for induction heating [5].

The heat generation subsystem primarily focuses on ensuring the output temperature is reached and providing critical information specified by the customer such as total power consumed and total temperature rise of the metal. This subsystem serves as the basis for correcting any errors in the output to best meet customer specifications.

##### Inputs and Outputs

- **Inputs:**
  - Analog voltage signal from microcontroller specifying what power level / temperature is desired
  - Analog voltage signal from feedback thermocouple sensors specifying what actual temperature is
  - Analog voltage signal from feedback wattmeter sensors specifying what actual power consumed is

- **Outputs:**
  - PWM signal from microcontroller to tell the power subsystem to increase the duty cycle to increase power delivered or decrease duty cycle to decrease power delivered to increase / decrease temperature of part respectively.

##### Operation Flow
Shown below is a detailed flow chart of the Heat Generation Subsystem:


![alt text](<HeatGenerationSubsystem.drawio.png>)


## Specifications and Constraints

This section should provide a list of constraints applicable to the subsystem, along with the rationale behind these limitations. For instance, constraints can stem from physics-based limitations or requirements, subsystem prerequisites, standards, ethical considerations, or socio-economic factors.

The team should set specifications for each subsystem. These specifications may require modifications, which must be authorized by the team. It could be necessary to impose additional constraints as further information becomes available.

Every subsystem must incorporate at least one constraint stemming from standards, ethics, or socio-economic factors.


## Overview of Proposed Solution

Describe the solution and how it will fulfill the specifications and constraints of this subsystem.


## Interface with Other Subsystems

Provide detailed information about the inputs, outputs, and data transferred to other subsystems. Ensure specificity and thoroughness, clarifying the method of communication and the nature of the data transmitted.


## 3D Model of Custom Mechanical Components

Should there be mechanical elements, display diverse views of the necessary 3D models within the document. Ensure the image's readability and appropriate scaling. Offer explanations as required.


## Buildable Schematic 

Integrate a buildable electrical schematic directly into the document. If the diagram is unreadable or improperly scaled, the supervisor will deny approval. Divide the diagram into sections if the text and components seem too small.

The schematic should be relevant to the design and provide ample details necessary for constructing the model. It must be comprehensive so that someone, with no prior knowledge of the design, can easily understand it. Each related component's value and measurement should be clearly mentioned.


## Printed Circuit Board Layout

Include a manufacturable printed circuit board layout.


## Flowchart

For sections including a software component, produce a chart that demonstrates the decision-making process of the microcontroller. It should provide an overview of the device's function without exhaustive detail.


## BOM

Provide a comprehensive list of all necessary components along with their prices and the total cost of the subsystem. This information should be presented in a tabular format, complete with the manufacturer, part number, distributor, distributor part number, quantity, price, and purchasing website URL. If the component is included in your schematic diagram, ensure inclusion of the component name on the BOM (i.e R1, C45, U4).

## Analysis

Deliver a full and relevant analysis of the design demonstrating that it should meet the constraints and accomplish the intended function. This analysis should be comprehensive and well articulated for persuasiveness.

## References

All sources that have contributed to the detailed design and are not considered common knowledge should be duly cited, incorporating multiple references.
