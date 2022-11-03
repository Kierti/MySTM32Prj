/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "base.h"
#include "lcd.h"
#include "24cxx.h"
#include "touch.h"
#include "stdio.h"
#include "stack.h"
#include "stdlib.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LENGTH 1024			//��������С
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char RxBuffer[LENGTH];		//���ջ�����
uint8_t RxFlag=0;			//���ձ�־����
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Usart_info(void);
void UI_1(void);	//UI����1
void UI_2(void);	//UI����2
void Key_staset(u16 x,u16 y,u8 keyx,u8 sta);
u8 Get_key(u16 x,u16 y);
void Key_staset2(u16 x,u16 y,u8 keyx,u8 sta);
u8 Get_key2(u16 x,u16 y);
char DataChange(int a,int b);
char *DataHandle(char *e);
void UI_1_Funiction(void);
void UI_2_Funiction(void);
//�����Ļ�������Ͻ���??"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//����   
 	POINT_COLOR=BLUE;//��������Ϊ��?? 
	LCD_ShowString(lcddev.width-24,0,200,16,16,(u8 *)"RST");//��ʾ��������
	POINT_COLOR=RED;//���û�����ɫ 
}
////////////////////////////////////////////////////////////////////////////////
//���ݴ�����ר�в�??
//��ˮƽ��
//x0,y0:����
//len:�߳�??
//color:��ɫ
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//��ʵ��Բ
//x0,y0:����
//r:�뾶
//color:��ɫ
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//������֮��ľ���?? 
//x1,x2����ȡ��ֵ������??
//����ֵ��|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//��һ����??
//(x1,y1),(x2,y2):��������ʼ��??
//size�������Ĵ�ϸ�̶�
//color����������ɫ
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ?? 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ?? 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ������������?? 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		gui_fill_circle(uRow,uCol,size,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
//5�����ص����??(���ݴ�������)												 
const u16 POINT_COLOR_TBL[5]={RED,GREEN,BLUE,BROWN,GRED};  
//���败�������Ժ�??
void rtp_test(void)
{
	u8 key;
	u8 i=0;	  
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//���
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//��ͼ	  			   
			}
		}else delay_ms(10);	//û�а������µ�ʱ?? 	    
		if(key==KEY0_PRES)	//KEY0����,��ִ��У׼��??
		{
			LCD_Clear(WHITE);	//����
		    TP_Adjust();  		//��ĻУ׼ 
			TP_Save_Adjdata();	 
			Load_Drow_Dialog();
		}
		i++;
		if(i%20==0)LED0_T;
	}
}
//���ݴ��������Ժ�??
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//??��һ�ε����� 
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
                //printf("X����:%d,Y����:%d\r\n",tp_dev.x[0],tp_dev.y[0]);
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
                    
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//����
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//���
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0)LED0_T;
	}	
}
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  delay_init(72);
  LCD_Init();
  tp_dev.init();				   		//��������ʼ�� 
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	Usart_info();
	UI_1_Funiction();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//���������Ϣ
void Usart_info(void)
{
  printf("\t[����Ҫ��]info:��־��\r\n");
  printf("\t[����Ҫ��]info:202021111112\r\n");
  printf("\t[����Ҫ��]info:��=3.1415926\r\n");
  printf("\t[���ڲ�������1]operation:q->LED0\r\n");
  printf("\t[���ڲ�������1]operation:w->LED1\r\n");
  printf("\t[���ڲ�������1]operation:e->HouseRaceLamp\r\n");
  printf("\t[���ڲ�������1]operation:r->BreathLamp\r\n");
  printf("\t[���ڲ�������1]operation:b->�����л�\r\n");
  printf("\t[���ڲ�������2]operation:c->����/��ʼ\r\n");
  printf("\t[���ڲ�������2]operatiow:d->�˸�\r\n");
  printf("\t[���ڲ�������2]operation:a->�����л�\r\n");	
}
//���ڽ����жϻص�����
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==huart2.Instance)
	{
		RxFlag=1;		//��λ������ɱ�־
		HAL_UART_Receive_IT(&huart2,(uint8_t*)RxBuffer,1);
	}
}
	
