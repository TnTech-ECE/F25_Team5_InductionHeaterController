#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include "pwm.h"
#include "stdint.h"
#include "math.h"
bool isPWMEnabled = false;
bool isPWMStarted = false;
const float CLOCK_SPEED = 5000000;
/**
 * @param frequency
 * @param dutyCycle 0-100%
 * @param deadTime 0-100%
 * @param phaseDegrees 0-360
 */
void TIM1_8_Update(float frequency, float dutyCycle, float deadTime, float phaseDegrees)
{

	if (frequency < 1.0f)
		frequency = 1.0f;

	if (dutyCycle < 0.0f)
		dutyCycle = 0.0f;
	if (dutyCycle > 100.0f)
		dutyCycle = 100.0f;

	if (deadTime > 255.0f)
		deadTime = 255.0f;
	if (deadTime < 0.0f)
		deadTime = 0.0f;

	while (phaseDegrees < 0.0f)
		phaseDegrees += 360.0f;
	while (phaseDegrees >= 360.0f)
		phaseDegrees -= 360.0f;

	MODIFY_REG(htim1.Instance->BDTR, TIM_BDTR_DTG, (unsigned)deadTime);
	unsigned periodTicks = (uint32_t)((CLOCK_SPEED * 2.0f) / frequency);
	if (periodTicks < 2U)
		periodTicks = 2U;

	unsigned arr = periodTicks - 1U;
	unsigned ccr = (unsigned)((periodTicks * dutyCycle) / 100.0f);
	if (ccr > arr)
		ccr = arr;

	unsigned phaseTicks = (unsigned)((periodTicks * phaseDegrees) / 360.0f) % periodTicks;

	// Stop counters before changing base timing and phase
	__HAL_TIM_DISABLE(&htim1);
	__HAL_TIM_DISABLE(&htim8);

	// Keep both timers at identical frequency
	__HAL_TIM_SET_AUTORELOAD(&htim1, arr);
	__HAL_TIM_SET_AUTORELOAD(&htim8, arr);

	// Same duty on both outputs
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ccr);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, ccr);

	// Phase offset: TIM1 starts at 0, TIM8 starts at phaseTicks
	__HAL_TIM_SET_COUNTER(&htim1, 0U);
	__HAL_TIM_SET_COUNTER(&htim8, phaseTicks);

	// Generate update so preloaded ARR/CCR latch immediately
	// HAL_TIM_GenerateEvent(&htim1, TIM_EVENTSOURCE_UPDATE);
	// HAL_TIM_GenerateEvent(&htim8, TIM_EVENTSOURCE_UPDATE);

	__HAL_TIM_ENABLE(&htim8);
	__HAL_TIM_ENABLE(&htim1);
}
void TIM1_8_Disable()
{
	isPWMEnabled = false;
	TIM1_8_stop();
}
void TIM1_8_Enable()
{
	isPWMEnabled = true;
}
void TIM1_8_start()
{
	if (!isPWMEnabled)
		return;
	if (isPWMStarted)
		return;
	isPWMStarted = true;
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}
void TIM1_8_stop()
{
	if (!isPWMStarted)
		return;
	isPWMStarted = false;
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim8, TIM_CHANNEL_3);
}
// /**
//  * @param frequency
//  * @param dutyCycle 0-100%
//  * @param deadTime 0-100%
//  * @param phaseDegrees 0-360
//  */
// void setTIM1_8_UpdateWithRestart(float frequency, float dutyCycle, float deadTime, float phaseDegrees)
// {
// 	TIM8_Update(frequency, dutyCycle, deadTime, phaseDegrees);
// 	TIM8_stop();
// 	TIM8_start();
// }

void updateTIM1_8_PowerLevel(float frequency, float powerLevel)
{

	TIM1_8_Update(frequency, 50.0f, 0.1, fmax(fmin(powerLevel * 1.8f, 100.0f), 0.0f));
}