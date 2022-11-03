/*
 * MLT_BT05.h
 *
 *  Created on: Mar 6, 2022
 *      Author: ��־��
 */

#ifndef DRIVER_MLT_BT05_MLT_BT05_H_
#define DRIVER_MLT_BT05_MLT_BT05_H_

#include "tim.h"
#include "usart.h"
#include "stdbool.h"
#define RxBufferSize 2048	//���ջ����С
/***********MLT_BT05�������Ͷ���***********/
/*MLT_BT05��ǰ����ģʽ*/
typedef enum{
	NOINIT,		//δ��ʼ��
	MASTER,		//��͸��
	SLAVE,		//��͸��
	LOOPBACK,	//�ػ�
	DISCONNECT	//�Ͽ�����
}mode_set;
/*����ִ�е�״̬ */
typedef enum{
	Idle,			//����
	Busy,			//��æ
	Sent			//������ϣ��ȴ�����
}uart_state_set;
/* ����ִ�еĽ�� */
typedef enum{
		Fail,        //ʧ��
		Success,     //�ɹ�
		Timeout      //��ʱ
}result;
typedef struct{
	UART_HandleTypeDef *huart;			//MLT_BT05���õĴ���
	DMA_HandleTypeDef *dma_rx;			//���ڴ��ڽ��յ�DMA
	TIM_HandleTypeDef *tim;				//���ڼ�¼�Ƿ�ʱ�Ķ�ʱ��
	mode_set mode;                 		//ESP8266�Ĺ���ģʽ
	uart_state_set uart_state;    		//��¼����ͨ�ŵĴ��ڵ�״̬
	char RxBuffer[RxBufferSize]; 		//���ջ���
	char AckBuffer[RxBufferSize]; 		//�����Ӧ���ݵĻ���
	result (*send)(char *str, const char* response, uint16_t timeout);
	void (*sendNoAck)(uint8_t *str);
	result (*sendCmd)(const char *str, const char* response, uint16_t timeout);
//	void (*quitTrans)(void);
	void (*RxCallback)(void);
	void (*setTimeout)(uint16_t ms);
	bool (*isTimeout)(void);
	void (*delay)(uint16_t ms);
	bool (*isResponded)(const char* response);
	result (*init)(void);
}MLT_BT05;

/*ESP8266�ṹ�������ʼ��*/
void bleInit(void);
/* �������� */
result send(char *str, const char* response, uint16_t timeout);
/* ��������(��Ҫ����Ӧ) */
void sendNoAck(uint8_t *str);
/* ����ָ�� */
result sendCmd(const char *str, const char* response, uint16_t timeout);
///* �˳�͸��ģʽ */
//void quitTrans(void);
/* ���ջص����������Լ���ӣ� */
void RxCallback(void);
/* ���ó�ʱʱ�� */
void setTimeout(uint16_t ms);
/* �ж��Ƿ�ʱ */
bool isTimeout(void);
/* �ӳٺ��� */
void delay(uint16_t ms);
/* �ж��Ƿ���Ӧ */
bool isResponded(const char* response);
/* ��ʼ�� */
result init(void);
/* �മ���ض������ */
void print(UART_HandleTypeDef* huart, const char* buf, ...);

#endif /* DRIVER_MLT_BT05_MLT_BT05_H_ */
