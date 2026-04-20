## Executive Summary

Team 5 Induction Heater Controller is tasked by Lockinvar to design and prototype an induction coil controller in such a way to heat both the 
internal and surface level material of a metal pipe.  

## Capabilities

**Problem Statement:** Design and prototype an induction coil controller in such a way to heat both the 
internal and surface level  material of a circular heating element.

Deliverables:  
1. Similar to current induction cook tops the control will have multiple power settings. 
2. The controller will be capable of inducing surface eddy currents and internal heating. 
3. Prototype shall be testing circular bar stock and standard black pipe thermocouple in such a manner to valid 
date the success of the heating. 
4. Total electrical power is to be measured. 
5. Total temperature rise of the metal shall be measured.

## Salient Outcomes

1. The control has multiple power settings implemented using a keypad and LCD. The user interface allows the user to choose between inputting a desired a power level (manual) or inputting a desired temperature (controlled). The desired temperature is used as the setpoint for a PID. 
2. The controller is capable of inducing surface eddy currents and internal heating by adjusting a PWM signal. The PWM determines how much current is sent through the coil. 
3. The prototype has a coil of wire wrapped around a pipe and measures the surface heating of the pipe using a K-type thermocouple. The K-type thermocouple produces a voltage in the mV range that is amplified and cold-junction compensated to allows the microcontroller to determine the temperature read by the thermocouple in degrees Celcius. 
4. Total electrical power is measured based on the user input power. 
5. Total temperature rise of the metal is measured using the K-type thermocouple.

## Project Demonstration & Images




## About Us

### Team


- [Aaron Neuharth](https://tntech.joinhandshake.com/profiles/ycu78y?hscid=f5442b62-e17b-4bc6-a349-da7c3d5d34f3) (Team Leader): Electrical Engineering with a Controls Concentration. Knowledgeable in controls systems and PLC logic. Responsible for the Safety and Protections Subsystem. 
- [John Donnell](https://www.linkedin.com/in/johndonnell1848/) (Reporter): Computer Engineering Student with experience in PCB Design, debugging, and testing. Knowledgeable with coding. Responsible for the PCB Subsystem. 
- [Austin DuCrest](https://www.linkedin.com/in/austin-ducrest-aa0422271/): Electrical Engineering Student with Power Concentration. Knowledgeable in Power systems and Power system P&C (Protection and Controls). Responsible for the Power Subsystem. 
- [Cole Wilson](https://www.linkedin.com/in/cole-wilson-348271256/): Electrical Engineering Student with Mechatronics Concentration. Knowledgable about Controls. Responsible for the Heat Generation Subsystem. 
- [Dow Cox](https://www.linkedin.com/in/dwc26/): Electrical and Computer Engineering Student with Industrial Control Systems Specialization. Knowledgeable in microcontrollers and software design concepts. Responsible for the Embedded Subsystem. 

### Faculty Supervisor

- Dr. Christopher Storm Johnson

### Stakeholders


- **Lockinvar:**  
Lochinvar is the customer for this project. For over 80 years, Lochinvar, an American company, has been a leader of innovation and high-efficiency water heating. Through Lochinvar's pride in leadership and commitment to excellence, the company has continually improved year after year. Today, Lochinvar touts the broadest line of high-efficiency water heating solutions, a world-class research & development department, comprehensive service with every sale and industry-leading training through Lochinvar University.  
[Lockinvar About Us Page](https://www.lochinvar.com/en_US/about-us.html)

### Recognitions 
The team would like to thank Mr. Ben Putnam and Mr. Daniel Rushing from Lochinvar for their expertise, and Mr. Conard Murray's help troubleshooting prototyping issues. 


## Repo Organization


### [Reports](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/tree/main/Reports)

The [Project Proposal](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/main/Reports/Project%20Proposal.md) is the intial document for the project where the problem is defined and the goals for the project.

The [Conceptual Design](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/main/Reports/Conceptual%20Design.md) gives a more detailed solution to the problem as well as defining the subsystems that the project will be divided into.

The Detailed Design of the individual subsystems goes into more detail and includes relevant documentation, circuit design, and analysis for the Power, Embedded, Safety, Heat Gen, and PCB subsystems. 

The [Capstone Final Presentation](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/blob/main/Reports/Capstone%20Final%20Presentation%20%20(1).pptx) is the final presentation given at the end of Capstone 1 explaining our susbsytems. 

The Experimental Analysis shows the results of our prototype.  

The Poster is a quick summary of our project to the public. 

The Final Presentation is the presentation presented before the Industrial Advisory Board. 

### [Documentation](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/tree/main/Documentation)

The [Meeting Minutes](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/tree/main/Documentation/Meeting%20Minutes) contains a weekly progress update of the team's progress and challenges. 

### [Software](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/tree/main/Software)

The [Editing Tools](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/tree/Detailed-Design-Embedded/Software/editing_tools) was created to make organizing references in IEEE format easier. 

The [Embedded System](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/tree/Detailed-Design-Embedded/Software/embedded_system) contains the necessary initializations to make the NUCLEO-L476RG work, and the code used for the project to control the system can be found in the [src](https://github.com/TnTech-ECE/F25_Team5_InductionHeaterController/tree/Detailed-Design-Embedded/Software/embedded_system/src). 
