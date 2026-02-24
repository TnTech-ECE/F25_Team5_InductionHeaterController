# Detailed Design


## Function of the Subsystem 

The Heat Generation Subsystem is the part of the project that contols the induction heating. This subsystem ensures that customer specifications are met accurately and reliably by utilizing closed loop feedback to compare the actual temperature of the pipe to the reference temperature inferred from user-specified power inputs. This subsystem provides information critical to meeting customer specifcations such as the total temperature rise of the metal. The Heat Generation Subsystem primarily consists of the thermocouples used to measure the temperature of the pipe and the PID control logic implemented using software to reduce error and ensure consistent results. 

## Specifications and Constraints

### Specifications
The deliverables given to us by Lochinvar:
1. This controller shall have multiple power settings similar to current induction cook tops.
2. This controller shall be capable of inducing surface eddy currents and internal heating.
3. This controller shall test circular bar stock and standard black pipe thermocouple in such a manner to validate the success of the heating.
4. This controller shall measure total electrical power consumed.  
5. This controller shall measure the total temperature rise of the metal.

This subsystem is primarily concerned with meeting the customer Specifications 3 and 5, and interfacing with the other subsystems to meet Specifications 1, 2, and 4. 

1. The heat generation subsystem shall measure the initial temperature of the bar stock using a thermocouple and store this temperature using a microcontroller.
2. The heat generation subsystem shall check for over- and under- heating of the bar stock using a thermocouple.
3. The heat generation subsystem shall increase the power delivered to the induction coil if the thermocouple senses temperatures below the desired temperature output based on user specified power input.
4. The heat generation subsystem shall reduce the power delivered to the induction coil if the thermocouple senses temperatures exceeding the desired temperature output based on user specified power input.
5. The heat generation subsystem shall accurately measure the final temperature of the bar stock using thermocouple and store this temperature using a microcontroller.

### Contraints

ANSI/IEEE 844-200 [1] applies directly to induction heating for pipelines and vessels. This standard states that the electric shock hazard for induction heating is minimal, but the high temperatures and induced current flow are design considerations, maximum maintainance temperature is anything exceeding 1200°F (649°C), and magnetic isolation is required. For the heat generation subsystem, the high temperatures are the main consideration. 

1. The subsystem shall limit the temperature rise of the metal to meet customer specs. The application is primarily a water heater rather than a water boiler, so the temperature of the metal does not need to greatly exceed 212°F (100°C). Many sensor bodies are rated for temps up to 425°F (220°C), so this subsystem will set the max temp induced to be 350°F (177°C) to ensure specs are met without damaging equipment. 
2. The Embedded Subsytem utilizes a Nucleo-STM32L476RG microcontroller, so any control implemented must be compatible with that microcontroller. 
3. The Power Subsystem will be responsible for generating heat on the pipe by inducing current on the coil, and the voltage source should remain constant. So, the primary method of control will operate by changing the duty cycle of the current delivered to the coil.  


## Overview of Proposed Solution


The subsystem can be best understood using a control block diagram: 

![alt text](./Heat_Generation_Subsystem/Updated_Control_Block_Diagram.drawio.png)

For our system: 
 - R(s) = user desired temperature (based on user power input)
 - Y(s) = temperature of the pipe
 - G(s) = dynamics of the induction heating 
 - H(s) = dynamics of the thermocouple sensor
 - Gc(s) = PID compensation to implemented using microcontroller 
 - summing junction = signals to and from microcontroller

PID control [2] is compensation solution that is the most comprehensive of compensators available. PID by definition is a Proportional, Integral, and Derivative control that allows for the reduction of error, noise, and chattering of signals respectively. This is achieved primarily by adjusting the values of and ratios of constants Kp, Ki, and Kd associated with each PID parameter. 

These constants can be calculated using control theory if the system specifications are known and the transfer functions of the plant and sensor are known. For most applications, including this one, the dynamics of the system and the dynamics of the sensor are not easily determined. However, these dynamics can be approximated using experimental data and knowledge of the behavior of the systems [3]. It is not always necessary to know the exact physics of these systems since the controller / compensator can be tuned as needed to ensure the best response is attained. This can be attained using Zigler-Nichols Tuning Rules, trial and error, or similar ad hoc approaches [4].  

