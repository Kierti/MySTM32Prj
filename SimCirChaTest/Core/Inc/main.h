/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define AD9959_SDIO2_Pin GPIO_PIN_4
#define AD9959_SDIO2_GPIO_Port GPIOE
#define AD9959_SDIO1_Pin GPIO_PIN_5
#define AD9959_SDIO1_GPIO_Port GPIOE
#define AD9959_SDIO3_Pin GPIO_PIN_6
#define AD9959_SDIO3_GPIO_Port GPIOE
#define AD9959_SDIO0_Pin GPIO_PIN_0
#define AD9959_SDIO0_GPIO_Port GPIOF
#define AD9959_SCLK_Pin GPIO_PIN_1
#define AD9959_SCLK_GPIO_Port GPIOF
#define AD9959_PS3_Pin GPIO_PIN_2
#define AD9959_PS3_GPIO_Port GPIOF
#define AD9959_CS_Pin GPIO_PIN_3
#define AD9959_CS_GPIO_Port GPIOF
#define AD9959_PS2_Pin GPIO_PIN_4
#define AD9959_PS2_GPIO_Port GPIOF
#define AD9959_UPDATE_Pin GPIO_PIN_5
#define AD9959_UPDATE_GPIO_Port GPIOF
#define AD9959_PS1_Pin GPIO_PIN_6
#define AD9959_PS1_GPIO_Port GPIOF
#define AD9959_RESET_Pin GPIO_PIN_7
#define AD9959_RESET_GPIO_Port GPIOF
#define AD9959_PS0_Pin GPIO_PIN_8
#define AD9959_PS0_GPIO_Port GPIOF
#define AD9959_PDC_Pin GPIO_PIN_9
#define AD9959_PDC_GPIO_Port GPIOF
#define ADS8688_RST_PD_Pin GPIO_PIN_2
#define ADS8688_RST_PD_GPIO_Port GPIOG
#define ADS8688_SDI_Pin GPIO_PIN_3
#define ADS8688_SDI_GPIO_Port GPIOG
#define ADS8688_SCLK_Pin GPIO_PIN_4
#define ADS8688_SCLK_GPIO_Port GPIOG
#define ADS8688_DAISY_Pin GPIO_PIN_5
#define ADS8688_DAISY_GPIO_Port GPIOG
#define ADS8688_SDO_Pin GPIO_PIN_6
#define ADS8688_SDO_GPIO_Port GPIOG
#define ADS8688_CS_Pin GPIO_PIN_7
#define ADS8688_CS_GPIO_Port GPIOG
#define Relay_Pin GPIO_PIN_9
#define Relay_GPIO_Port GPIOG
#define Relay2_Pin GPIO_PIN_10
#define Relay2_GPIO_Port GPIOG
#define LED0_Pin GPIO_PIN_13
#define LED0_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