//һ���˵�����
void UI_1(void)			
{
	POINT_COLOR=BLACK;
	LCD_Show_CH_Font24(80,15,0);
	LCD_Show_CH_Font24(105,15,1);
	LCD_Show_CH_Font24(130,15,2);
	LCD_ShowString(30,40,200,24,24,"  Wang ZhiHan");
	LCD_ShowString(30,70,200,24,24," 202021111112");
	LCD_ShowString(30,100,200,24,24,"  3.1415926");
	LCD_DrawRectangle(0,0,20,20);
	LCD_DrawRectangle(0,160,120,240);		//LED0
	LCD_DrawRectangle(0,240,120,320);		//LED1
	LCD_DrawRectangle(120,160,240,240);		//HouseRaceLamp
	LCD_DrawRectangle(120,240,240,320);		//BreathLamp
	POINT_COLOR=RED;
	LCD_ShowString(35,190,120,80,24,"LED0");
	LCD_ShowString(155,190,120,80,24,"LED1");
	LCD_ShowString(5,255,100,70,24,"HouseRaceLamp");
	LCD_ShowString(135,255,70,70,24,"BreathLamp");
}

//�����˵�����
void UI_2(void)
{
	POINT_COLOR=BLACK;
	LCD_DrawRectangle(0,0,20,20);
	LCD_DrawRectangle(0,20,240,320);
	LCD_DrawRectangle(0,120,60,160);
	LCD_DrawRectangle(60,120,120,160);
	LCD_DrawRectangle(120,120,180,160);
	LCD_DrawRectangle(180,120,240,160);
	LCD_DrawRectangle(0,160,60,200);
	LCD_DrawRectangle(0,200,60,240);
	LCD_DrawRectangle(0,240,60,280);
	LCD_DrawRectangle(0,280,60,320);
	LCD_DrawRectangle(60,160,120,200);
	LCD_DrawRectangle(60,200,120,240);
	LCD_DrawRectangle(60,240,120,280);
	LCD_DrawRectangle(60,280,120,320);
	LCD_DrawRectangle(120,160,180,200);
	LCD_DrawRectangle(120,200,180,240);
	LCD_DrawRectangle(120,240,180,280);
	LCD_DrawRectangle(120,280,180,320);
	LCD_DrawRectangle(180,160,240,200);
	LCD_DrawRectangle(180,200,240,240);
	LCD_DrawRectangle(180,240,240,280);
	LCD_DrawRectangle(180,280,240,320);
	POINT_COLOR=RED;
	LCD_ShowString(20,130,60,40,24,"C");
	LCD_ShowString(20,170,60,40,24,"7");
	LCD_ShowString(20,210,60,40,24,"4");
	LCD_ShowString(20,250,60,40,24,"1");
	LCD_ShowString(15,290,60,40,24,".");
	LCD_ShowString(80,130,60,40,24,"/");
	LCD_ShowString(80,170,60,40,24,"8");
	LCD_ShowString(80,210,60,40,24,"5");
	LCD_ShowString(80,250,60,40,24,"2");
	LCD_ShowString(80,290,60,40,24,"0");
	LCD_ShowString(140,130,60,40,24,"*");
	LCD_ShowString(140,170,60,40,24,"9");
	LCD_ShowString(140,210,60,40,24,"6");
	LCD_ShowString(140,250,60,40,24,"3");
	LCD_ShowString(140,290,60,40,24,"(");
	LCD_ShowString(190,130,60,40,24,"del");
	LCD_ShowString(200,170,60,40,24,"-");
	LCD_ShowString(200,210,60,40,24,"+");
	LCD_ShowString(200,250,60,40,24,"=");
	LCD_ShowString(200,290,60,40,24,")");
	
}
//��������������״̬����
void Key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{
	u16 i=keyx/2,j=keyx%2;		//j:����|i:����
	if(keyx>5) return;
	if(sta&&keyx!=5)
		LCD_Fill(x+j*120+1,y+i*80+1,x+j*120+118,y+i*80+78,RED);	//�����򱻰����������Ϊ��ɫ
	else if(keyx!=5)
		LCD_Fill(x+j*120+1,y+i*80+1,x+j*120+118,y+i*80+78,WHITE);
	
	if(sta&&keyx==5)					//�˵��л�����
		LCD_Fill(1,1,19,19,RED);
	else if(keyx==5)
		LCD_Fill(1,1,19,19,WHITE);
	
	if(keyx!=6)		//��������������ʾ
	{
		LCD_ShowString(35,190,120,80,24,"LED0");
		LCD_ShowString(155,190,120,80,24,"LED1");
		LCD_ShowString(5,255,100,70,24,"HouseRaceLamp");
		LCD_ShowString(135,255,70,70,24,"BreathLamp");
	}
}

