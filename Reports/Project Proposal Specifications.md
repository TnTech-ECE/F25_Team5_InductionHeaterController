# Project Proposal

This document provides a comprehensive explanation of what a project proposal should encompass. The content here is detailed and is intended to highlight the guiding principles rather than merely listing expectations. The sections that follow contain all the necessary information to understand the requirements for creating a project proposal.


## General Requirements for the Document
- All submissions must be composed in markdown format.
- All sources must be cited unless the information is common knowledge for the target audience.
- The document must be written in third person.
- The document must identify all stakeholders including the instuctor, supervisor, and cusotmer.
- The problem must be clearly defined using "shall" statements.
- Existing solutions or technologies that enable novel solutions must be identified.
- Success criteria must be explicitly stated.
- An estimate of required skills, costs, and time to implement the solution must be provided.
- The document must explain how the customer will benefit from the solution.
- Broader implications, including ethical considerations and responsibilities as engineers, must be explored.
- A list of references must be included.
- A statement detailing the contributions of each team member must be provided.


## Introduction - John

The introduction must be the opening section of the proposal. It acts as the "elevator pitch" of the project, briefly introducing the objective, its importance, and the proposed solution. Because readers may only read this section, it should effectively capture their attention and encourage them to read further.

Toward the end of the introduction, include a subsection that outlines what the proposal will cover. This helps set reader expectations for the ensuing sections.


## Formulating the Problem - John

Formulating the problem or objective involves clearly defining it through background information, specifications, and constraints. Think of it as "fencing in" the objective to make it unambiguously clear what is and is not being addressed and why.

Questions to consider:
- Who does the problem affect (i.e. who is your customer)?
- Why do we need this solution?
- What challenges necessitate a dedicated, multi-person engineering team?
- Why aren’t off-the-shelf solutions sufficient?

### Background - Austin

Provide context and details necessary to define the problem clearly and delineate its boundaries.

### Specifications and Constraints - Austin & Cole

Specifications and constraints define the system's requirements. They can be positive (do this) or negative (don't do that). They can be mandatory (shall or must) or optional (may). They can cover performance, accuracy, interfaces, or limitations. Regardless of their origin, they must be unambiguous and impose measurable requirements.

#### Specifications - Austin & Cole

Specifications are requirements imposed by **stakeholders** to meet their needs. If a specification seems unattainable, it is necessary to discuss and negotiate with the stakeholders.

1. This controller shall have multiple power settings similar to current induction cook tops.


2. This controller shall be capable of inducing surface eddy currents and internal heating.


3. This controller shall test circular bar stock and standard black pipe thermocouple in such a manner to validate the success of the heating.


4. This controller shall measure total electrical power consumed.  


5. This controller shall measure the total temperature rise of the metal.



#### Constraints - Austin & Cole

Constraints often stem from governing bodies, standards organizations, and broader considerations beyond the requirements set by stakeholders.

Questions to consider:
- Do governing bodies regulate the solution in any way?
- Are there industrial standards that need to be considered and followed?
- What impact will the engineering, manufacturing, or final product have on public health, safety, and welfare?
- Are there global, cultural, social, environmental, or economic factors that must be considered?

NEC Article 427 [4] Part V covers the codes regarding Induction Heating, and Part VII covers the Controls and Protection. Notably if the controller contains an off switch it should not be able to enerize automatically while in the off position, and selecting off should open all ungrounded conductors.


## Survey of Existing Solutions - Cole

Research existing solutions, whether in literature, on the market, or within the industry. Present these findings in a coherent, organized manner. Remember to cite all information that is not common knowledge.

Lochinvar already has many industrial solutions for induction heater controllers. Lochinvar's boilers and water heaters often contain a Smart Touch Control [1] that allow the user to control the heater's setpoint and many other parameters. Lochinvar also sells a complete control panel [2] available for purchase for about $5300.

You can also find simple controllers sold by companies, such as Automation Direct's SOLO Basic Temperature Controller [3] for much cheaper, closer to $52 or $150. However, these controllers would still need to be interfaced with additional hardware to actually control the induction heating. Likewise the 100 ms sampling rate [3] may be too slow to properly control the induction heating if there is a sudden spike in temperature. In induction heating, temperature spikes may rise thousands of degress in fractions of a second.


## Measures of Success - Aaron

Define how the project’s success will be measured. This involves explaining the experiments and methodologies to verify that the system meets its specifications and constraints.


## Resources - Aaron

Each project proposal must include a comprehensive description of the necessary resources.

### Budget - Aaron

Provide a budget proposal with justifications for expenses such as software, equipment, components, testing machinery, and prototyping costs. This should be an estimate, not a detailed bill of materials.

The budget of the project will need to not only meet the expenses required for any components required for the induction controller, but also take into account unforseen expsenses. Below is a table that shows a list of components needed as well as an estimated cost for each listed.
| Components   | Estimated Cost |
| ------------ | -------------- |
| Row 1, Col 1 | Row 1, Col 2   |
| Row 2, Col 1 | Row 2, Col 2   |
| Row 2, Col 1 | Row 2, Col 2   |

### Personnel - Aaron

The team has a strong foundation in analyzing electrical circuits and programming. However, to meet the goal of designing an induction coil controller to heat a circular heating element, our team will need to possess the following additional skills:

 - AutoCAD Electrical
 - C++
 - Circuit Design
 - PCB Design
 - Control System Design
 - Power System Design

Additionaly, the team will need to have a fundamental understanding of key topics including:
 - Data Analysis from Measurements
 - Material Response to Induction Heating

Through the use of these skills and topics, the team will be able to successfully navigate the design and prototyping of an induction heater controller.

