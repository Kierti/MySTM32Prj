/*
 * ADS1256.c
 *
 *  Created on: Jul 5, 2022
 *      Author: 王志涵
 */

#include "ADS1256.h"

void ADS1256_Init(void)
{

//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//	/* GPIO Ports Clock Enable */
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOH_CLK_ENABLE();
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//
//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(ADS1256_RST_GPIO_Port, ADS1256_RST_Pin, GPIO_PIN_RESET);
//
//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(GPIOA, ADS_CS_Pin|ADS1256_DIN_Pin|ADS1256_SCLK_Pin, GPIO_PIN_RESET);
//
//	/*Configure GPIO pin : PtPin */
//	GPIO_InitStruct.Pin = ADS1256_RST_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	HAL_GPIO_Init(ADS1256_RST_GPIO_Port, &GPIO_InitStruct);
//
//	/*Configure GPIO pins : PAPin PAPin PAPin */
//	GPIO_InitStruct.Pin = ADS_CS_Pin|ADS1256_DIN_Pin|ADS1256_SCLK_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//	/*Configure GPIO pins : PAPin PAPin */
//	GPIO_InitStruct.Pin = ADS_DRDY_Pin|ADS_DOUT_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	RST_L;		// 拉低复位引脚
	delay_ms(1);
	RST_H;		// 拉高复位引脚
	delay_ms(100);
	CS_H;			// 拉高片选引脚
	SCLK_L;		// 拉低时钟引脚
	DIN_H;		// 拉高输入引脚
	delay_ms(300);
}

/*
*********************************************************************************************************
*	函 数 名: ADS1256_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
void ADS1256_Send8Bit(uint8_t data)
{
	uint8_t i;
	/* 连续发送多个字节时，需要延迟一下 */
	delay_us(1);
	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for(i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			DIN_H;
		}
		else
		{
			DIN_L;
		}
		SCLK_H;
		delay_us(1);
		data <<= 1;
		SCLK_L;			/* <----  ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
	  delay_us(1);
	}
}

/*********************************************************************************************************
*	函 数 名: ADS1256_Recive8Bit
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
uint8_t ADS1256_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;
	delay_ns(250);
	delay_ns(250);
	delay_ns(250);
	delay_ns(250);
	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for (i = 0; i < 8; i++)
	{
		SCLK_H;
		delay_ns(250);
		delay_ns(250);
		read = read<<1;
		SCLK_L;
		if (DO_IS_H)
		{
			read++;
		}
		delay_ns(250);
		delay_ns(250);
	}
	return read;

}

/*********************************************************************************************************
*	函 数 名: ADS1256_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_WriteReg(uint8_t RegID, uint8_t RegValue)
{
	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_WREG | RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);		/* 寄存器个数 - 1, 此处写1个寄存器 */
	ADS1256_Send8Bit(RegValue);	/* 发送寄存器值 */
	CS_H;	/* SPI片选 = 1 */
}

/*********************************************************************************************************
*	函 数 名: ADS1256_ReadReg
*	功能说明: 读指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。
*	返 回 值: 读到的寄存器值。
*********************************************************************************************************/
uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_RREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);	/* 寄存器个数 - 1, 此处读1个寄存器 */
	delay_us(1); 	/* 必须延迟才能读取芯片返回数据 */
	read = ADS1256_Recive8Bit();	/* 读寄存器值 */
	CS_H;	/* SPI片选 = 1 */

	return read;
}

/*********************************************************************************************************
*	函 数 名: ADS1256_WriteCmd
*	功能说明: 发送单字节命令
*	形    参:  _cmd : 命令
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_WriteCmd(uint8_t cmd)
{
	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(cmd);
	CS_H;	/* SPI片选 = 1 */
}