//������������ȡ����
u8 Get_key(u16 x,u16 y)
{
	u16 i,j;
	static u8 Key_x=0;
	u8 key=0;
	HAL_UART_Receive_IT(&huart2,(uint8_t*)RxBuffer,1);
	if(RxFlag==1)
	{
		if(RxBuffer[0]=='b')
		{
			key=5;
			printf("\t[���Ӳ���]info:UI1->UI2\r\n");
			RxFlag=0;
		}
		else if(RxBuffer[0]=='q')
		{
			key=1;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='w')
		{
			key=2;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='e')
		{
			key=3;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='r')
		{
			key=4;
			RxFlag=0;
		}
		
	}
	
	tp_dev.scan(0);		//������ɨ��
	if(tp_dev.sta&TP_PRES_DOWN)		//������������
	{
		if(tp_dev.x[0]<20&&tp_dev.x>0&&tp_dev.y[0]<20&&tp_dev.y[0]>0)		//�˵��л�ɨ��
		{
			key=5;
			Key_staset(x,y,key,1);
			HAL_Delay(100);
			Key_staset(x,y,key,0);
		}	
		
		for(i=0;i<2;i++)
		{
			for(j=0;j<2;j++)
			{
				if(tp_dev.x[0]<(x+j*120+120)&&tp_dev.x[0]>(x+j*120)&&tp_dev.y[0]<(y+i*80+80)&&tp_dev.y[0]>(y+i*80))
				{
					key=i*2+j+1;		//i�����У�ÿһ�б仯��Ҫ����ֵj�����У�ÿһ�б仯ֻ��Ҫ�仯һ��ֵ
					break;
				}
			}
			if(key)
			{
				if(Key_x==key) key=0;
				else
				{
					Key_staset(x,y,Key_x-1,0);
					Key_x=key;
					Key_staset(x,y,Key_x-1,1);
				}
				break;
			}
		}
	}
	else if(Key_x)		//ÿ�δ�0��ʼ������¼�ϴε�״̬
	{
		Key_staset(x,y,Key_x-1,0);
		Key_x=0;
	}
	return key;
}

//��������״̬����2
void Key_staset2(u16 x,u16 y,u8 keyx,u8 sta)
{
	u16 i=keyx/4,j=keyx%4;
	if(keyx>21) return;
	
	if(sta&&keyx!=21)
		LCD_Fill(x+j*60+1,y+i*40+1,x+j*60+58,y+i*40+38,RED);
	else if(keyx!=21)
		LCD_Fill(x+j*60+1,y+i*40+1,x+j*60+58,y+i*40+38,WHITE);

	if(sta&&keyx==21)			//�˵��л�����
		LCD_Fill(1,1,19,19,RED);
	else if(sta==0&&keyx==21)
		LCD_Fill(1,1,19,19,WHITE);
	
	if(keyx!=22)
	{
	LCD_ShowString(20,130,60,40,24,"C");
	LCD_ShowString(20,170,60,40,24,"7");
	LCD_ShowString(20,210,60,40,24,"4");
	LCD_ShowString(20,250,60,40,24,"1");
	LCD_ShowString(15,290,60,40,24,".");
	LCD_ShowString(80,130,60,40,24,"/");
	LCD_ShowString(80,170,60,40,24,"8");
	LCD_ShowString(80,210,60,40,24,"5");
	LCD_ShowString(80,250,60,40,24,"2");
	LCD_ShowString(80,290,60,40,24,"0");
	LCD_ShowString(140,130,60,40,24,"*");
	LCD_ShowString(140,170,60,40,24,"9");
	LCD_ShowString(140,210,60,40,24,"6");
	LCD_ShowString(140,250,60,40,24,"3");
	LCD_ShowString(140,290,60,40,24,"(");
	LCD_ShowString(190,130,60,40,24,"del");
	LCD_ShowString(200,170,60,40,24,"-");
	LCD_ShowString(200,210,60,40,24,"+");
	LCD_ShowString(200,250,60,40,24,"=");
	LCD_ShowString(200,290,60,40,24,")");
	}
}

