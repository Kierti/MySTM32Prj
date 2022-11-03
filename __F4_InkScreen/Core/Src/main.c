/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "EPD_2in13_V3.h"
#include "EPD_Test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const unsigned char gImage_Lissandra[3456] = { /* 0X81,0X01,0XD8,0X00,0X7A,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X20,0X20,0X00,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X1F,0X84,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X0D,0XC1,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X05,0XE8,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X78,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X7C,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X6F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X27,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X11,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X04,0XF4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X03,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFC,0X00,0X00,0X00,0X40,0X00,0X00,0X06,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0X02,0X00,0X00,0X00,0X00,0X00,0X03,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XB6,0X00,0X00,0X00,0X00,0X00,0X03,0X80,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XEF,0XDE,0X00,0X00,0X00,0X00,0X00,0X01,0X80,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0X5F,0XFC,0X40,0X00,0X00,0X00,0X01,0X80,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XEF,0XEF,0XFF,0XE8,0X00,0X00,0X00,0X01,0X80,
0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XEF,0XFF,0XF8,0X00,0X00,0X00,0X00,0XC0,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XDF,0XFC,0X04,0X00,0X00,0X00,0XC0,
0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XA4,0X00,0X00,0X00,0XC0,
0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XF0,0X3F,0XF4,0X00,0X00,0X00,0XC0,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XC0,0X07,0XFC,0X00,0X00,0X00,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0X80,0X07,0XFC,0X00,0X00,0X00,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0X00,0X01,0XF8,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFE,0X00,0X01,0XF0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XF8,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XE0,0X00,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XC0,0X00,0X1E,0X00,0X00,0X00,0X00,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFE,0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFC,0X00,0X00,0X1C,0X00,0X00,0X00,0X00,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFC,0X00,0X00,0X38,0X98,0X00,0X10,0X00,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0X01,0X00,0X67,0XBC,0X00,0X07,0X00,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFC,0X00,0X4F,0XFF,0X00,0X01,0X80,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC3,0X80,0X00,0XFF,0XFF,0XE0,0X00,0X40,0X40,
0X00,0X00,0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XE0,0X80,0X18,0XC0,
0X00,0X03,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XB1,0X00,0X00,0XC0,
0X00,0X03,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0X83,0X03,0XC0,
0X00,0X00,0XFF,0XFE,0X00,0X00,0X00,0X1A,0XC0,0X3F,0XFF,0XFF,0XC0,0X00,0X00,0XC0,
0X00,0X00,0X7F,0XFF,0X00,0X00,0X00,0X10,0X02,0X7F,0XFF,0XFF,0X00,0X00,0X00,0X00,
0X00,0X0F,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X17,0XFF,0XFF,0XFF,0XFE,0X00,0X00,0X00,
0X00,0X0F,0XFF,0XC4,0X00,0X00,0X04,0X00,0X27,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,
0X00,0X1F,0XFF,0X00,0X00,0X00,0X20,0X80,0X4F,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X00,
0X00,0X0F,0XFE,0X00,0X84,0X00,0X12,0X60,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,
0X00,0X03,0XFC,0X00,0X23,0X00,0X01,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XF1,0XF8,0X00,
0X00,0X23,0XF8,0X00,0X1E,0X80,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFE,0X00,
0X00,0X0F,0XF8,0X00,0X07,0XC0,0X20,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,
0X00,0X07,0XFF,0X00,0X00,0X07,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,
0X00,0X07,0XFF,0X08,0X00,0X03,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0X0F,0XFD,0XC0,0X00,
0X00,0X07,0XFF,0X00,0X00,0X87,0XFB,0XFF,0XFF,0XFF,0XEF,0XFF,0XF1,0XF0,0X00,0X00,
0X08,0X0F,0XFF,0X80,0X00,0X1F,0XF7,0XFF,0XFF,0XFF,0XBF,0XFF,0XFE,0X00,0X00,0X00,
0X0C,0X0F,0XFF,0X80,0X00,0X1F,0XE7,0XFF,0XFF,0XFE,0XFF,0XFF,0XC0,0X00,0X00,0X00,
0X0C,0X1F,0XFF,0XFF,0XFF,0X3F,0XCF,0XFF,0XFF,0XFD,0XFF,0XFE,0X00,0X00,0X00,0X00,
0X28,0X3F,0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,
0X02,0X3F,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X40,0X00,
0X02,0X7F,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFE,0X00,0X07,0XF8,0X00,0X0C,0X00,0X00,
0X07,0XFF,0XFF,0XFF,0XFF,0XFE,0X6F,0XFF,0XC0,0X00,0X05,0XFF,0XE0,0X60,0X00,0X00,
0X0A,0XF1,0XFF,0XFF,0XFF,0XFC,0XFF,0XF8,0X00,0X00,0X00,0X7F,0XFE,0X00,0X00,0X00,
0X1E,0X00,0X7F,0XFF,0XFF,0XF8,0XFF,0XC0,0X00,0X00,0X01,0X7F,0XFC,0X00,0X00,0X00,
0X1F,0X00,0XFF,0XFF,0XFF,0XF1,0XFE,0X00,0X00,0X00,0X31,0X9F,0XF0,0X00,0X00,0X00,
0X3F,0X84,0X3F,0XFF,0XFF,0XF1,0XF8,0X00,0X00,0X00,0X1E,0X0F,0XF8,0X00,0X00,0X00,
0X3F,0X87,0X3F,0XFF,0XBF,0XE7,0XC0,0X00,0X00,0X00,0X67,0X97,0XFE,0X00,0X00,0X00,
0X3F,0X87,0X3F,0XFE,0X1F,0X8F,0X00,0X00,0X00,0X0F,0XF9,0XE3,0XFF,0XC0,0X00,0X00,
0X3F,0XC7,0XBF,0XFC,0X1E,0X1C,0X00,0X00,0X00,0X00,0XFE,0XF7,0XFF,0XF0,0X00,0X00,
0X3F,0XC7,0XBF,0XBC,0X1C,0X18,0X00,0X00,0X00,0X00,0XFF,0XBB,0XFF,0XFC,0X00,0X00,
0X7F,0XE6,0X3F,0X38,0X10,0X00,0X00,0X00,0X00,0X00,0X7F,0XDF,0XFF,0XFF,0X00,0X00,
0X7F,0X87,0X22,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0X37,0XE0,0X00,
0X7F,0X07,0X20,0X60,0X20,0X00,0X00,0X00,0X00,0X01,0X7F,0XFF,0XEF,0X01,0XF8,0X00,
0X7F,0X07,0X17,0XE0,0X00,0X00,0X00,0X10,0X00,0X00,0X7F,0XF0,0X87,0X03,0XFE,0X00,
0XFF,0X06,0X17,0XC0,0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X08,0X00,0X03,0XFF,0X00,
0XFF,0X80,0X0F,0X80,0X60,0X00,0X00,0X00,0X00,0X00,0X02,0X00,0X00,0X07,0XFF,0X00,
0XFF,0XE0,0X0C,0X00,0XE0,0X00,0X00,0X00,0X00,0X06,0X80,0X00,0X03,0XFF,0XE0,0X00,
0XFF,0XE3,0X00,0X00,0XE0,0X00,0X03,0X00,0X00,0X00,0X00,0X00,0X41,0XFC,0X00,0X00,
0XFF,0XE0,0X00,0X00,0XE0,0X00,0X0B,0XC0,0X00,0X00,0X00,0X00,0X02,0XE0,0X00,0X00,
0XFF,0XE0,0X00,0X00,0XE0,0X00,0X3F,0XD9,0X00,0X00,0X02,0X08,0X0F,0X00,0X00,0X00,
0X7F,0XC2,0X00,0X00,0XE0,0X00,0X3F,0XE5,0X80,0X00,0X00,0X00,0X1F,0XF0,0X00,0X00,
0X7F,0X81,0X38,0X01,0XE0,0X00,0X1F,0XE3,0X80,0X00,0X00,0X00,0X07,0XE0,0X00,0X00,
0X7F,0X87,0X1C,0X00,0XA0,0X00,0X1F,0XC4,0X40,0X00,0X00,0X00,0X00,0X07,0X00,0X00,
0X7F,0XB2,0X0E,0X01,0X80,0X00,0X0F,0XD8,0X00,0X00,0X00,0X10,0X00,0X00,0X00,0X00,
0X3F,0XB1,0XC0,0X00,0XC0,0X10,0X0F,0XD1,0X00,0X00,0X00,0X06,0X00,0X00,0X00,0X00,
0X1F,0XE0,0X00,0X00,0X00,0X00,0X0F,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X1F,0XCC,0X00,0X74,0X00,0X00,0X00,0X88,0X00,0X00,0X00,0X02,0X00,0X00,0X00,0X00,
0X1F,0XC1,0X04,0X00,0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X1F,0XF8,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0X00,0X00,0X00,
0X0F,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X00,0X00,0X00,
0X0F,0XFC,0X00,0X00,0X00,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X07,0X00,0X00,0X00,
0X07,0XFC,0X10,0X00,0X30,0X00,0X00,0X80,0X00,0X00,0X00,0X00,0X01,0XC0,0X00,0X00,
0X03,0XFE,0X12,0X00,0X1E,0X00,0X01,0X88,0X00,0X00,0X00,0X00,0X1A,0X88,0X00,0X00,
0X03,0XFD,0XD6,0X00,0X0F,0X80,0X00,0X00,0X00,0X00,0X00,0X30,0X01,0X87,0X00,0X00,
0X03,0XFC,0X66,0X00,0X01,0XE0,0X00,0X10,0X00,0X00,0X00,0X10,0X00,0X00,0X00,0X00,
0X03,0XF2,0X66,0X00,0X00,0X70,0X00,0X00,0X00,0X00,0X00,0X07,0X80,0X00,0X00,0X00,
0X03,0XE0,0X6E,0X00,0X00,0XF8,0X00,0X80,0X00,0X03,0X00,0X01,0XE0,0X00,0X00,0X00,
0X03,0XCF,0XFD,0X00,0X00,0XFC,0X00,0X00,0X00,0X03,0XE0,0X00,0X7C,0X00,0X00,0X00,
0X03,0XFF,0XFD,0X11,0X81,0XFE,0X00,0X00,0X00,0X03,0XFC,0X00,0X3F,0X82,0X18,0X00,
0X03,0XBF,0XF8,0X31,0X83,0XFF,0X00,0X00,0X00,0X07,0XFF,0X80,0XFF,0XF7,0X80,0X00,
0X01,0X80,0X7F,0XF9,0XCF,0XFF,0X80,0X00,0X00,0X07,0XFF,0XF0,0XBF,0XFF,0X00,0X00,
0X01,0X98,0X7F,0XFC,0XFF,0XFF,0XC0,0X00,0X00,0X0F,0XFF,0XFE,0X1F,0XFF,0X00,0X00,
0X01,0XBC,0X7F,0XFD,0X7F,0XFF,0XC0,0X00,0X02,0X0F,0XFF,0XFF,0X9F,0XF0,0X00,0X00,
0X00,0X9F,0XFF,0XFE,0XFF,0XFF,0XE0,0X00,0X00,0X7F,0XFF,0XBF,0XFF,0X00,0X00,0X00,
0X00,0X9F,0XFF,0XFE,0XFF,0XFF,0XE0,0X01,0X00,0X1F,0XFF,0XFB,0XF0,0X00,0X00,0X00,
0X00,0XEF,0XFF,0XFE,0XFF,0XFF,0XF0,0X00,0X00,0X3F,0XFF,0XFF,0X00,0X00,0X00,0X00,
0X00,0XEF,0XFF,0XFE,0X5F,0XFF,0XF0,0X00,0X20,0X3F,0XFF,0XE0,0X00,0X00,0X00,0X00,
0X00,0X0F,0XFF,0XFE,0X5F,0XFF,0XF0,0X00,0X08,0X7F,0XFE,0X00,0X00,0X00,0X08,0X00,
0X00,0X0F,0XFF,0XFE,0X7F,0XEE,0X00,0X00,0X06,0X7F,0XF8,0X00,0X00,0X00,0X00,0X00,
0X00,0X1F,0XFF,0XFC,0X7F,0XDE,0X00,0X00,0X01,0XFF,0XFF,0X3C,0X00,0X00,0X00,0X00,
0X00,0X3F,0XFF,0XFC,0XFF,0X1C,0X00,0X00,0X00,0XFF,0XDC,0X64,0X60,0X00,0X00,0X00,
0X00,0X0F,0XFF,0XFC,0X7E,0X38,0X00,0X00,0X01,0XFF,0XFF,0XFC,0X08,0X00,0X00,0X00,
0X00,0X05,0XFF,0XFC,0XFE,0X38,0X00,0X00,0X01,0XFF,0XFB,0XE8,0X60,0X00,0X01,0X00,
0X00,0X00,0X73,0X5C,0XFC,0X72,0X00,0X00,0X03,0XFF,0XFE,0XF4,0X7F,0X00,0X03,0X00,
0X00,0X00,0X19,0X3D,0XF8,0X60,0X00,0X00,0X07,0XFF,0XFF,0XD0,0X3F,0X80,0X42,0X00,
0X00,0X00,0X1F,0XF9,0XF0,0XE0,0X00,0X01,0X07,0XFF,0XFE,0X0C,0X1F,0X80,0X06,0X00,
0X00,0X00,0X3F,0XFB,0XFC,0XC0,0X00,0X00,0X8F,0XFE,0XE0,0X03,0XFE,0X00,0X84,0X00,
0X00,0X00,0X7F,0XFB,0XFD,0XC0,0X00,0X00,0X0F,0XFF,0X70,0X7F,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X06,0X00,0X80,0X0B,0X80,0X5F,0XFD,0X85,0XFB,0X08,0X00,0X00,0X00,
0X00,0X00,0X00,0X06,0X10,0X00,0X00,0X80,0X3C,0XE0,0X1F,0XF3,0X08,0X00,0X00,0X00,
0X00,0X00,0X00,0X0C,0X00,0X04,0X30,0XC0,0X11,0X80,0XFF,0X9E,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5C,0X00,0X00,0X1B,0XC0,0X22,0X07,0XFF,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X30,0X18,0X00,0X00,0X1B,0XE0,0X0C,0X1F,0XFF,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X21,0X28,0X00,0X00,0XFB,0XF0,0X30,0XFF,0XC1,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X0B,0XFB,0XE0,0X03,0XFE,0X00,0X00,0X00,0X01,0XE0,0X00,
0X00,0X00,0X00,0X80,0X00,0X0F,0XFF,0XF0,0X0F,0XC0,0X00,0X00,0X00,0X03,0XC0,0X00,
0X00,0X00,0X08,0X80,0X00,0X3F,0XFD,0XF0,0X7E,0X00,0X03,0X00,0X01,0X07,0X80,0X00,
0X00,0X00,0X08,0X00,0X00,0X7F,0XFD,0XE0,0X30,0X00,0X01,0X00,0X00,0XB7,0X00,0X00,
0X00,0X00,0X02,0X40,0X01,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0XF4,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X0F,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X1F,0XFF,0XE4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X7F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X01,0XFF,0XFF,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XFF,0XE6,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0XF7,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X1F,0XFF,0X9C,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X3A,0X79,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X20,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X41,0X00,0X00,0X00,0X00,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X04,0X00,0X00,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //Create a new image cache
   EPD_2in13_V3_Init();
   EPD_2in13_V3_Clear();
   UBYTE *BlackImage;
   UWORD Imagesize = ((EPD_2in13_V3_WIDTH % 8 == 0)? (EPD_2in13_V3_WIDTH / 8 ): (EPD_2in13_V3_WIDTH / 8 + 1)) * EPD_2in13_V3_HEIGHT;
   if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
       Debug("Failed to apply for black memory...\r\n");
       return -1;
   }
   Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);
//   Paint_Clear(WHITE);
//   Paint_SelectImage(BlackImage);
   Paint_Clear(WHITE);
   Paint_DrawBitMap(gImage_Lissandra);
   EPD_2in13_V3_Display(BlackImage);

   Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);
   Paint_SelectImage(BlackImage);
   Paint_ClearWindows(210,1, 250,122, BLACK);
   EPD_2in13_V3_Display_Partial(BlackImage);
   DEV_Delay_ms(2000);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_Delay(10000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