Daniel Rushing, an electrical engineer from Lochinvar, has stepped in as our supervisor over this project. Rushing will provide guidance and direction for the project.

The team has asked Dr. Ali Alouani to assist in analyzing the controls for an induction coil as he has extensive knowledge of controls and sensor analysis.

### Timeline - Aaron

Provide a detailed timeline, including all major deadlines and tasks. This should be illustrated with a professional Gantt chart.


## Specific Implications - Dow

Explain the implications of solving the problem for the customer. After reading this section, the reader should understand the tangible benefits and the worthiness of the proposed work.

The specific implications of solving this problem is faster heating, more precise temperature control, better efficiency, and improved safety. Induction heating is faster as it heats directly within the object as opposed to indirectly with conventional resistance-based heating methods. Since the heating is faster, the temperature of the object can be controlled more precisely. Also, induction heating has more control over how deep and which parts of the material are heated. The energy efficiency is improved as a majority of the energy is directly entering the material from the electric current from the electromagnetic fields from the induction coils. With traditional electric heating, the resistive elements blast a lot more heat energy into the air, as not all the energy is conductively transferred to the material being heated. Induction is safer as only the material heats up, not the coils, unlike regular electric heating.


## Broader Implications, Ethics, and Responsibility as Engineers - Dow

Consider the project’s broader impacts in global, economic, environmental, and societal contexts. Identify potential negative impacts and propose mitigation strategies. Detail the ethical considerations and responsibilities each team member bears as an engineer.

The broader impacts of this problem are creating a cheaper option for controlling industrial induction heating applications, which, through the team's success, the industry will hopefully gain a cost-effective option. The ethical considerations and responsibilities of solving this problem are notifying the user that it is heating the object, the cost of the controller compared to other heating options, proprietary objects, making sure it operates within intended temperatures, and does not use more power than the specified recommended amount. With an induction cooktop, users report that they have a hard time telling that the stove is heating a pot, as without some notification or signal, there is no indication that the cooktop is working. When the team designs the controller, telling the user that it is on with an LED light or message will be crucial for the heater to be safe. Checking a heater by touching it is dangerous. Induction heating can be more expensive than traditional heating methods, but the advantages explained above in the Specific Implications outweigh the increased cost. The team will have to make sure the heater does not overheat the object, as that could damage it by being heated past its melting point, or make sure it does not surpass the object's temperature differential maximum by heating the material too quickly. The heater has to be controlled without surpassing the amperage limit of the circuit the heater is wired to. The Team will need to make the temperature range, maximum power draw, and temperature differential configurable, making this controller more versatile and allowing the heater to operate safely. 


## References - Everyone

All sources used in the project proposal that are not common knowledge must be cited. Multiple references are required.

[1]“Smart Touch Control | Lochinvar,” Lochinvar.com, 2025. [https://www.lochinvar.com/en_US/lochinvaru-video-tutorials/smart-touch-control.html](https://www.lochinvar.com/en_US/lochinvaru-video-tutorials/smart-touch-control.html) (accessed Sep. 24, 2025).

[2]“Lochinvar 100164434 Complete Control Panel,” NorthStock, 2022. [https://northstock.com/lochinvar-100164434-complete-control-panel/](https://northstock.com/lochinvar-100164434-complete-control-panel/) (accessed Sep. 24, 2025).

[3]“SLB4848 Series Overview.” Accessed: Sep. 25, 2025. [Online]. Available: https://cdn.automationdirect.com/static/specs/solobasic4848.pdf

[4]opitts2k, “Article 427,” The NEC Wiki, 2021. https://thenecwiki.com/2021/02/article-427/ (accessed Sep. 25, 2025).
‌
[5] S. Salkowski, “Hot topic: Dispelling the myths about induction stoves,” Acadia Center, [https://acadiacenter.org/hot-topic-dispelling-the-myths-about-induction-stoves/?gad_source=1&gad_campaignid=22858216787&gbraid=0AAAAADcED4I34TheDlrXP_tno7EhyL51z&gclid=Cj0KCQjw0NPGBhCDARIsAGAzpp32uNfoZ_91Cf5AoCcbgiRRpbQHGZyFujKQ54woGBVXDbM_sppvIYEaAhEvEALw_wcB](https://acadiacenter.org/hot-topic-dispelling-the-myths-about-induction-stoves/?gad_source=1&gad_campaignid=22858216787&gbraid=0AAAAADcED4I34TheDlrXP_tno7EhyL51z&gclid=Cj0KCQjw0NPGBhCDARIsAGAzpp32uNfoZ_91Cf5AoCcbgiRRpbQHGZyFujKQ54woGBVXDbM_sppvIYEaAhEvEALw_wcB) (accessed Sep. 25, 2025).
‌
‌[6] P. Hope, A. St. John                                                                                                                                        Senior Home & Tech Writer, and D. Yao, “Pros and cons of induction cooktops and ranges,” Consumer Reports, [https://www.consumerreports.org/appliances/ranges/pros-and-cons-of-induction-cooktops-and-ranges-a5854942923/](https://www.consumerreports.org/appliances/ranges/pros-and-cons-of-induction-cooktops-and-ranges-a5854942923/) (accessed Sep. 25, 2025).

## Statement of Contributions - Everyone

Each team member must contribute meaningfully to the project proposal. In this section, each team member is required to document their individual contributions to the report. One team member may not record another member's contributions on their behalf. By submitting, the team certifies that each member's statement of contributions is accurate.

Dow Cox - Specific Implications and Broader Implications, Ethics, and Responsibility as Engineers. Reference [5] and [6]