//������������ȡ����2
u8 Get_key2(u16 x,u16 y)
{
	u16 i,j;
	static u8 Key_x=0;
	u8 key=0;
	HAL_UART_Receive_IT(&huart2,(uint8_t*)RxBuffer,1);
	tp_dev.scan(0);
	if(RxFlag==1)
	{
		if(RxBuffer[0]=='1')
		{
			key=13;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='2')
		{
			key=14;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='3')
		{
			key=15;

			RxFlag=0;
		}
		else if(RxBuffer[0]=='4')
		{
			key=9;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='5')
		{
			key=10;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='6')
		{
			key=9;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='7')
		{
			key=5;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='8')
		{
			key=6;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='9')
		{
			key=7;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='0')
		{
			key=18;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='+')
		{
			key=12;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='-')
		{
			key=8;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='*')
		{
			key=3;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='/')
		{
			key=2;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='d')
		{
			key=4;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='(')
		{
			key=19;
			RxFlag=0;
		}
		else if(RxBuffer[0]==')')
		{
			key=20;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='=')
		{
			key=16;
			RxFlag=0;
		}
		else if(RxBuffer[0]=='c')
		{
			key=1;
			printf("\n\t[������]info:����/��ʼ\r\n");
			RxFlag=0;
		}
		else if(RxBuffer[0]=='a')
		{
			key=21;
			printf("\n\t[���Ӳ���]info:UI2->UI1\r\n");
			RxFlag=0;
		}
		else if(RxBuffer[0]=='.')
		{
			key=17;
			printf(".");
			RxFlag=0;
		}	
	}
	if(tp_dev.sta&TP_PRES_DOWN)
	{
		if(tp_dev.x[0]<20&&tp_dev.x>0&&tp_dev.y[0]<20&&tp_dev.y[0]>0)		//�˵��л�ɨ��
		{
			key=21;
			Key_staset2(x,y,key,1);
			HAL_Delay(100);
			Key_staset2(x,y,key,0);
		}	
		
		for(i=0;i<5;i++)
		{
			for(j=0;j<4;j++)
			{
				if(tp_dev.x[0]<(x+j*60+60)&&tp_dev.x[0]>(x+j*60)&&tp_dev.y[0]<(y+i*40+40)&&tp_dev.y[0]>(y+i*40))
				{
					key=i*4+j+1;
					break;
				}
			}
			if(key)
			{
				if(Key_x==key) key=0;
				else
				{
					Key_staset2(x,y,Key_x-1,0);
					Key_x=key;
					Key_staset2(x,y,Key_x-1,1);
				}
				break;
			}
		}
	}
	else if(Key_x)
	{
		Key_staset2(x,y,Key_x-1,0);
		Key_x=0;
	}
	return key;
}

