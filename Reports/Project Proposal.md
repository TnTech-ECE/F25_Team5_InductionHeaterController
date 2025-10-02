# Project Proposal

## Introduction - John

The introduction must be the opening section of the proposal. It acts as the "elevator pitch" of the project, briefly introducing the objective, its importance, and the proposed solution. Because readers may only read this section, it should effectively capture their attention and encourage them to read further. something something something 

Toward the end of the introduction, include a subsection that outlines what the proposal will cover. This helps set reader expectations for the ensuing sections.


## Formulating the Problem - John

Formulating the problem or objective involves clearly defining it through background information, specifications, and constraints. Think of it as "fencing in" the objective to make it unambiguously clear what is and is not being addressed and why.

Questions to consider:
- Who does the problem affect (i.e. who is your customer)?
- Why do we need this solution?
- What challenges necessitate a dedicated, multi-person engineering team?
- Why aren’t off-the-shelf solutions sufficient?

### Background

Induction heating is a non-contact method of transferring energy into conductive materials through electromagnetic induction. When a time-varying magnetic field surrounds a conductive body, eddy currents are induced within the material. These currents, along with losses due to magnetic hysteresis in ferromagnetic materials, generate heat. The technique is valued for its efficiency, cleanliness, and ability to deliver rapid and localized heating. 

In applications where uniform heating is critical, focusing exclusively on surface-level heating can lead to problems such as overheating of the outer regions while the core lags behind in temperature. Conversely, targeting only internal heating may cause surface underheating or delays in reaching the desired surface temperature. This imbalance can reduce efficiency, compromise material performance, or, in consumer contexts, lead to poor user experience. For a circular heating element, in particular, the ability to regulate both surface and core heating ensures even temperature distribution, faster thermal response, and improved overall performance.

The key to addressing this challenge lies in the design of the induction heater’s controller. By enabling real-time adjustment of inverter frequency and output power, the controller can shift the balance between surface-level and internal heating according to operational needs. Furthermore, modern control strategies may integrate feedback from temperature sensors or current sensors, allowing closed-loop operation that adapts dynamically to the heating profile. Such an approach has the potential to improve efficiency, enhance safety, and expand the flexibility of induction heating systems in both industrial and consumer applications. 


### Specifications and Constraints

#### Specifications

Based on the deliverables given to us by Lochinvar:

1. This controller shall have multiple power settings similar to current induction cook tops.


2. This controller shall be capable of inducing surface eddy currents and internal heating.


3. This controller shall test circular bar stock and standard black pipe thermocouple in such a manner to validate the success of the heating.


4. This controller shall measure total electrical power consumed.  


5. This controller shall measure the total temperature rise of the metal.


#### Constraints


NEC Article 427 [4] covers Fixed Electric Heating Equipment for Pipelines and Vessels. Part V covers the codes regarding Induction Heating, and Part VII covers the Controls and Protection. Notably if the controller contains an off switch it should not be able to enerize automatically while in the off position, and selecting off should open all ungrounded conductors.


NEC Article 665 [7] covers Induction and Dielectric Heating Equipment. Part I has guidelines "the current flow from the output circuit to ground under operating and ground-fault conditions shall be limited to a value that does not cause 50 volts or more to ground to appear on any accessible part of the heating equipment and its load." Part II states the control panel should be dead front construction, that is: no live circuits at risk of shocking people. 


ANSI/IEEE 844-200 [8] applies directly to induction heating for pipelines and vessels. It states that the elctric shock hazard for induction heating is minimal, but the high temperatures and induced current flow are design considerations [8]. There are a couple other IEEE standards related to heating such as ANSI/IEEE 515-2011 with standards for industrial electrical trace heating applications and ANSI/NECA 202-2013 with standards for installing and maintaining industrial heat tracing systems [4]. 



## Survey of Existing Solutions 

