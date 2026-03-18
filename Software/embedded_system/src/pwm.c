#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include "pwm.h"
#include "stdint.h"
bool isPWMEnabled = false;
const float CLOCK_SPEED = 5000000;
/**
 * @param frequency
 * @param dutyCycle 0-100%
 * @param deadTime 0-100%
 * @param phaseDegrees 0-360
 */
void TIM8_Update(float frequency, float dutyCycle, float deadTime, float phaseDegrees)
{
	/* USER CODE BEGIN TIM8_Init 0 */

	/* USER CODE END TIM8_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	/* USER CODE BEGIN TIM8_Init 1 */
	unsigned period = (unsigned)((CLOCK_SPEED * 2) / frequency);
	/* USER CODE END TIM8_Init 1 */
	htim8.Instance = TIM8;
	htim8.Init.Prescaler = 0;
	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim8.Init.Period = period;
	htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim8.Init.RepetitionCounter = 0;
	htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = (unsigned)(period * dutyCycle / 100.0f);
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = (unsigned)(period * deadTime / 100.0f);
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM8_Init 2 */

	/* USER CODE END TIM8_Init 2 */
	HAL_TIM_MspPostInit(&htim8);

	unsigned arr = __HAL_TIM_GET_AUTORELOAD(&htim8); // Period ticks
	unsigned periodTicks = arr + 1U;

	unsigned dutyTicks = (uint32_t)(periodTicks * dutyCycle / 100.0f);
	unsigned phaseTicks = (uint32_t)(periodTicks * phaseDegrees / 360.0f);

	unsigned ccr1 = dutyTicks;
	unsigned ccr2 = (dutyTicks + phaseTicks) % periodTicks;

	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, ccr1);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, ccr2);
}
void TIM8_start()
{
	if (isPWMEnabled)
		return;
	isPWMEnabled = true;
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}
void TIM8_stop()
{
	if (!isPWMEnabled)
		return;
	isPWMEnabled = false;
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_4);
}
/**
 * @param frequency
 * @param dutyCycle 0-100%
 * @param deadTime 0-100%
 * @param phaseDegrees 0-360
 */
void setTIM8UpdateWithRestart(float frequency, float dutyCycle, float deadTime, float phaseDegrees)
{
	TIM8_Update(frequency, dutyCycle, deadTime, phaseDegrees);
	TIM8_stop();
	TIM8_start();
}

void updateTIM8PowerLevel(float frequency, float powerLevel)
{
	setTIM8UpdateWithRestart(frequency, 50.0f, 0.1, powerLevel * 1.8f);
	TIM8_start();
}