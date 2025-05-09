/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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
#define ENPower5V_Pin GPIO_PIN_10
#define ENPower5V_GPIO_Port GPIOE
#define USER_KEY_Pin GPIO_PIN_14
#define USER_KEY_GPIO_Port GPIOE
#define LED_G_TlM4_CH3_Pin GPIO_PIN_14
#define LED_G_TlM4_CH3_GPIO_Port GPIOD
#define LED_B_TlM4_CH4_Pin GPIO_PIN_15
#define LED_B_TlM4_CH4_GPIO_Port GPIOD
#define Buzzer_TlM8_CH1_Pin GPIO_PIN_6
#define Buzzer_TlM8_CH1_GPIO_Port GPIOC
#define LED_R_TIM8_CH2_Pin GPIO_PIN_7
#define LED_R_TIM8_CH2_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
