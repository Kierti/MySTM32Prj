/*
 * AT24CXX.c
 *
 *  Created on: Jul 11, 2022
 *      Author: ��־��
 */

#include "AT24CXX.h"

/*!
 *  \brief  AT24CXX(EEPROMоƬ)��ȡһ���ֽ�
 *  \param  ReadAddr ��ȡ�ĵ�ַ
 *  \retval ��ȡ��������
 */
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t pBuffer;

	HAL_I2C_Mem_Read(&at24cxx, Read_ADDR, ReadAddr, I2C_MEMADD_SIZE_8BIT, &pBuffer, 1, HAL_MAX_DELAY);

	return pBuffer;
}

/*!
 *  \brief  AT24CXX(EEPROMоƬ)д��һ���ֽ�
 *  \param  WriteAddr   д��ĵ�ַ
 *  \param  DataToWrite д�������
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
	uint8_t pBuffer = DataToWrite;

	if (HAL_I2C_Mem_Write(&at24cxx, Write_ADDR, WriteAddr, I2C_MEMADD_SIZE_8BIT, &pBuffer, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		//д���Ҫ�ӳ�5ms
		delay_ms(5);
		return HAL_OK;
	}
	else
		return HAL_ERROR;
}

/*!
 *  \brief  AT24CXX(EEPROMоƬ)����ָ�������ֽ�
 *  \param  ReadAddr    ��ʼ�����ĵ�ַ
 *  \param  pBuffer     �����Ļ����ַ
 *  \param  NumToRead   Ҫ�������ݵĸ���
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
	if (HAL_I2C_Mem_Read(&at24cxx, Read_ADDR, ReadAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, NumToRead, HAL_MAX_DELAY) == HAL_OK)
		return HAL_OK;
	else
		return HAL_ERROR;
}

/*!
 *  \brief  AT24CXX(EEPROMоƬ)д��ָ�������ֽ�
 *  \param  WriteAddr   ��ʼд������ĵ�ַ
 *  \param  pBuffer     д������ݵ�ַ
 *  \param  NumToWrite  Ҫд�����ݵĸ���
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
	uint16_t i;
	for (i = 0; i < NumToWrite; i++)
	{
		if (HAL_I2C_Mem_Write(&at24cxx, Write_ADDR, WriteAddr + i, I2C_MEMADD_SIZE_8BIT, pBuffer + i, 1, HAL_MAX_DELAY) == HAL_OK)
		{
			//д���Ҫ�ӳ�5ms
			delay_ms(5);
			continue;
		}
		else
			return HAL_ERROR;
	}
	return HAL_OK;
}

