## Shift References
#### What the software does
Shifts References to splice in a new reference in IEEE format.
#### All dependencies
 - node.js
 - fs
 - path
#### How to Install
 1. Install node.js [https://nodejs.org/en/download](https://nodejs.org/en/download).
 2. Open `./Software/editing_tools` in a terminal.
 3. To install dependencies run: 
```bash
npm i
```
#### How to run/How to use
 1. In references add a reference with [<number>#] ie [23#]. <number> will be at the reference you want it to be and the program will shift it to splice in that number.
 2. Run:
```bash
npm run shift -- "relative path from editing_tools or static path"
```
 3. Your shifted references file will be in out/ within the same folder as this file is in with the same name has the file you told it to fix.
#### Notes
with multiple references make sure you use [number#] in the file above \#\# references and do not have duplicate references. ie [29] and a different [29] or [29#] and a different [29#]. The order is how the numbers are replaced by. So, having [100#] does not really matter it will become the number which it is ordered at.

#### Example

###### Meat Paragraphs
The key to addressing this challenge lies in the design of the induction heater’s controller. By enabling real-time adjustment of inverter frequency and output power, the controller can shift the balance between surface-level and internal heating depending on operational needs [1]. Modern inverter designs use resonant tracking, frequency modulation, and energy-dosing strategies to maintain efficiency while adapting penetration depth [2],[3#].`[number#] means that it will be a new reference to shift everything` Furthermore, modern control strategies integrate feedback from sensors—ranging from infrared temperature sensors to magnetic-field or inductive sensors that respond to changes near the Curie point—allowing closed-loop operation that dynamically adapts to the heating profile [2763#]. Such adaptive approaches improve energy efficiency, enhance safety, and expand the flexibility of induction heating systems in both industrial and consumer applications [4].

###### References
[1] “Thermal Analysis of Electromagnetic Induction Heating for Cylinder-Shaped Objects.” Electrophoresis, 2025, https://pubmed.ncbi.nlm.nih.gov/39831469/. Accessed 3 Oct. 2025.

[2] Jankowski, T. A., et al. “Approximate Analytical Solution for Induction Heating of Solid Cylinders.” Applied Mathematical Modelling, 2015, https://www.osti.gov/pages/biblio/1225114. Accessed 3 Oct. 2025.
`Order the references within references by their order of reference`
[3#] “State of the Art and Future Trends in Monitoring for Induction Surface Hardening.” Electronics, 2024, https://www.mdpi.com/2079-9292/13/13/2591. Accessed 3 Oct. 2025.

`Number doesn't matter will take the number of order it is listed by`
[2763#] Xia, Z., et al. “Design and Temperature Uniformity Optimization of Three Circular Coil Patterns.” Energies, 2025, https://www.mdpi.com/1996-1073/18/11/2695. Accessed 3 Oct. 2025.
`Make sure that you don't have duplicate things between brackets like like [40#] which references two different thing`
[3] Areitioaurtena, M., et al. “A Semi-Analytical Coupled Simulation Approach for Induction Heating.” AMSES Journal, 2021, https://amses-journal.springeropen.com/articles/10.1186/s40323-021-00199-0. Accessed 3 Oct. 2025.
.

##### This will process to

###### Meat Paragraphs
The key to addressing this challenge lies in the design of the induction heater’s controller. By enabling real-time adjustment of inverter frequency and output power, the controller can shift the balance between surface-level and internal heating depending on operational needs [1]. Modern inverter designs use resonant tracking, frequency modulation, and energy-dosing strategies to maintain efficiency while adapting penetration depth [2],[3]. Furthermore, modern control strategies integrate feedback from sensors—ranging from infrared temperature sensors to magnetic-field or inductive sensors that respond to changes near the Curie point—allowing closed-loop operation that dynamically adapts to the heating profile [4]. Such adaptive approaches improve energy efficiency, enhance safety, and expand the flexibility of induction heating systems in both industrial and consumer applications [5].

###### References
[1] “Thermal Analysis of Electromagnetic Induction Heating for Cylinder-Shaped Objects.” Electrophoresis, 2025, https://pubmed.ncbi.nlm.nih.gov/39831469/. Accessed 3 Oct. 2025.

[2] Jankowski, T. A., et al. “Approximate Analytical Solution for Induction Heating of Solid Cylinders.” Applied Mathematical Modelling, 2015, https://www.osti.gov/pages/biblio/1225114. Accessed 3 Oct. 2025.

[3] “State of the Art and Future Trends in Monitoring for Induction Surface Hardening.” Electronics, 2024, https://www.mdpi.com/2079-9292/13/13/2591. Accessed 3 Oct. 2025.

[4] Xia, Z., et al. “Design and Temperature Uniformity Optimization of Three Circular Coil Patterns.” Energies, 2025, https://www.mdpi.com/1996-1073/18/11/2695. Accessed 3 Oct. 2025.

[5] Areitioaurtena, M., et al. “A Semi-Analytical Coupled Simulation Approach for Induction Heating.” AMSES Journal, 2021, https://amses-journal.springeropen.com/articles/10.1186/s40323-021-00199-0. Accessed 3 Oct. 2025.