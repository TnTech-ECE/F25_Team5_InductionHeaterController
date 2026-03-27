/*
        Authors: Cole Wilson and Dow Cox
        Note: PID control probably overkill for this system, probably mainly use P, but ID available if needed
*/
#include "max31856.h"
#include "stdint.h"
#include "run.h"
#include "delay.h"
#include "pwm.h"
float PID_Controller();

// variables
float actual_temp, user_desired_temp, max_temp, min_temp, error;
float integral, derivative, control_signal_output, power_output_percentage;

// initialize variables
float prev_error = 0;
float integral = 0;
float error = 0;
float derivative = 0;
float max_temp = 100; // boiling point of water, degrees in celcius

#define SAMPLING_RATE 100 // milliseconds

float lastPowerLevel = 0;
void PID_Callback()
{
        float powerLevel = PID_Controller();
        if (lastPowerLevel == powerLevel)
                return;
        lastPowerLevel = powerLevel;

        updateTIM1_8_PowerLevel(controllerData.frequency, powerLevel);
        if (!isPWMEnabled)
                return;
        if (powerLevel <= 0.01)
                TIM1_8_stop();
        else
                TIM1_8_start();
}
int runId;
void run_PID()
{
        if (runId > -1)
                return;
        runId = runInterval(PID_Callback, SAMPLING_RATE); // 100 millisecond sampling rate
}
void clearRun_PID()
{
        if (runId > -1)
                clearRun(runId); // 100 millisecond sampling rate
        runId = -1;
}
float PID_Controller()
{
        // get value from amplified thermocouple signal
        // same function from lcd_ui.c
        float actual_temp = max31856_read_TC_temp(&thermoSPI2);
        if (thermoSPI2.sr.val)
        {
                power_output_percentage = 0;
                goto returnPosition;
        }

        // protections
        if (actual_temp > max_temp)
                control_signal_output = 0; // give pipe time to cool off, deliver less power

        // get value from user what temp is desired
        user_desired_temp = controllerData.desiredTemperature;

        // calculate PID terms
        error = user_desired_temp - actual_temp;                                                                                 // for proportional control
        integral = integral + (error * (SAMPLING_RATE / 1000));                                                                  // for integral control
        derivative = (error - prev_error) / SAMPLING_RATE;                                                                       // for derivative control
        control_signal_output = (controllerData.kp * error) + (controllerData.ki * integral) + (controllerData.kd * derivative); // value in temperature

        // convert temp to power percentage integer
        // temp should be 0 to 120 (or whatever max temp)

        power_output_percentage = control_signal_output; /// max_temp;

        // if error is negative (actual temp > user wants) pipe needs to cool, turn off coil

        // save error for next iteration if needed for derivative control
        prev_error = error;
returnPosition:
        return power_output_percentage;
}
