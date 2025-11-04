# Detailed Design


## Function of the Subsystem


These controls will require: 
- ability to increase and decrease temperature of pipe 
- ability to determine user's desired temp from the user inputted power level
- temperature sensor to ensure actual temp approximates desired temp
- reprogrammability to ensure controller meets specifications despite noise of system dynamics 


This subsystem is responsible for ensuring the induction heater is able to produce accurate temperature outputs based on user-specified power inputs. This subsystem ensures that customer specifications are met accurately and reliably utilizing closed loop feedback.


The heat generation subsystem primarily focuses on ensuring the output temperature is reached and providing critical information specified by the customer such as total power consumed and total temperature rise of the metal. This subsystem serves as the basis for correcting any errors in the output to best meet customer specifications.



## Specifications and Constraints

### Specifications
Based on the deliverables given to us by Lochinvar:

1. This controller shall have multiple power settings similar to current induction cook tops.
2. This controller shall be capable of inducing surface eddy currents and internal heating.
3. This controller shall test circular bar stock and standard black pipe thermocouple in such a manner to validate the success of the heating.
4. This controller shall measure total electrical power consumed.  
5. This controller shall measure the total temperature rise of the metal.

### Contraints
NEC Article 427 [2] covers Fixed Electric Heating Equipment for Pipelines and Vessels. Part V covers the codes regarding Induction Heating, and Part VII covers the Controls and Protection. Notably if the controller contains an off switch it should not be able to energize automatically while in the off position, and selecting off should open all ungrounded conductors.


NEC Article 665 [3] covers Induction and Dielectric Heating Equipment. Part I has guidelines "the current flow from the output circuit to ground under operating and ground-fault conditions shall be limited to a value that does not cause 50 volts or more to ground to appear on any accessible part of the heating equipment and its load." Part II states the control panel should be dead front construction, that is: no live circuits at risk of shocking people. 


ANSI/IEEE 844-200 [4] applies directly to induction heating for pipelines and vessels. It states that the electric shock hazard for induction heating is minimal, but the high temperatures and induced current flow are design considerations [4]. There are a couple other IEEE standards related to heating such as ANSI/IEEE 515-2011 with standards for industrial electrical trace heating applications and ANSI/NECA 202-2013 with standards for installing and maintaining industrial heat tracing systems [2]. 


## Overview of Proposed Solution

This subsystem is primarily concerned with meeting the customer Specifications 3 and 5, and interfacing with the power subsystem to meet Specifications 1,2, and 4. 

1. The heat generation subsystem shall measure the initial temperature of the bar stock using a thermocouple and store this temperature using a microcontroller.
2.  The heat generation subsystem shall check for over- and under- heating throughout the length of the bar stock using thermocouple(s).
3. The heat generation subsystem shall increase the power delivered to the induction coil if the thermocouple senses temperatures below the desired temperature output based on user specified power input.
4. The heat generation subsystem shall reduce the power delivered to the induction coil if the thermocouple senses temperatures exceeding the desired temperature output based on user specified power input.
5. The heat generation subsystem shall accurately measure the final temperature of the bar stock using thermocouple and store this temperature using a microcontroller.
6. The heat generation subsystem shall not heat the circular bar stock to a temperature exceeding 1200°F (649°C) as per IEEE recommended max maintenance temperature for induction heating [5].


The system the team will be implementing can be best understood using a control block diagram: 

![alt text](<Control_Block_Diagram.drawio (1).png>)

For our system: 
 - R(s) = user desired power
 - Y(s) = temperature of the pipe
 - G(s) = dynamics of the induction heating 
 - H(s) = dynamics of the thermocouple sensor
 - Gc(s) = compensation to be implemented to ensure specifications are met
 - summing junction = signals to and from microcontroller

PID control [1]is compensation solution that is the most comprehensive of compensators available. PID by definition is a Proportional, Integral, and Derivative control that allows for the reduction of error, noise, and chattering of signals respectively. This is achieved primarily by adjusting the values of and ratios of constants Kp, Ki, and Kd associated with each PID parameter. These constants can be calculated using control theory if the system specifications are known and the transfer functions of the plant and sensor are known. These can be determined experimentally, but the constants are usually set in a more ad hoc manner to save time. In both cases the PID control is usually fine tuned by adjusting the constants as needed to get the best response. PID can be implemented using software or hardware, but it is typically done using software. Software allows for reprogrammability, and can be implemented easily using a microcontroller. Hardware PID involves longer lead times to receive multiple parts and would be difficult to fine tune without even longer lead times. For this system, the proportional control will be critical to remove the error signal between the user desired temperature and the actual temperature of the pipe. The other parameters will not be as critical, but they will still be useful to ensure customer specifications are met. 