PID can be implemented using software or hardware, but it is typically done using software for modern applications [3]. The chosen microcontroller of the Software Subsystem is a Nucleo-STM32L476RG, so this subsystem will implement the PID control using C code to ensure compatibility. 

Typical PID control implemented using C code is of the following form according to Digikey [5]: 
![alt text](./Heat_Generation_Subsystem/Digikey_PID.png)

Where the constants are adjusted using control theory or ad hoc methods, the interval is the sampling rate of the microcontroller, and the output for this system is the PWM signal controlling the duty cycle controlling the amount of amps delivered to the coil which controls the temperature of the pipe. The PID controller recieves an error signal based on the difference between the actual temperature of the pipe and the user desired temperature of the pipe, and the controller outputs a controller signal that determines the PWM signal delivered to the power subsystem. 

An additional part of this subsystem is the implementation of thermocouple sensors. A PID controller operates by trying to reduce the error of output by as much as possible, and thus requires feedback sensors to be able to read error values. Thermocouples are used as these sensors to meet customer specs. Thermocouples operate by producing a Seebeck voltage in reponse to metals being heated, and this voltage can be measured by an ADC (analog to digital converter) to tell the controller the measured temperature of the part [6].

Thermocouple Voltage [6]: 
![alt text](./Heat_Generation_Subsystem/Thermocouple_TI_Image.png)

When an ADC is used the Nucleo operates on an analog supply voltage between 1.62 and 3.6 V [7]. This is much higher than the typical millivolt output of a thermocouple [6]. Therefore, amplification will be required for this system's thermocouples. This can be accomplished a number of ways using standard circuit components, but can be more easily done using dedicated thermocouple amplifiers. 

Adafruit produces an Adafruit Universal Thermocouple Amplifier MAX31856 Breakout Board [8,9] that solves both the issue of amplification and cold junction amplification: 

![alt text](./Heat_Generation_Subsystem/Adafruit_MAX31856.png)

There are many different thermocouples available with price mostly depending on how the thermocouple connects to the workpiece. K-type thermocouples are general purpose thermocouples and have a temperature range [6] suitable for this system's applications, with capabilities to read from –270 to 1370 °C. The critical requirement for the thermocouple chosen for this application is electromagnetic interference (EMI) shielding. This is because of the chosen coil geometry. 

Recall, the induction coil will be wrapped around the pipe: 

![alt text](<Conceptual Design/image-7.png>)

The thermocouple must be placed near or around such coil geometry in order to best measure the temperature rise of the pipe due to induction. Thus, to reduce the noise from EMI, the thermocouple used requires EMI shielding. The Omega KMQSS-062U-12 K type thermocouple has a 0.062" diameter 304 Stainless Steel (SS) sheath [10] that provides decent EMI shielding, mechanical strength, and corrosion resistance [11] that will be useful for an application involving noise from the induction coil and ensure durability from potential hazards such as heat or water: 


![alt text](./Heat_Generation_Subsystem/Omega_thermocouple_picture.png)


The implementation of the Omega KMQSS-062U-12 thermocouple and MAX31856 thermocouple amplifier will allow for accurate temperature measurements of the pipe being heated. The Nucleo will need to be able to store these temperature measurements in order to measure the total temperature rise of the metal. This can be accomplished fairly simply by writing a program to store the measured temperature of the pipe when the user selects to start the operation and to store the measured temperature of the pipe when the measured temperature is within ± 5% of the user desired temperature. 


The user desired temperature shall be determined from the user desired power input setting and common practices. Typical hot water temperatures for residential use vary between 120 °F and 160 °F [12], so the pipe should be heated to around those temperatures. However, the heating of the water would be slower than the heating of the pipe, so the pipe temperature may be increased if faster water heating is desired. For this application, temperature values between 120 °F and 160 °F provide a good baseline for desired temps. 


## Interface with Other Subsystems

This subsystem primarily consists of the thermocouple sensors, any necessary components to make the thermocouples work, and the PID controller code to control the system. 