Our team is supplied with a SK-IH18G23T induction cooker with a cost of about $75. It comes with some basic controls already including: on/off button, a minus and a plus button for adjusting time, temperature, or power, a keep warm button, and a lock button. Likewise the cooker has check points for voltage, surface temperature, and some indicators for if the IGBT sensor is working. These built in existing solutions work great for many of the specifications, but the cooker will still need adjustment to be able to heat a circular bar stock and standard black pipe thermocouple. Measuring the total temperature rise of the metal may also be more complicated than simply measuring the surface temperature. 


There are simple controllers available for purchase, such as Automation Direct's SOLO Basic Temperature Controller [3] close to $52 or $150. However, these controllers would still need to be interfaced with additional hardware to actually control the induction heating. Likewise the 100 ms sampling rate [3] may be too slow to properly control the induction heating if there is a sudden spike in temperature. In induction heating, temperature spikes may rise thousands of degress in fractions of a second.


Lochinvar has many industrial solutions for induction heater controllers. Lochinvar's boilers and water heaters often contain a Smart Touch Control [1] that allow the user to control the heater's setpoint and many other parameters. Lochinvar also sells a complete control panel [2] available for purchase for about $5300. These solutions would meet and exceed our required specifications, but they also exceed our budget by thousands of dollars. 


## Measures of Success

The project is a success if it meets each of the following deliverables assigned by Lochinvar:

1. The induction controller shall have multiple power settings similar to current induction cook tops (ie. 0-460F, 0-1800W)


2. The induction controller shall be capable of inducing surface eddy currents and internal heating.


3. The controller shall heat a circular bar stock and a standard black pipe thermocouple in such a manner to validate the success of the heating.


4. Total electrical power consumed shall be measured.  


5. Total temperature rise of the heated metal shall be measured.

Once these deliverables are met, the project has succeeded. Any further benefits outside of the deliverables from the new induction heater controller shall be counted as a bonus.

## Resources
For this project to be successful, an estimated budget and allocation of personnel is needed to ensure adequate preparations are made in a timely manner. Below, the budget and personnel is shown.

### Budget