//-----------------------------------------------------------------
// void ADS1256_CfgADC(unsigned char gain,unsigned char drate)
//-----------------------------------------------------------------
// 函数功能: 配置ADC参数，增益及转换速率
// 入口参数: 增益，速率
// 返回参数: 无
// 全局变量: 无
// 调用模块: void ADS1256_WriteCmd(unsigned char cmd)
//         void ADS1256_WriteReg(unsigned char Reg_ID,unsigned char Reg_Date)
//         void ADS1256_Send8Bit(unsigned char date)
// 注意事项:
//-----------------------------------------------------------------
void ADS1256_CfgADC(uint8_t gain,uint8_t drate)   // 初始化设置，设置增益以及转换速率
{
    ADS1256_Init();
	ADS1256_WriteCmd(CMD_RESET);                // 写复位指令
	ADS1256_WriteReg(REG_STATUS,0XF4);          // 写状态，数据传输默认高位在前，启动矫正，禁止使用缓冲
	ADS1256_WriteCmd(CMD_SELFCAL);              // 自校准
	delay_us(20);
	{
		uint8_t buf[4];
		/* 状态寄存器定义
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.

			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.

			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled

			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.

			ACAL=1使能自校准功能。当 PGA，BUFEEN, DRATE改变时会启动自校准
		*/
		buf[0] = (0 << 3) | (1 << 2) | (1 << 1);

		/* 高四位0表示AINP接 AIN0,  低四位8表示 AINN 固定接 AINCOM */
		buf[1] = 0x08;                      //通道设置选择

		/*	ADCON: A/D Control Register (Address 02h)
			Bit 7 Reserved, always 0 (Read Only)
			Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
				00 = Clock Out OFF
				01 = Clock Out Frequency = fCLKIN (default)
				10 = Clock Out Frequency = fCLKIN/2
				11 = Clock Out Frequency = fCLKIN/4
				When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

			Bits 4-2 SDCS1, SCDS0: Sensor Detect Current Sources
				00 = Sensor Detect OFF (default)
				01 = Sensor Detect Current = 0.5 μ A
				10 = Sensor Detect Current = 2 μ A
				11 = Sensor Detect Current = 10μ A
				The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
				ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

			Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
				000 = 1 (default)
				001 = 2
				010 = 4
				011 = 8
				100 = 16
				101 = 32
				110 = 64
				111 = 64
		*/
		buf[2] = (0 << 5) | (0 << 3) | (gain << 0);

		buf[3] = drate;	// DRATE_10SPS;	/* 选择数据输出速率 */
		CS_L;
		ADS1256_Send8Bit(CMD_WREG|0);          // 写寄存器
		ADS1256_Send8Bit(0x03);                // 连续写入4个数据
		ADS1256_Send8Bit(buf[0]);              // 设置状态寄存器
		ADS1256_Send8Bit(buf[1]);              // 设置输入通道参数
		ADS1256_Send8Bit(buf[2]);              // 设置ADCON控制寄存器，增益
		ADS1256_Send8Bit(buf[3]);	             // 设置数据速率
		CS_H;
	}
	delay_us(50);
}

//-----------------------------------------------------------------
// unsigned long ADS1256_GetAdc(unsigned char channel)
//-----------------------------------------------------------------
// 函数功能: 获取ADC的采样结果
// 入口参数: 无
// 返回参数: ADC采样结果
// 全局变量: 无
// 调用模块: void ADS1256_WriteCmd(unsigned char cmd)
//        void ADS1256_WriteReg(unsigned char Reg_ID,unsigned char Reg_Date)
//        void ADS1256_Send8Bit(unsigned char date)
// 注意事项:
//-----------------------------------------------------------------
uint32_t ADS1256_GetAdc(uint8_t channel)
{
	uint32_t read;
	read = 0;

// 	while(DRDY);           //当DRDY变为低电平时，数据开始传输
	ADS1256_WriteReg(REG_MUX,channel);       // 写入读取的通道
	ADS1256_WriteCmd(CMD_SYNC);              //同步A/D转换命令
// 	Delay_1us(1);
	ADS1256_WriteCmd(CMD_WAKEUP);            //完成SYNC并退出待机模式
	while(DRDY_IS_H){};                             // 等待数据转换完成

	CS_L;                                    //片选拉低
	ADS1256_Send8Bit(CMD_RDATA);             //读取数据命令

	delay_us(1);

	//连续接收3个数据，高字节在前
	read = ((uint32_t)ADS1256_Recive8Bit() << 16);
	read +=( (uint32_t)ADS1256_Recive8Bit() << 8);
	read += ADS1256_Recive8Bit() ;
	CS_H;
    return read;
//	  read = read ^ 0x800000;
//    return (((0.596047*read)-5000000)/1000000);  // 测量出来是以v作单位

}

/* 使用流程
    ADS1256_CfgADC(PGA_1, DATARATE_100);	//增益1:1, 数据100Hz
    adc = ADS1256_GetAdc(AIN0);	 	// 从全局缓冲区读取采样结果。 采样结果是在中断服务程序中读取的
    volt = adc * AD24_mV_Scale;
*/



