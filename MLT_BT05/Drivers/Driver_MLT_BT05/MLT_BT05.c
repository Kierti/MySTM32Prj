/*
 * MLT_BT05.c
 *
 *  Created on: Mar 6, 2022
 *      Author: ��־��
 */
#include "MLT_BT05.h"
#include "usart.h"
#include "tim.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdarg.h"
#include "main.h"

extern DMA_HandleTypeDef hdma_usart6_rx;

/**
 * @brief MLT_BT05�ṹ�������ʼ��
 * @retval None
 */
MLT_BT05 ble;
void bleInit(void){
	ble.huart=&huart6;
	ble.dma_rx=&hdma_usart6_rx;
	ble.tim=&htim6;
	ble.send=send;
	ble.sendNoAck=sendNoAck;
	ble.sendCmd=sendCmd;
//	ble.quitTrans=quitTrans;
	ble.RxCallback=RxCallback;
	ble.setTimeout=setTimeout;
	ble.isTimeout=isTimeout;
	ble.delay=delay;
	ble.isResponded=isResponded;
	ble.init=init;
}

/**
 * @brief  �മ���ض���
 * @retval None
 **/
void print(UART_HandleTypeDef* huart, const char* buf, ...)
{
  char str[RxBufferSize] = {0};
  va_list v;
  va_start(v, buf);
  vsprintf(str, buf, v); 	//ʹ�ÿɱ�������ַ�����ӡ������sprintf
  HAL_UART_Transmit(huart,(uint8_t*)str,strlen(str),0xffffffff);
  va_end(v);
}

/**
 * @brief  MLT_BT05 ���ڽ��ջص�����
 * @retval None
 **/
void RxCallback(){
	UART_HandleTypeDef*  huart = ble.huart; //MLT_BT05�õ��Ĵ���

	/* �ж��Ƿ��������ж� */
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
		__HAL_UART_CLEAR_IDLEFLAG(huart); //���IDLE�жϱ�־λ
		HAL_UART_DMAStop(huart);          // ֹͣDMA����

		//���²����ǽ����յ������ݴ��� AckBuffer ��
		int recv_end    = RxBufferSize - __HAL_DMA_GET_COUNTER(ble.dma_rx); // �õ����յ����ַ�����
		int recv_start  = recv_end % RxBufferSize;                      // ���յ������ݵ���ʼ����
		while(ble.RxBuffer[recv_start] == 0) { //�ҳ����յ���ʼ
			recv_start = (recv_start + 1) % RxBufferSize;
		}
		int index = recv_start;
		int i;
		for(i=0; index != recv_end; i++)
		{
			ble.AckBuffer[i] = ble.RxBuffer[index];
			ble.RxBuffer[index] = 0;
			index = (index+1)%RxBufferSize;
		}
		ble.AckBuffer[i] = 0; //�����ַ����ָ���

		//�����ã������յ�����Ӧͨ���������
		print(&huart3,"\r\n����:\r\n");
		print(&huart3,"%s",ble.AckBuffer);
		//ʹ�ö���������
		if(ble.uart_state == Sent) {	//������ڷ�����ϣ��ȴ�����
			ble.uart_state = Idle; 	//��Ϊ����ģʽ
		}
		if(ble.uart_state == Idle) { //��һ���Ǳ����ؽ��յ���Ϣ��
		//for(;;); //TODO:
			if(!strcmp(ble.AckBuffer, "LED")) {
				print(&huart3,"\r\n�Ʒ�ת\r\n");
				HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
			};
		}

		/* ׼����һ��DMA���� */
		__HAL_DMA_SET_COUNTER(ble.dma_rx, 0);
		HAL_UART_Receive_DMA(huart, (uint8_t*)ble.RxBuffer, RxBufferSize);
	}
}

/**
 * @brief  ESP8266 ���÷���ָ������ݺ����ĵȴ���ʱʱ��
 * @note ms���ܳ���43,690
 * @retval None
 **/