#### 1. Power Subsystem 
- **Inputs:**
The MAX31856 thermocouple amplifier is the primary component of this subsystem requiring power. According to the datasheet the MAX31856 works best when supplied with 5 V [8] with a power consumption of less than 5 mW. It should be noted that the KMQSS-062U-12 thermocouple does not require any power input, only the amplifier. 
- **Outputs:**
The Nucleo shall send a PWM signal controlling the duty cycle controlling the amount of amps delivered to the coil which controls the temperature of the pipe. 

#### 2. Embedded Subsystem 
- **Outputs:**
The MAX31856 thermocouple amplifier shall connect to the Nucleo-STM32L476RG ADC 1 CH14 on PC4 providing an analog voltage signal.The Nucleo provides the brain act as a controller to enable the sending of a PWM signal to the power subsystem.

## Buildable Schematic 

Schematic from Adafruit for MAX31856 [8]: 
![alt text](./Heat_Generation_Subsystem/MAX31856_Schematic.png)


![alt text](./Heat_Generation_Subsystem/MAX31856_fab_print.png)




Simplified KiCAD Schematic with Nucleo Connection: 
![alt text](./Heat_Generation_Subsystem/KiCad_Heat_Gen_Schematic_Updated.png)



Pseudo Code for system implemented in the Nucleo: 
![alt text](./Heat_Generation_Subsystem/Heat_Gen_Psuedo_Code.png)


## Flowchart

#### Shown below is a detailed flow chart of the Heat Generation Subsystem's Hardware Components: 


![alt text](./Heat_Generation_Subsystem/Heat_Generation_Schematic_AGAIN.drawio.png)


#### Shown below is a detailed flow chart of how the Hardware Components read the pipe surface temperature: 


![alt text](./Heat_Generation_Subsystem/MAX31856_Flow_Chart.drawio.png)


#### Shown below is a detailed flow chart of the Heat Generation Subsystem's Software Components: 


![alt text](./Heat_Generation_Subsystem/New_HeatGenerationSubsystem.drawio.png)




where: 
- red ellipses are start and end points
- blue rectangles and squares are actions
- yellow diamonds are questions

## BOM

