/*
 * algorithm.h
 *
 *  Created on: Jul 5, 2022
 *      Author: ��־��
 */

#ifndef INC_ALGORITHM_H_
#define INC_ALGORITHM_H_

#include "main.h"
#include "base.h"
#include "arm_math.h"

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

#define AD_Size 1024
#define AD_Size1 300
#define AD_Size2 1200
#define AD_Size3 3000

/*  ���Ҳ���  */
typedef struct _SinWave_T   //��������ṹ��
{
    float amp;      //���ֵ mv���迹ƥ��֮��
    float freq;     //Ƶ�� Hz
    float phase;     //��λ o
	float period;   //���� us
}SinWave_T;
extern SinWave_T tSinwave;

/*  ���β����ṹ��  */
typedef struct _ArrayParam_T   //��������ṹ��
{
    float max;      //���ֵ
    float min;      //��Сֵ
    float tft_len;  //����һ������ռ��������
    float tft_cycle;
    float Vpp;   //���ֵ mV
    float Period;//����   us
    float Aver;  //ƽ��ֵ mv
    float Rms;   //��Чֵ mv
    float Fre;
    float rmsDivideVpp;
}ArrayParam_T;
extern ArrayParam_T tAD_Params;
extern ArrayParam_T tAD_Params1;
void Calc_array(ArrayParam_T *params,float* calc_addr,u32 calc_len);    //��������Ĳ���
void Calc_array1(ArrayParam_T *params,float* calc_addr,u32 calc_len);
int * __sinWaveData(int points,int amplitude,int mid_value,int * _sinWave);

/*  ����Ҷ�任  */
extern float
TFT_arr1[2][AD_Size1],
*pTFT_arr1, *pTFT_arr_end1,

TFT_arr2[2][AD_Size2],
*pTFT_arr2, *pTFT_arr_end2,

TFT_arr3[2][AD_Size3],
*pTFT_arr3, *pTFT_arr_end3
;


void calc_FFT(float*Input,float*Output);
void PowerPhaseRadians_f32(float32_t *_ptr, float32_t *_phase, uint16_t _usFFTPoints, float32_t _uiCmpValue);
void cal_fftPhase(float*Input,float*Output);

/*  AD����  */

#define AD_mV_Scale 0.3125f
#define AD_V_Scale 0.0003125f

#define AD_mV_Scale_new 0.3023317334945769f
#define AD_V_Scale_new 0.0003023317334945769f

#define ADS8688_ZERO    32767

#define AD24_mV_Scale 0.0005960464832810452
#define AD24_V_Scale  0.0000005960464832810452

extern float
AD_arr1[2][AD_Size1],
*pAD_arr1,*pAD_arr_end1,		//��ͨ�������ɲ��ö�ά����

AD_arr2[2][AD_Size2],
*pAD_arr2,*pAD_arr_end2,

AD_arr3[2][AD_Size3],
*pAD_arr3,*pAD_arr_end3,

AD_sum_arr[AD_Size],
*pAD_sum_arr,*pAD_sum_arr_end;

void ADarr_Init(void);
//void get_AD_Results(void);


/* ���ݾ��� */
#define CHOICE_LEN 5
#define THRESHOLD  0.01f
#define ROW     20   //���֧��20·�˲�

#define AVER    0   //ƽ��ֵ
#define MANY    1   //����
#define MID     2   //��λ��
#define SMOOTH  3   //ƽ�����
#define MAX     4   //���ֵ
#define MIN     5   //��Сֵ
#define NON     6   //�޲���

float many_in_array(float *p);						//��������
void Sort(float *a, u8 n);							//����
float is_Smooth(float* arr,u32 len);				//�ж�ƽ��
float Make_Choices(float choice,u16 row,u16 mode);	//��·����
void quickSort(float *arr, int star, int end);
void swap(float *left, float *right);

/* ���������� */
double bessi0(double x);
double bessi1(double x);
double bessi(int n,double x);


/* FIR�˲� */
//#define FIR_NUM     AD_Size          	//FIR����
//#define BLOCK_SIZE  128              	//���㵥��FIR
//#define FIR_ORDER   64               	//�˲�������
//#define FIR_SHIFT   FIR_ORDER/2			//FIRȺ��ʱ��λ
//#define FIR_LEN     FIR_ORDER+1         //�˲���ϵ������
//#define NUMBLOCKS   FIR_NUM/BLOCK_SIZE  //��Ҫ����arm_fir_f32�Ĵ���
//extern uint16_t FIR_Shift;              //��λ������һ��|FIRȺ��ʱ��λ
//uint16_t calc_FIR_W(uint32_t Ts,uint32_t Tc);
//void calc_FIR(float *input,uint16_t w);
//void FIR_calc(float *input);
#endif /* INC_ALGORITHM_H_ */