The budget of the project will need to not only meet the expenses required for any components required for the induction controller, but also take into account unforseen expsenses. Below is a table that shows a list of components needed as well as an estimated cost for each listed.
| Components      | Estimated Cost |      Link      |
| ------------    | -------------- | -------------- |
| Litz Wire       |       $30      |       N/A      |
| PCB             |       $50      |       N/A      |
| Display         |       $10      |      [Link](https://www.digikey.com/en/products/detail/display-visions/EA-DOGM132L-5/4896710)              |
| Microcontroller |       $25      |      [Link](https://www.bestmodulescorp.com/en/ht45f0059-16nsop.html)       |
| Encasing        |       $70      |       N/A      |

This brings the total estimated budget for the project to be $185

### Personnel

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

### Timeline

![alt text](image-3.png)
![alt text](image-4.png)
![alt text](image-5.png)

## Specific Implications

The specific implications of solving this problem is faster heating, more precise temperature control, better efficiency, and improved safety. Induction heating is faster as it heats directly within the object as opposed to indirectly with conventional resistance-based heating methods. Since the heating is faster, the temperature of the object can be controlled more precisely. Also, induction heating has more control over how deep and which parts of the material are heated. The energy efficiency is improved as a majority of the energy is directly entering the material from the electric current from the electromagnetic fields from the induction coils. With traditional electric heating, the resistive elements blast a lot more heat energy into the air, as not all the energy is conductively transferred to the material being heated. Induction is safer as only the material heats up, not the coils, unlike regular electric heating.


## Broader Implications, Ethics, and Responsibility as Engineers


The broader impacts of this problem include creating a cheaper option for controlling industrial induction heating applications, which, through the team's success, the industry will hopefully gain a cost-effective option. The ethical considerations and responsibilities of solving this problem are notifying the user that it is heating the object, the cost of the controller compared to other heating options, making sure it operates within intended temperatures, and does not use more power than the specified recommended amount. With an induction cooktop, users report that they have a hard time telling that the stove is heating a pot, as without some notification or signal, there is no indication that the cooktop is working. When the team designs the controller, telling the user that it is on with an LED light or message will be crucial for the heater to be safe. Checking a heater by touching it is dangerous. 

Although induction heating can be more expensive than traditional heating methods, the advantages explained above in the Specific Implications outweigh the increased cost. The team will have to make sure the heater does not overheat the object, as that could damage it by being heated past its melting point. Along with this, the controller must make sure it does not surpass the object's temperature differential maximum by heating the material too quickly. The heater has to be controlled without surpassing the amperage limit of the circuit the heater is wired to. Finally, the team will need to make the temperature range, maximum power draw, and temperature differential all configurable, making this controller more versatile and allowing the heater to operate more efficiently. 


## References

[1]“Smart Touch Control | Lochinvar,” Lochinvar.com, 2025. [https://www.lochinvar.com/en_US/lochinvaru-video-tutorials/smart-touch-control.html](https://www.lochinvar.com/en_US/lochinvaru-video-tutorials/smart-touch-control.html) (accessed Sep. 24, 2025).

[2]“Lochinvar 100164434 Complete Control Panel,” NorthStock, 2022. [https://northstock.com/lochinvar-100164434-complete-control-panel/](https://northstock.com/lochinvar-100164434-complete-control-panel/) (accessed Sep. 24, 2025).

[3]“SLB4848 Series Overview.” Accessed: Sep. 25, 2025. [Online]. Available: https://cdn.automationdirect.com/static/specs/solobasic4848.pdf

[4]opitts2k, “Article 427,” The NEC Wiki, 2021. https://thenecwiki.com/2021/02/article-427/ (accessed Sep. 25, 2025).
‌
[5] S. Salkowski, “Hot topic: Dispelling the myths about induction stoves,” Acadia Center, [https://acadiacenter.org/hot-topic-dispelling-the-myths-about-induction-stoves/?gad_source=1&gad_campaignid=22858216787&gbraid=0AAAAADcED4I34TheDlrXP_tno7EhyL51z&gclid=Cj0KCQjw0NPGBhCDARIsAGAzpp32uNfoZ_91Cf5AoCcbgiRRpbQHGZyFujKQ54woGBVXDbM_sppvIYEaAhEvEALw_wcB](https://acadiacenter.org/hot-topic-dispelling-the-myths-about-induction-stoves/?gad_source=1&gad_campaignid=22858216787&gbraid=0AAAAADcED4I34TheDlrXP_tno7EhyL51z&gclid=Cj0KCQjw0NPGBhCDARIsAGAzpp32uNfoZ_91Cf5AoCcbgiRRpbQHGZyFujKQ54woGBVXDbM_sppvIYEaAhEvEALw_wcB) (accessed Sep. 25, 2025).
‌
‌[6] P. Hope, A. St. John                                                                                                                                        Senior Home & Tech Writer, and D. Yao, “Pros and cons of induction cooktops and ranges,” Consumer Reports, [https://www.consumerreports.org/appliances/ranges/pros-and-cons-of-induction-cooktops-and-ranges-a5854942923/](https://www.consumerreports.org/appliances/ranges/pros-and-cons-of-induction-cooktops-and-ranges-a5854942923/) (accessed Sep. 25, 2025).

[7] opitts2k, “Article 665,” The NEC Wiki, 2021. https://thenecwiki.com/2021/02/article-665/ (accessed Sep. 29, 2025).

[8]N. R. Rafferty and G. Tarbutton, “IEEE 844-2000: Recommended Practice for Electrical Impedance, Induction, and Skin Effect Heating of Pipelines and Vessels,” IEEE Transactions on Industry Applications, vol. 38, no. 4, pp. 921–926, Jul. 2002, doi: https://doi.org/10.1109/tia.2002.800586.
‌
‌

## Statement of Contributions
Aaron Neuharth - Measures of Success, Resources, Budget, Timeline, Personnel

Dow Cox - Specific Implications and Broader Implications, Ethics, and Responsibility as Engineers. Reference [5] and [6]

Austin DuCrest - Background, Survey of Existing Solutions, Measures of success