Using a microcontroller has the additional benefit that the microcontroller will be able to send and receive signals to / from the system. A microcontroller would be able to receive information from the sensor's actual temperature and user's desired temperature and send information to that the system needs to adjust to match actual temp to desired temp. Implementing this control in a microcontroller reduces the need for additional components requiring additional connections to communicate with each other. 

There are less comprehensive compensators available such as phase lead and phase lag, and PID can be reduced to P and so on as needed to reduce costs if needed. However, by implementing this control using software in a microcontroller the team should not incur any additional cost by implementing PID. 


## Interface with Other Subsystems

Provide detailed information about the inputs, outputs, and data transferred to other subsystems. Ensure specificity and thoroughness, clarifying the method of communication and the nature of the data transmitted.

##### Inputs and Outputs

- **Inputs:**
  - Analog voltage signal from microcontroller specifying what power level / temperature is desired
  - Analog voltage signal from feedback thermocouple sensors specifying what actual temperature is
  - Analog voltage signal from feedback wattmeter sensors specifying what actual power consumed is

- **Outputs:**
  - PWM signal from microcontroller to tell the power subsystem to increase the duty cycle to increase power delivered or decrease duty cycle to decrease power delivered to increase / decrease temperature of part respectively.


## Buildable Schematic 

Integrate a buildable electrical schematic directly into the document. If the diagram is unreadable or improperly scaled, the supervisor will deny approval. Divide the diagram into sections if the text and components seem too small.

The schematic should be relevant to the design and provide ample details necessary for constructing the model. It must be comprehensive so that someone, with no prior knowledge of the design, can easily understand it. Each related component's value and measurement should be clearly mentioned.


## Printed Circuit Board Layout

Include a manufacturable printed circuit board layout.


## Flowchart


Shown below is a detailed flow chart of the Heat Generation Subsystem:


![alt text](<HeatGenerationSubsystem.drawio.png>)


## BOM

Provide a comprehensive list of all necessary components along with their prices and the total cost of the subsystem. This information should be presented in a tabular format, complete with the manufacturer, part number, distributor, distributor part number, quantity, price, and purchasing website URL. If the component is included in your schematic diagram, ensure inclusion of the component name on the BOM (i.e R1, C45, U4).

## Analysis

Deliver a full and relevant analysis of the design demonstrating that it should meet the constraints and accomplish the intended function. This analysis should be comprehensive and well articulated for persuasiveness.

## References

[1] E. Chen, “Controls and Signals Maslab IAP 2011.” Accessed: Oct. 27, 2025. [Online]. Available: https://web.mit.edu/6.186/2011/Lectures/controls/ControlsandSignals.pdf

[2] opitts2k, “Article 427,” The NEC Wiki, 2021. https://thenecwiki.com/2021/02/article-427/ (accessed Sep. 25, 2025).

[3] opitts2k, “Article 665,” The NEC Wiki, 2021. https://thenecwiki.com/2021/02/article-665/ (accessed Sep. 29, 2025).

[4] N. R. Rafferty and G. Tarbutton, “IEEE 844-2000: Recommended Practice for Electrical Impedance, Induction, and Skin Effect Heating of Pipelines and Vessels,” IEEE Transactions on Industry Applications, vol. 38, no. 4, pp. 921–926, Jul. 2002, doi: https://doi.org/10.1109/tia.2002.800586.  

[5] N. R. Rafferty and G. Tarbutton, “IEEE 844-2000: Recommended Practice for Electrical Impedance, Induction, and Skin Effect Heating of Pipelines and Vessels,” IEEE Transactions on Industry Applications, vol. 38, no. 4, pp. 921–926, Jul. 2002, doi: https://doi.org/10.1109/tia.2002.800586.  

[5] Phil's Lab, "PID Controller Implementation in Software," https://youtu.be/zOByx3Izf5U?si=dD22pElXplkJUA5t (accessed Nov. 04, 2025).
‌
[6] DigiKey, "How to Tune a PID Controller for an Inverted Pendulum," https://youtu.be/hRnofMxEf3Q?si=JwhMR7xFTRcJ91Fh (accessed Nov. 04, 2025).
‌
[7] MATLAB, "What Is System Identification?," https://youtu.be/Z1QS6FsxrJI?si=gIbGyuVUAKYjSpp5 (accessed Nov. 04, 2025).
‌