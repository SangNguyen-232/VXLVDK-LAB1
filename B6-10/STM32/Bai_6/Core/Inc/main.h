/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Chan_1_Pin GPIO_PIN_4
#define Chan_1_GPIO_Port GPIOA
#define Chan_2_Pin GPIO_PIN_5
#define Chan_2_GPIO_Port GPIOA
#define Chan_3_Pin GPIO_PIN_6
#define Chan_3_GPIO_Port GPIOA
#define Chan_4_Pin GPIO_PIN_7
#define Chan_4_GPIO_Port GPIOA
#define Chan_5_Pin GPIO_PIN_8
#define Chan_5_GPIO_Port GPIOA
#define Chan_6_Pin GPIO_PIN_9
#define Chan_6_GPIO_Port GPIOA
#define Chan_7_Pin GPIO_PIN_10
#define Chan_7_GPIO_Port GPIOA
#define Chan_8_Pin GPIO_PIN_11
#define Chan_8_GPIO_Port GPIOA
#define Chan_9_Pin GPIO_PIN_12
#define Chan_9_GPIO_Port GPIOA
#define Chan_10_Pin GPIO_PIN_13
#define Chan_10_GPIO_Port GPIOA
#define Chan_11_Pin GPIO_PIN_14
#define Chan_11_GPIO_Port GPIOA
#define Chan_12_Pin GPIO_PIN_15
#define Chan_12_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
