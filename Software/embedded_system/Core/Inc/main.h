/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
  extern SPI_HandleTypeDef hspi1;
  extern SPI_HandleTypeDef hspi2;
  extern SPI_HandleTypeDef hspi3;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
  TIM_TypeDef getTimer3Instance();
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define D7_Pin GPIO_PIN_0
#define D7_GPIO_Port GPIOC
#define D6_Pin GPIO_PIN_1
#define D6_GPIO_Port GPIOC
#define flow_sensor_Pin GPIO_PIN_3
#define flow_sensor_GPIO_Port GPIOC
#define flow_sensor_EXTI_IRQn EXTI3_IRQn
#define RS_Pin GPIO_PIN_0
#define RS_GPIO_Port GPIOA
#define EN_Pin GPIO_PIN_1
#define EN_GPIO_Port GPIOA
#define COL2_Pin GPIO_PIN_2
#define COL2_GPIO_Port GPIOA
#define COL3_Pin GPIO_PIN_3
#define COL3_GPIO_Port GPIOA
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_Port GPIOA
#define D5_Pin GPIO_PIN_0
#define D5_GPIO_Port GPIOB
#define ROW1_Pin GPIO_PIN_10
#define ROW1_GPIO_Port GPIOB
#define ROW1_EXTI_IRQn EXTI15_10_IRQn
#define spi_cn2_Pin GPIO_PIN_12
#define spi_cn2_GPIO_Port GPIOB
#define ROW0_Pin GPIO_PIN_8
#define ROW0_GPIO_Port GPIOA
#define ROW0_EXTI_IRQn EXTI9_5_IRQn
#define COL1_Pin GPIO_PIN_10
#define COL1_GPIO_Port GPIOA
#define pipe_temp_fault_Pin GPIO_PIN_11
#define pipe_temp_fault_GPIO_Port GPIOA
#define pipe_temp_fault_EXTI_IRQn EXTI15_10_IRQn
#define igbt_temp_fault_Pin GPIO_PIN_12
#define igbt_temp_fault_GPIO_Port GPIOA
#define igbt_temp_fault_EXTI_IRQn EXTI15_10_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define current_fault_Pin GPIO_PIN_15
#define current_fault_GPIO_Port GPIOA
#define current_fault_EXTI_IRQn EXTI15_10_IRQn
#define spi_cn3_Pin GPIO_PIN_2
#define spi_cn3_GPIO_Port GPIOD
#define COL0_Pin GPIO_PIN_3
#define COL0_GPIO_Port GPIOB
#define ROW2_Pin GPIO_PIN_4
#define ROW2_GPIO_Port GPIOB
#define ROW2_EXTI_IRQn EXTI4_IRQn
#define ROW3_Pin GPIO_PIN_5
#define ROW3_GPIO_Port GPIOB
#define ROW3_EXTI_IRQn EXTI9_5_IRQn
#define SD_CS_Pin GPIO_PIN_6
#define SD_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define HSPI_SDCARD &hspi1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
