### Embedded PWM Generation Accuracy Experiment

1. **Purpose and Justification**:

The purpose of this experiment is to evaluate the accuracy of the PWM output after it is level-shifted from 3.3V to 5V using an LM311P comparator. The goal is to ensure that the average phase error remains within 10%, which is acceptable given the LM311P’s optimal switching range of 10–50 kHz. While the PWM signal without the comparator has an error within 2%, this experiment verifies that the signal quality does not degrade beyond the 10% threshold after voltage shifting.

2. **Detailed Procedure**:

   1. Set up the embedded system as described in [Detailed Design Embedded](./Embedded_System/Detailed%20Design%20Embedded.md).
   
   ![Electro-Cookie for Voltage Scaling and MicroSD Card Reader](./Embedded_System/signal_scaling_electro_cookie.png)

   2. Connect two jumper wires to the LM311P output terminal blocks as shown in the image above.
   3. Set up a two-channel oscilloscope and connect each probe to the free end of each jumper wire.
   4. Connect a ground jumper wire to the microcontroller.
   5. Attach both oscilloscope probe ground leads to the ground wire.
   6. Power the microcontroller via micro USB or by connecting a 5V source to the E5V pin.
   7. Enable PWM by pressing 'D', then 'D' again until the cursor is on the PWM setting, then press '1' and '#'.
   8. Adjust the power level by pressing 'A' and entering the desired value on the keypad.
   9. Record the phase difference between channels 1 and 2 on the oscilloscope.
   10. For each channel, record the top voltage, minimum voltage, and frequency.
   11. Repeat steps 7–11 for each power level.

3. **Expected Results**:

   The average phase error should be less than 10%. The average minimum voltage should be 0 V, the average top voltage should be 5 V, and the average frequency should be 30 kHz.

4. **Actual Results**:

# PWM Performance Analysis Report

#### 1. Summary Data (Power Level > 0%)
| Metric                  | Value    |
| :---------------------- | :------- |
| **Average Phase Error** | -9.52%   |
| **Average Top Voltage** | 5.066 V  |
| **Average Min Voltage** | -0.716 V |

---

#### 2. Detailed Power Level Measurements

##### Power Level: 100.00%
| Phase         | Top Voltage (V) | Min Voltage (V) | Frequency (Hz) | Phase Error |
| :------------ | :-------------- | :-------------- | :------------- | :---------- |
| **PWM CH1 A** | 175.7           | 5               | -0.96          | 3.01E+04    | -2.39% |
| **PWM CH8 B** | 5.02            | -0.64           | 3.01E+04       | -           | -      |

##### Power Level: 80.00%
| Phase         | Top Voltage (V) | Min Voltage (V) | Frequency (Hz) | Phase Error |
| :------------ | :-------------- | :-------------- | :------------- | :---------- |
| **PWM CH1 A** | 132.3           | 5.1             | -0.96          | 3.01E+04    | -8.12% |
| **PWM CH8 B** | 5.01            | -0.48           | 3.01E+04       | -           | -      |

##### Power Level: 60.00%
| Phase         | Top Voltage (V) | Min Voltage (V) | Frequency (Hz) | Phase Error |
| :------------ | :-------------- | :-------------- | :------------- | :---------- |
| **PWM CH1 A** | 98.67           | 5.17            | -0.32          | 3.01E+04    | -8.64% |
| **PWM CH8 B** | 4.98            | -0.48           | 3.01E+04       | -           | -      |

##### Power Level: 40.00%
| Phase         | Top Voltage (V) | Min Voltage (V) | Frequency (Hz) | Phase Error |
| :------------ | :-------------- | :-------------- | :------------- | :---------- |
| **PWM CH1 A** | 64.97           | 5.16            | -0.96          | 3.01E+04    | -9.76% |
| **PWM CH8 B** | 5.03            | -0.76           | 3.01E+04       | -           | -      |

##### Power Level: 20.00%
| Phase         | Top Voltage (V) | Min Voltage (V) | Frequency (Hz) | Phase Error |
| :------------ | :-------------- | :-------------- | :------------- | :---------- |
| **PWM CH1 A** | 29.28           | 5.12            | -0.88          | 3.01E+04    | -18.67% |
| **PWM CH8 B** | 5.07            | -0.72           | 3.01E+04       | -           | -       |

##### Power Level: 0.00%
| Phase         | Top Voltage (V) | Min Voltage (V) | Frequency (Hz) | Phase Error |
| :------------ | :-------------- | :-------------- | :------------- | :---------- |
| **PWM CH1 A** | 0               | 0.16            | 0              | 3.01E+04    | 0.00% |
| **PWM CH8 B** | 0.24            | 0               | 3.01E+04       | -           | -     |

5. **Interpretation and Conclusions**:

   The measured phase error was 9.52%, slightly below the 10% target. This error is acceptable, though it is elevated due to the charging time of a capacitor in the LM311P comparator, which introduces additional switching delay beyond the intended dead time. Slight differences in switching times between the two comparators also affect the phase. The average top voltage was 5.066 V (1.32% error), which is reasonable and likely due to the comparator slightly boosting the 5V output. The average minimum voltage of -0.716 V is attributed to a transient spike when the comparator switches.