void setTimeout(uint16_t ms){
	__HAL_TIM_SET_AUTORELOAD(ble.tim, ms*3/2);  //Ҫ�����ӳ�10ms���жϣ�����Ҫ���� ARR Ϊ 10*3/2;
	__HAL_TIM_SET_COUNTER(ble.tim, 1);          //��ΪҪ����CNT�Ƿ�Ϊ0���ж��Ƿ�ʱ��������������Ϊ1
	__HAL_TIM_ENABLE(ble.tim);                  //������ʱ������ʱ����������Զ���0����ֹͣ����
}

/**
 * @brief  ESP8266 �ж��Ƿ�ʱ
 * @note   ������setTimeout֮�����ʹ��
 * @retval None
 **/
bool isTimeout(void){
	if(__HAL_TIM_GET_COUNTER(ble.tim) == 0){
		return true;
	}
	else {
		return false;
	}
}
void sendNoAck(uint8_t *str) {
	print(&huart6,"%s",(char*)str);
//TODO: ������
	if(!(str[0] == '\r' && str[1] == '\n')) {
		print(&huart3,"\r\n����:\r\n");
		print(&huart3,"%s",(char*)str);
	}
}


result send(char *str,const char* response,uint16_t timeout){
	/* �������� */
	sendNoAck((uint8_t*)str);

	/* �ȴ���Ӧ */
	ble.uart_state = Sent;
	setTimeout(timeout); //���ó�ʱʱ��
	while(!isTimeout() && !isResponded(response));
	ble.uart_state = Idle;

	/* �ж�ִ�еĽ�� */
	if(isTimeout()) {
		return Timeout; //����ʧ�ܻ��߳�ʱ
	}
	else {
		return Success; //���ݳɹ�����
	}
}

result sendCmd(const char *str,const char* response,uint16_t timeout){
	/* �������� */
	sendNoAck((uint8_t*)str);
	sendNoAck((uint8_t*)"\r\n");

	/* �ȴ���Ӧ */
	ble.uart_state = Sent;
	setTimeout(timeout); //���ó�ʱʱ��
	while(!isTimeout() && !isResponded(response));
	ble.uart_state = Idle;

	/* �ж�ִ�еĽ�� */
	if(isResponded(response)) {
		return Success;
	}
	else {
		return Timeout; //����ʧ�ܻ��߳�ʱ
	}
}
/**
 * @brief  �ӳٺ���
 * @note   ͨ����ʱ��ʵ�ֵ��ӳ٣�ֻ��ʹ����quitTrans����
 * @retval None
 **/
void delay(uint16_t ms) {
	setTimeout(ms);
	while(!isTimeout());
}

/**
 * @brief  �ж��Ƿ��ESP8266�õ���������Ӧ��ĸ�����Ӵ���
 * @retval �Ƿ�õ���������Ӧ
 **/
bool isResponded(const char* response){
	int responseLen = strlen(response);
	int rxBufferLen = strlen(ble.RxBuffer);
	for(int i=0,j=0; i<rxBufferLen; i++) {
		if(ble.RxBuffer[i] == response[j])	{
			if(++j == responseLen) { //�ҵ��ִ� �����ҵ���Ӧ��
				return true;
			}
		}
		else{
			j = 0;
		}
	}
	return false; //δ�ҵ��Ӵ�
}

/**
 * @brief  MLT_BT05 ��ʼ������
 * @retval ��ʼ���Ƿ�ɹ�
 **/
result init(){
	bleInit();
	UART_HandleTypeDef*  huart = ble.huart; //ESP8266�õ��Ĵ���
	/* ����DMA���� */
	HAL_UART_Receive_DMA(huart, (uint8_t*)ble.RxBuffer, RxBufferSize);
	__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); //���������ж�

	/* ע�����ʱ��ʱ����û�п�ʼ���� */
	HAL_TIM_Base_Start(ble.tim);
	HAL_TIM_OnePulse_Init(ble.tim, TIM_OPMODE_SINGLE);                 //����Ϊ OnePulse ģʽ
	__HAL_TIM_SET_PRESCALER(ble.tim, HAL_RCC_GetHCLKFreq()/3/1000-1);  //���÷�Ƶ��Ϊ 56,000

	delay(100);
	return Success;
}
