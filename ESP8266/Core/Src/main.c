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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ESP8266.h"
#include "stdio.h"
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
  MX_DMA_Init();
  MX_USART6_UART_Init();
  MX_TIM6_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  char buf[128];

  init();

//  sendCmd("ATE0"    , "OK" , 100);  			//关闭回显
//  sendCmd("AT+CWMODE_DEF=3","OK",1500);			//设置ESP8266为AP模式
//
//  //设置模块的AP模式
//  sprintf(buf,"AT+CWSAP=\"%s\",\"%s\",%s,%s","ATK_ESP8266","12345678","10","4");
//  sendCmd(buf,"OK",2000);
//
//  /* 设置模块AP当前IP地址 */
//  sprintf(buf,"AT+CIPAP_CUR=\"%s\",\"%s\",\"%s\"", "192.168.0.233","192.168.5.1","255.255.255.0");
//  sendCmd(buf, "OK", 1500);
//
//  //创建多连接
//  sendCmd("AT+CIPMUX=1","OK",1000);
//
//  //创建模块TCP服务器
//  //创建TCP服务器之后，会自动启动TCP服务监听，当有TCP客户端连接时，会有CONNECT提示
//  sprintf(buf,"AT+CIPSERVER=%s,%s","1","1001");
//  sendCmd(buf,"OK",2000);




  	while(sendCmd("AT", "OK", 100) == Timeout){
  		quitTrans(); //退出透传
  	}

  	sendCmd("ATE0"    , "OK" , 100);  //关闭回显
  	sendCmd("AT+CWMODE_DEF=1","OK",1500);			//设置ESP8266为STA模式
  	//	sendCmd("AT+CWLAP", "OK" , 5000); //显示可加入的AP

  	/* 加入wifi */
  	sprintf(buf,"AT+CWJAP=\"%s\",\"%s\"","ATK_ESP8266","12345678");
  	sendCmd(buf, "WIFI GOT IP", 15000); //加入AP
  	send((char*)"", "OK"         , 2000); //加入AP

  	/* 设置IP */
  	sprintf(buf,"AT+CIPSTA=\"%s\"", "192.168.0.133");
  	sendCmd(buf, "OK", 15000);

  	/* 与PC机建立连接 */
  	sprintf(buf,"AT+CIPSTART=\"%s\",\"%s\",%s", "TCP", "192.168.0.233", "1001");
  //	sprintf(buf,"AT+CIPSTART=\"%s\",\"%s\",%s", "TCP", "192.168.43.66", "8087");
  	sendCmd(buf, "OK", 15000);

  	/* 进入透传模式 */
//  	sendCmd("AT+CIPMODE=1", "OK", 200); //开启透传模式
//  	sendCmd("AT+CIPSEND"  , ">", 100); //进入透传

//  //发送消息
//  while(1){
//	  static int num=0;
//	  sprintf(buf,"%4d\r\n",num++);
//
//	  sendMessage(buf,"0","6");
//	  sendMessage(buf,"1","6");
//	  sendMessage(buf,"2","6");
//  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
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