//UI����1����ʵ��
void UI_1_Funiction(void)
{
	LCD_Clear(WHITE);
	UI_1();
	while(1)
	{
		u8 key1;
		key1=Get_key(0,160);
	if(key1==1)
	{
		printf("\t[����Ҫ��]info:LED0 ON\r\n");
		while(1)
		{
			LED0_ON;
			key1=Get_key(0,160);
			if(key1==1)
			{
				LED0_OFF;
				printf("\t[����Ҫ��]info:LED0 OFF\r\n");
				break;
			}
		}
	}
	else if(key1==2)
	{
		printf("\t[����Ҫ��]info:LED1 ON\r\n");
		while(1)
		{
			LED1_ON;
			key1=Get_key(0,160);			
			if(key1==2)
			{
				LED1_OFF;
				printf("\t[����Ҫ��]info:LED1 OFF\r\n");
				break;
			}
		}
	}
	else if(key1==3)
	{
		printf("\t[����Ҫ��]info:����� ON\r\n");
		while(1)
		{
			LED0_ON;		//�����	
			LED1_OFF;
			delay_ms(50);
			key1=Get_key(0,160);
			if(key1==3)
			{
				LED0_OFF;
				LED1_OFF;
				printf("\t[����Ҫ��]info:����� OFF\r\n");
				break;
			}
			LED0_OFF;
			LED1_ON;
			delay_ms(50);
			key1=Get_key(0,160);
			if(key1==3)
			{
				LED0_OFF;
				LED1_OFF;
				printf("\t[����Ҫ��]info:����� OFF\r\n");
				break;
			}
		}
	}
	else if(key1==4)
	{
		int a,b=1001;
		delay_ms(200);
		printf("\t[����Ҫ��]info:������ ON\r\n");
		while(1)
		{
			for(a=1;a<b;a++)
			{
				LED0_OFF;
				LED1_OFF;
				delay_us(a);
				LED0_ON;
				LED1_ON;
				delay_us(b-a);
				key1=Get_key(0,160);
				if(key1==4)	break;
			}
			if(key1==4)		//���������ƣ�Ϩ��
			{
				LED0_OFF;
				LED1_OFF;
				printf("\t[����Ҫ��]info:������ OFF\r\n");
				HAL_Delay(200);
				break;
			}
			for(a=b;a>0;a--)
			{
				LED0_OFF;
				LED1_OFF;
				delay_us(a);
				LED0_ON;
				LED1_ON;
				delay_us(b-a+1);
				key1=Get_key(0,160);
				if(key1==4)	 break;
			}
			if(key1==4)		//���������ƣ�Ϩ��
			{
				LED0_OFF;
				LED1_OFF;
				printf("\t[����Ҫ��]info:������ OFF\r\n");
				HAL_Delay(200);
				break;
			}

		}
	}
	else if(key1==5)
		break;
	}
	LCD_Fill(0,0,240,320,WHITE);
	UI_2_Funiction();
}

void UI_2_Funiction(void)
{
  LCD_Clear(WHITE);
  UI_2();
  LCD_ShowString(22,3,220,20,12,"close!");
  while(1)
  {
		u8 key2;
		key2=Get_key2(0,120);
		if(key2==21)
			break;
		else if(key2==1)
		{
			LCD_Fill(0,21,240,119,WHITE);
			char* o=0;
			while(1)
			{
			LCD_ShowString(22,3,220,20,12,"start!");
			char *Expression = (char*)malloc(sizeof(char)*100);
			o = DataHandle(Expression);
			STACKS(o);
			free(Expression);
				break;
			}
			LCD_ShowString(22,3,220,20,12,"close!");
		}		
		
  }
  LCD_Fill(0,0,240,320,WHITE);
  UI_1_Funiction();
}