| Component | Manufactuer | Part Number | Distrubutor | Distributor Part Number | Quantity | Price | Purchasing Website URL |
| --------- | ----------- | ----------- | ----------- | ----------------------- | -------- | ----- | ---------------------- |
| Surface Temp Thermocouple | Omega | KMQSS-062U-12 | Digikey | 5880-KMQSS-062U-12-ND | 1 | $72.24 | [Link](https://www.digikey.com/en/products/detail/omega/KMQSS-062U-12/25632840) |
| Thermocouple Amplifier | Adafruit | 3263  | Digikey | 1528-1772-ND | 1 | $17.50 | [Link](https://www.digikey.com/en/products/detail/adafruit-industries-llc/3263/6227009) |
| Ceramic Cement | Rutland | 78 | Amazon | B000V4LTXC | 1 | $11.99 | [Link](https://www.amazon.com/Rutland-Stove-Gasket-Cement-Cartridge/dp/B000V4LTXC/ref=pd_bxgy_d_sccl_1/134-4821432-9170960?pd_rd_w=cU7Um&content-id=amzn1.sym.9bef5913-5870-4504-8883-3ba89d7f8e39&pf_rd_p=9bef5913-5870-4504-8883-3ba89d7f8e39&pf_rd_r=Y39JSRBEYQNH54AASQZK&pd_rd_wg=k0ZDf&pd_rd_r=f9976f11-6d41-48c8-9771-a0034455f62a&pd_rd_i=B000V4LTXC&th=1) |
| Total Cost | N/A | N/A | N/A | N/A | N/A | $101.73 | N/A |


The Embedded Subsystem includes the cost of the Nucleo and provides more detail about the nature of the connections between the Nucleo and the MAX31856. The Embedded Subsystem also includes the cost for shielded extension wire used for connecting the Nucleo and MAX31856 using terminal blocks.  


The Power Subsystem includees the cost of testing equipment such as a variac that will be used to experimentally obtain data. The variac will be used to increase the voltage slowly so that the team can test the response of the system in a safe manner, starting with lower voltages before increasing the voltage to minimize risk in case of system failure during testing. The math for the control theory remains the same. 


## Analysis

#### PID Closed Loop Control
Temperature systems have plant dynamics that can be approximated as a first order order system due to the dynamics of their operation. 

Typical response of a temperature system [13]: 
![alt text](./Heat_Generation_Subsystem/temperature_graph.png)

First order approximation [14]: 
![alt text](./Heat_Generation_Subsystem/timedomain.png)
![alt text](./Heat_Generation_Subsystem/firstordertransfer.png)
![alt text](./Heat_Generation_Subsystem/yss_stuff.png)

where: 

  yss = steady state output 
  A = analog input 


This model should give an approximation of the the system dynamics, but it would be necessary to experimentally collect data in order to find values of K and tau which cannot be accomplished until the coil and pipe hardware are implemented. Thus this model will serve primarily as an approximation of the expected dynamics to make the PID constants a bit easier to predict.  


It is difficult to accurately predict the exact response of the system without having an physical model of the pipe and coil. Typical system identification using the black box method requires the ability to get experimental data from the system given a known input and output [3]. An alternative method is to analytically derive the transfer function of the system using the laws of physics. This would require knowledge about the material, the resistance, the length, and the diameter of the section of the pipe heated; the number of turns of and the material of the coil; and knowledge about thermodynamics. It is difficult to determine analytically the exact temperature output in response to a certain current input. 


It is more practical and efficient to experimentally derive the plant dynamics when able, but this will not be practical for this application until after parts are ordered and when the team is able to start constructing the physical system and able to obtain experimental data. When the team is experimenting with the system the current delivered to the coil will be kept low with the ability to stop the system using an emergency stop if the temperature output is suspected of being unstable. Once this data is obtained it will be possible to analytically and experimentally determine the values for the PID constants. 


This systems PID control is implemented using C code to ensure compatibility with the Nucleo-STM32L476RG. It is possible to implement PID control using dedicated PID blocks using PLC or LabVIEW software, but these items would add unneccessary cost and compatibility issues to the system. Implementing PID control using C code is best to keep costs low and maximize compatibility with the Nucleo. A notable constraint from the Safety and Protections Subsystem is that the Heat Generation subsystem will be unable to operate if the 


#### Thermocouple Requirements Overview 
Thermocouples are used as the sensors to meet customer specs. Thermocouples operate by producing a Seebeck voltage in reponse to metals being heated, and this voltage can be measured by an ADC (analog to digital converter) to tell the controller the measured temperature of the part [6]. Thermocouple sensors themselves do not require any power to operate, but thermocouples do require power for their peripherals interpreting the data being sent. For this subsystem, the only peripheral is the thermocouple amplifier, which only consumes approximately 4.95 milliWatts [8]. 


The Nucleo-STM32L476RG has three 12 bit ADCs with 16 channels each and three SPIs. When an ADC is used the Nucleo operates on an analog supply voltage between 1.62 and 3.6 V [7]. This is much higher than the typical millivolt output of a thermocouple [6]. Therefore, amplification will be required for this system's thermocouples. This can be accomplished a number of ways using standard circuit components, but can be more easily done using dedicated thermocouple amplifiers. Dedicated thermocouple amplifiers interface easily with microcontrollers and thermocouples which would lead to faster and more efficient build times [13]. Thermocouples typically require cold junction compensation because the standard reference tables in use are designed for a reference temperature of 0˚C [15]. Cold junction compensation is often included with dedicated thermocouple amplifiers [9]. 


#### Adafruit Universal Thermocouple Amplifier MAX31856 Breakout Board
Adafruit produces an Adafruit Universal Thermocouple Amplifier MAX31856 Breakout Board [8,9] that solves both the issue of amplification and cold junction amplification: 
![alt text](./Heat_Generation_Subsystem/Adafruit_MAX31856.png)

The Adafruit Adafruit Universal Thermocouple Amplifier MAX31856 Breakout Board is able to read any type of thermococouple, including the K-type thermocouple selected for this application. Furthermore, the MAX31856 performs the amplication of the thermocouple signals. 

The MAX31856 Datsheet [8] states "The temperature conversion process consists of five steps as described in the sections below: 
1. The input amplifier and ADC amplify and digitize the thermocouple’s voltage output. 
2. The internal temperature sensor measures the cold-junction temperature. 
3. Using the internal lookup table (LUT), the ADC code corresponding to the cold junction temperature for the selected thermocouple type is determined. 
4. The thermocouple code and the cold-junction code are summed to produce the code corresponding to the cold-junction compensated thermocouple temperature. 
5. The LUT is used to produce a cold-junction compensated output code in units of °C." [8]


The output signal from the MAX31856 is then sent to the Nucleo. The Nucleo recieves a signal indicating the temperature of the surface of the pipe, as determined using the thermocouple and MAX31856, in the form of code. The Nucleo is then able to perform closed loop control using this information.  

![alt text](./Heat_Generation_Subsystem/MAX31856_Flow_Chart.drawio.png)


The MAX31856 also includes the ability to produce fault signals to ensure safety is maintained. The fault helps reduce unneccessary delays from the microcontroller trying to shut off power using code. The breakout board is set up in the basic configuration as seen in the buildable schematic, so the primary issue of integration will be connecting the thermocouple leads, the power to the amplifier, and the output to the PCB. This set up is detailed by the PCB and Embedded subsystems. 

#### Thermocouple Noise Reduction 
The critical requirement for the thermocouple chosen for this application is the reduction of noise in the sensor in order to maintaining accurate readings. Thus determining the proper thermocouple focused on noise reduction as the critical factor and focusing on other features later to isolate the exact model to implement. 


The thermocouple must be placed near or around such coil geometry in order to best measure the temperature rise of the pipe due to induction. Thus, to reduce the noise from EMI, the thermocouple used requires EMI shielding. This can be accomplished on the wire by buying mineral insulated thermocouples, or by buying shielded thermocouple extension wire. Mineral insulated thermocouples have EMI shielding by nature. The Omega KMQSS-062U-12 has a 304 Stainless Steel (SS) sheath [10] that provides decent EMI shielding and physical protections [11] that will be useful for an application involving noise from the induction coil and ensure durability from potential hazards such as heat or water. 


Thermocouples are available in grounded, ungrounded, and exposed junctions configurations. Grounded and exposed junctions have typically have faster temperature resonses, but they are more susceptible to noise due to the electrical contact [4]. Ungrounded junctions have slower temperature responses, but ungrounded junctions isolate the sensor from noise [13]. Isolation from noise is desirable due to the EMI of the coil; therefore, the thermocouple shall be ungrounded. The Omega KMQSS-062U-12 is ungrounded [10]. 


The sheath diameter can be decreased to increase response time [13], but it also reduces durability so the KMQSS-062U-12's 0.062" diameter sheath should be a good compromise as it is half the size of Omega's next largest diameter 0.125" diameter sheaths but within a few thousandths of the other sizes offered [10]. The KMQSS-062U-12's standard 12" is preferred to ensure the thermcouple can measure the length of the pipe as needed without picking up too much noise from the coil since the pipe measured is expected to be at least 1-2 feet or greater. The standard 6" provides less reach capabilities for only a couple US Dollars less. 


#### Thermocouple Mounting 
The Omega KMQSS-062U-12 thermocouple should be placed along the pipe a short distance away from the coil but not inside the coil. The electromagnetic field within the coil could induce voltage and / or heating in the thermocouple [18]. This would create unstable reading and potentially damage the thermocouple. The pipe will be heated outside the coil by conduction and allow for accurate readings. This provides about two potential locations for the thermocouple to measure, to the left or to the right of the coil. Thus, welding's permanence should not be an issue for control as the temperature of the pipe should not drastically change from one side to another. 


The Omega KMQSS-062U-12 Thermocouple will need to be mechanically attached to the pipe's surface. This can be accomplished a few different ways, such as welding or using adhesive or mechanical mounting equipment [16]. Utilizing pipe clamps to connect the thermocouple to the pipe, but these can add additional parts costs to the BOM. A pipe clamp would be more convenient to move the thermocouple compared to welding, but ideally the KMQSS-062U-12 thermocouple should not have to be moved. 


If welding is chosen, the sheath of the ungrounded thermocouple would be welded to the pipe. Capacitive discharge (CD) welding [17] is recommended for welding thermocouples to pipe [16] to minimize the amount the thermocouple is heated up. If the sheath and the pipe are welded together is chosen, the  sheath will heat up due to the conduction of heat between the sheath and the pipe. The welded connection would also ground the sheath of the thermocouple. This would require additional protective casings to prevent operators from touching the sheath's hot surface. Notably, if the sheath is heated up too much during the welding process, the thermocouple wires may become joined to the sheath and have a shorted connection to the pipe. This would destroy the thermocouple's ability to function. Welding as a service would be free using the fabrication shop at TTU's Brown building, but the possibility of destroying a thermocouple is not desirable because of wait times and cost for replacing the Omega KMQSS-062U-12 Thermocouple. 


An additional option is to drill a small hole partially into the surface of the pipe and to mount the thermocouple there using thermal rated epoxy or ceramic cements [16]. This method is simpler and cheaper than other mounting methods with less room for fabrication errors. Epoxies are typically rated for lower temperatures, around 550 °F [19] compared to ceramic cement [16], and for this induction heating application the temperature may spike to higher temperatures.


Ceramic cement is thermal insulating [20], so this will slow the thermal response of the thermocouple. To minimize the effect of this in the application it will be important to avoid applying too much cement, only enough to make the connection between the thermocouple sheath and the pipe. Ceramic cement is likely to fail due to constant shock loads and vibrations due to its brittle nature [20]. This system, an Induction Heater Controller for a water pipe should not suffer from these issues unless the system is dropped, and if the connection fails reconnection is possible using more cement. Ceramic Cement is eletrically nonconductive due to the properties of ceramic as an insulator, so the use of ceramic cement should not suffer from picking up any extra noise. Ceramic cement is available from suppliers such as Amazon for relatively cheap, such as Rutland Stove and Gasket Cement [21], which can be purchased for around $12. Rutland does not explicity state it is ceramic cement, but the chemical makeup and the reviews match with common properties of ceramic cement. This specific adhesive must sit for at least 1 hour and then have a heat cure performed within 30 days. The team can utilize scrap wood from the fabrication shop in Brown as or to make supports to keep the thermocouple straight while the ceramic cement is drying and/or during operation. 


The Omega KMQSS-062U-12 comes with a glass filled nylon connector body compatible with mating connectors and clamps sold seperately. 

![alt text](./Heat_Generation_Subsystem/omega_quick_connect.png)

This feature is not necessary for our project, so there is no need to spend additional money on these mating connectors and clamps. The quick connect body can be removed in the fabrication shop if the plugs are unable to fit in the MAX31856 breakout board's terminal blocks, as seen in the picture above [9] it is the green connectors with screws with (+) and (-) indicators. In that case, the quick connect body can simply be cut off and the thermocouple wires fed directly into terminal blocks. 

The Nucleo will need to be able to store the temperature measurements from the Omega KMQSS-062U-12 thermocouple as amplified by the MAX31856 thermocouple amplifier in order to measure the total temperature rise of the metal. This will be accomplished by writing a program to store the measured temperature of the pipe when the user selects to start the operation and to store the measured temperature of the pipe when the measured temperature is within ± 5% of the user desired temperature, which will be around 120 °F and 160 °F [12]. The Embedded Subsystem will also include a SD card act as memory to store these values, with more details on that implementation in that Subsystem's document. 

## References
[1] N. R. Rafferty and G. Tarbutton, “IEEE 844-2000: Recommended Practice for Electrical Impedance, Induction, and Skin Effect Heating of Pipelines and Vessels,” IEEE Transactions on Industry Applications, vol. 38, no. 4, pp. 921–926, Jul. 2002, doi: https://doi.org/10.1109/tia.2002.800586.  

[2] E. Chen, “Controls and Signals Maslab IAP 2011.” Accessed: Oct. 27, 2025. [Online]. Available: https://web.mit.edu/6.186/2011/Lectures/controls/ControlsandSignals.pdf

[3] MATLAB, "What Is System Identification?," https://youtu.be/Z1QS6FsxrJI?si=gIbGyuVUAKYjSpp5 (accessed Nov. 04, 2025).

[4] J. Bennett, “9.3: PID Tuning via Classical Methods,” Engineering LibreTexts, May 19, 2020. https://eng.libretexts.org/Bookshelves/Industrial_and_Systems_Engineering/Chemical_Process_Dynamics_and_Controls_(Woolf)/09%3A_Proportional-Integral-Derivative_(PID)_Control/9.03%3A_PID_Tuning_via_Classical_Methods

[5] “What is a PID Controller? | DigiKey,” www.youtube.com. https://www.youtube.com/watch?v=tFVAaUcOm4I (accessed Nov. 02, 2023).

[6] J. Wu, “A Basic Guide to Thermocouple Measurements.” Accessed: Nov. 06, 2025. [Online]. Available: https://www.ti.com/lit/an/sbaa274a/sbaa274a.pdf?ts=1762432799418&ref_url=https%253A%252F%252Fwww.google.com%252F

[7] “STM32L476RG Datasheet,” ST, 2019. Available: https://www.st.com/resource/en/datasheet/stm32l476rg.pdf

[8] “MAX31856 Datasheet.” Available: https://cdn-learn.adafruit.com/assets/assets/000/035/948/original/MAX31856.pdf
‌
[9] A. Industries, “Adafruit Universal Thermocouple Amplifier MAX31856 Breakout,” www.adafruit.com. https://www.adafruit.com/product/3263

[10] “Omega KMQSS-062U-12 Datasheet,” Omega, Accessed: Nov. 17, 2025. [Online]. Available: https://assets.omega.com/pdf/test-and-measurement-equipment/temperature/sensors/thermocouple-probes/JMQSS.pdf

[11] “What is the Best Material for EMI Shielding? 10 Top Recommendations for 2025,” www.emcnoordin.com, May 20, 2025. https://www.emcnoordin.com/what-is-the-best-material-for-emi-shielding10-top-recommendations-for-2025/ (accessed Nov. 17, 2025).

[12] State Water Heaters, "A Technical Guide to Designing Energy-Efficient Commercial Water Heater Systems," Available: https://assets.statewaterheaters.com/damroot/Original/10008/317913-000.pdf (accessed 18 Nov. 2025).

[13] D. Dlugos, “Grounded vs. Ungrounded Thermocouple Junctions: Why Select One over the Other?,” Ashcroft.com, Sep. 26, 2024. https://blog.ashcroft.com/grounded-vs-ungrounded-thermocouple-junctions (accessed Nov. 17, 2025).

[14] Alouani, Ali. ECE 3260_DAQ_Notes P.12.

[15] D. Szmulewicz, “Implementing a Thermocouple Interface With ADC12_A Application Report Implementing a Thermocouple Interface With ADC12_A,” 2011. Accessed: Nov. 12, 2025. [Online]. Available: https://www.ti.com/lit/an/slaa501/slaa501.pdf?ts=1762893695464

[16] “Correct Mounting Techniques for Surface and Immersion Thermocouples,” Instrunexus.com, 2025. https://instrunexus.com/correct-mounting-techniques-for-surface-and-immersion-thermocouples/ (accessed Nov. 24, 2025).
‌
[17] “Understanding Capacitor Discharge Welding: An In-Depth Technical Overview | Process Equipment Company Welding Services,” Peco-us.com, 2025. https://peco-us.com/understanding-capacitor-discharge-welding-an-in-depth-technical-overview/ (accessed Nov. 24, 2025).
‌
[18] “Temperature Measurement in Electromagnetic Environments,” https://www.dwyeromega.com/en-us/, 2023. https://www.dwyeromega.com/en-us/resources/temperature-measurement-in-electromagnetic-environments

[19] “HighHeat Syringe,” J-B Weld, 2026. https://www.jbweld.com/product/high-heat-syringe (accessed Feb. 09, 2026).
‌
‌[20] P. R. Mehta, “An In-Depth Analysis of Ceramic Cement: Properties and Future Directions,” Physixis, Nov. 08, 2024. https://physixis.com/articles/ceramic-cement-analysis-properties-applications/ (accessed Feb. 09, 2026).

[21] “Stove & Gasket Cement,” Rutland, 2022. https://rutland.com/products/stove-gasket-cement (accessed Feb. 09, 2026).
‌