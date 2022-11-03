/*
 * oscilloscope.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Yogurt
 */

#ifndef INC_OSCILLOSCOPE_H_
#define INC_OSCILLOSCOPE_H_

#include "main.h"

/*  ϵͳ����  */
typedef struct _Sys_T
{
    short channel;		 //ͨ��

    int levelSensitivity;	//ˮƽ������

    int verticalSensitivity0;	//ͨ��1��ֱ������
    int verticalSensitivity1;	//ͨ��2��ֱ������

    int CH1out;
    int CH2out;

    int count0;					//ˮƽ������1ms/div����
    int count1;					//ˮƽ������10ms/div����

    unsigned char waveData0[600];
    unsigned char waveData1[600];

    int freMeaNum0;				//ͨ��1Ƶ�����벶��num
    int freMeaFlag0;			//ͨ��1Ƶ�����벶���־λ

    int freMeaNum1;
    int freMeaFlag1;

    int skewingNum;				//��λ�����
    int skewingFlag;			//��λ�������־λ
    int skewingFlagTime;		//��λ�����Ƶ�ʱ�־λ
    float skewing;				//��λ��

    unsigned long triggerLevel;			//������ƽ
    unsigned long triggerStartBit;		//������ʼλ

    int triggerSwitch;					//��������
    unsigned char triggerSelect;		//����ѡ��

    unsigned char waveDataMax0;			//������ͨ��1������ʾ�������ֵ
    unsigned char waveDataMin0;			//������ͨ��1������ʾ������Сֵ
    unsigned char waveDataMax1;
    unsigned char waveDataMin1;

    int runOrStop;
    int autoSet;
    int autoSetFlag0;					//�Զ�����ͨ��1Ƶ�ʲ�����־λ
    int autoSetFlag1;					//�Զ�����ͨ��2Ƶ�ʲ�����־λ

    int zoomFlag0;
    int zoomFlag1;
}Sys_T;
extern Sys_T tSys;

void float2char(float slope,char*buffer,int n);
void myInit(void);
void sysInit(void);
void oscilloscopeFunction(void);
void autoSet(void);
void waveDataDisplay(void);
void waveAttributeDisplay(void);
void freAndSkewingDataDisplay(void);
void triggerModeAndDisplay(void);
void waveRecognition(void);

#endif /* INC_OSCILLOSCOPE_H_ */