//������ɨ��ķ���ֵ��������ת��Ϊ�ַ�
char DataChange(int a,int b)
{
	int Revert=-1;	//Get_key2�����ķ���ֵ
	int x,y;
	char Symbol;	//�����ķ���ֵ
	
	x=a;
	y=b;
	
	while(1)
	{
		Revert=Get_key2(0,120);
		if(Revert>=2&&Revert<=20)
		{
			switch(Revert)		//��ɨ�躯���ķ���ֵת��Ϊ��Ӧ���ַ���ͬʱ����Ļ����ʾ�ַ�
			{
				case 2:Symbol='/';printf("/");LCD_ShowChar(x,y,47,24,0);
					break;
				case 3:Symbol='*';printf("*");LCD_ShowChar(x,y,42,24,0);
					break;
				case 4:Symbol='D';x = x - 20;LCD_ShowChar(x,y,32,24,0);
					break;
				case 5:Symbol='7';printf("7");LCD_ShowChar(x,y,55,24,0);
					break;
				case 6:Symbol='8';printf("8");LCD_ShowChar(x,y,56,24,0);
					break;
				case 7:Symbol='9';printf("9");LCD_ShowChar(x,y,57,24,0);
					break;
				case 8:Symbol='-';printf("-");LCD_ShowChar(x,y,45,24,0);
					break;
				case 9:Symbol='4';printf("4");LCD_ShowChar(x,y,52,24,0);
					break;
				case 10:Symbol='5';printf("5");LCD_ShowChar(x,y,53,24,0);
					break;
				case 11:Symbol='6';printf("6");LCD_ShowChar(x,y,54,24,0);
					break;
				case 12:Symbol='+';printf("+");LCD_ShowChar(x,y,43,24,0);
					break;
				case 13:Symbol='1';printf("1");LCD_ShowChar(x,y,49,24,0);
					break;
				case 14:Symbol='2';printf("2");LCD_ShowChar(x,y,50,24,0);
					break;
				case 15:Symbol='3';printf("3");LCD_ShowChar(x,y,51,24,0);
					break;
				case 16:Symbol='=';printf("=");LCD_ShowChar(x,y,61,24,0);
					break;
				case 17:Symbol='.';printf(".");LCD_ShowChar(x,y,46,24,0);
					break;
				case 18:Symbol='0';printf("0");LCD_ShowChar(x,y,48,24,0);
					break;
				case 19:Symbol='(';printf("(");LCD_ShowChar(x,y,40,24,0);
					break;
				case 20:Symbol=')';printf(")");LCD_ShowChar(x,y,41,24,0);
					break;
			}
			break;
		}
	}
	return Symbol;
}

//������DataChange�ķ���ֵ�������з���ֵ����������ı��ʽ
char *DataHandle(char *e)
{
	int j=0,i;				//j:ѭ������
	int a=1,b=30;			//a��b:���ݸ�Datachange������ֵ��ʹ������Ļ����ʾ�ַ�λ����λ�ı䣬ʵ�ֹ�깦��
	char data;				//��������ֵ
	char* Expression=e;		//���ݹ����Ķ�̬����
	for(i=0;i<100;i++)		//���ٿռ��ʼ��
	{
		Expression[1]=0;
	}
	while(1)				//����һֱ��ȡDataChange�������ص�ֵ��֪�����֡�=������ѭ��
	{
		data=DataChange(a,b);	//��ȡ����ֵ
		if(data=='=')			//�����ֵȺ�������ѭ������������ʽ�Ӹ����㺯��
		{
			j++;
			break;
		}
		else if(data!='D'&&data!='=')		//�����ǵ��ڻ�����λ�������ڶ�̬�����д��°�����������ַ�
		{
			Expression[j]=data;
			j++;
		}
		
		a=a+20;						//������һλ
		if(a>=240&&data!='D')
		{
			a=1;
			b=b+25;
		}
		else if(data=='D')			//������µ���ɾ���������ڶ�̬�����н�������һλ��ֵΪ'\0'��������Ļ�ϵ���һ���ַ�ɾ��������������һ���ַ���λ��
		{
			data='\0';
			j--;
			Expression[j]=data;
			a=a-40;
			if(a==-20&&b==-10)		//�����һ���ַ�����һ�����һ�����������һ�ŵ�һ������ı�������һ�����һ����ɾ��ַ
			{
				a=220;
				b=b-25;
				LCD_ShowChar(a,b,32,24,0);
			}
		}
	}
	return Expression;			//�������̬�������ʽ�Ӹ����㺯��
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
