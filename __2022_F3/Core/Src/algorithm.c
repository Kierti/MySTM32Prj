/*
 * algorithm.c
 *
 *  Created on: Jul 5, 2022
 *      Author: ��־��
 */

#include "algorithm.h"
#include "arm_math.h"
#include "math.h"
#include "tim.h"
#include "fir.h"

/*  ����Ҷ�任  */
#define FFT_SIZE AD_Size
static float FFT_Buffer[FFT_SIZE];
/*
 * @brief ����FFT���ֵ
 */
void calc_FFT(float*Input,float*Output)
{
    //FIR_calc(Input);
    arm_rfft_fast_instance_f32 S;//�ṹ��
    arm_rfft_fast_init_f32(&S,FFT_SIZE);//��ʼ���ýṹ��
    arm_rfft_fast_f32(&S, Input, FFT_Buffer, 0);//ifft_flag=0�����任�� Ϊ1������任
    arm_cmplx_mag_f32(FFT_Buffer, Output, FFT_SIZE);
    arm_scale_f32(Output,2.0f/FFT_SIZE,Output,FFT_SIZE);    //�������ʵV
    Output[0] *= 0.5f;
}

/*
* @name PowerPhaseRadians_f32
* @function: ����λ
* @param _ptr  ��λ��ַ����ʵ�����鲿
*        _phase �����λ����λ�Ƕ��ƣ���Χ(-180, 180]
*        _usFFTPoints  ����������ÿ������������float32_t��ֵ
*        _uiCmpValue  �Ƚ�ֵ����Ҫ�����λ����ֵ
* @retval ��
*/
void PowerPhaseRadians_f32(float32_t *_ptr, float32_t *_phase, uint16_t _usFFTPoints, float32_t _uiCmpValue)
{
	float32_t lX, lY;
	uint16_t i;
	float32_t phase;
	float32_t mag;

	for (i=0; i < _usFFTPoints; i++)
	{
		lX= _ptr[2*i];  	  /* ʵ�� */
		lY= _ptr[2*i + 1];    /* �鲿 */

 		phase = atan2f(lY, lX);    		  				 /* atan2���Ľ����Χ��(-pi, pi], ������ */
		arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* ��ģ */

//		if(_uiCmpValue < mag)
//		{
//			_phase[i] = 0;
//		}
//		else
//		{
			_phase[i] = phase* 180.0f/3.1415926f;   /* �����Ľ���ɻ���ת��Ϊ�Ƕ� */
//		}
	}
}
/*
 * @brief ����FFT����λ
 */
void cal_fftPhase(float*Input,float*Output){
	arm_rfft_fast_instance_f32 S;
	arm_rfft_fast_init_f32(&S,FFT_SIZE);	//��ʼ���ýṹ��
	arm_rfft_fast_f32(&S,Input,FFT_Buffer,0);//ifft_flag=0�����任�� Ϊ1������任
	PowerPhaseRadians_f32(FFT_Buffer,Output,FFT_SIZE,500);
}



/*  AD����  */
ArrayParam_T tAD_Params;
float
AD_arr[AD_Size] = {0,},
*pAD_arr, *pAD_arr_end,

AD_sum_arr[AD_Size] = {0,},
*pAD_sum_arr, *pAD_sum_arr_end
;

u8
TFT_arr[AD_Size] = {0,},
*pTFT_arr, *pTFT_arr_end
;

void ADarr_Init(void)
{
    pAD_arr = AD_arr;
    pAD_arr_end = AD_arr + AD_Size;
    arm_fill_f32(0,AD_arr,AD_Size);
}

void get_AD_Results(void)
{
    pAD_arr=AD_arr;pAD_arr_end=AD_arr+AD_Size;
    HAL_TIM_Base_Start_IT(&htim3);
    while(pAD_arr!=pAD_arr_end){};
    HAL_TIM_Base_Stop(&htim3);
}
/* ���ڲ���������ƽ��ֵ
float get_AD_Results()
{
    pAD_arr=AD_arr;pAD_arr_end=AD_arr+AD_N;
    HAL_TIM_Base_Start_IT(&htim3);
    while(pAD_arr!=pAD_arr_end){};
    HAL_TIM_Base_Stop(&htim3);
    Calc_array(&AD_Params,AD_arr,AD_N);
    return AD_Params.Aver;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim3)
    {
        if(pAD_arr < pAD_arr_end)
        {
            *pAD_arr++ = (get_ADS_ch1()-ADS8688_ZERO)*AD_mV_Scale;
        }
    }
}
*/

/*
 * @brief ���㲨������Ĳ���
 */
void Calc_array(ArrayParam_T *params,float* calc_addr,u32 calc_len)
{
	uint32_t useless;
    //arm_offset_f32(calc_addr,-32768,calc_addr,calc_len);        		//ȥ��ԭʼ���ݵ�ֱ��ƫ��
    //arm_scale_f32(calc_addr,AD_mV_Scale,calc_addr,calc_len);    		//�������ʵV
    //FIR_calc(calc_addr); calc_addr += FIR_Shift;  calc_len = FIR_NUM-FIR_Shift;
    arm_max_f32(calc_addr,calc_len,&params->max,&useless);  			//�������ֵ
    arm_min_f32(calc_addr,calc_len,&params->min,&useless);  			//������Сֵ
    params->Vpp=params->max-params->min;                   				//��������ֵ
    arm_mean_f32(calc_addr,calc_len,&params->Aver);    					//����ƽ��ֵ
    arm_rms_f32(calc_addr,calc_len,&params->Rms);      					//������Чֵ
}

/**
 * @brief �������Ҳ�����
 * @param points:���Ҳ������
 * 		  amplitude����ֵ
 * 		  mid_value����ֵ
 * 		  _sinWave�����Ҳ�������/ָ��
 * @retval _sinWave ���Ҳ�������ָ��
 */
int * __sinWaveData(int points,int amplitude,int mid_value,int * _sinWave)
{
    int index = 0, halfPoint = 0;
    float radian = 0.0;        	//����
    float peak = 0.0;       	//��ֵ
    int tem = 0;
    halfPoint = points / 2;			//ˮƽ��λ�� ��Ƭ��û�и���ѹ ˮƽ��Ϊ��ֵ������һ��
    radian = PI / halfPoint;        	// ��/2 ��ÿһ�����Ӧ�Ļ���ֵ
    for( index = points/4; index < points+points/4; index++ )
    {
    	peak = amplitude * sin( radian * index + PI / 2 + PI ) + mid_value;
        tem = (int )(peak * 1.0);
        _sinWave[index - points/4] = tem;
    }
    return _sinWave;
}

/* ���ݾ��� */
float many_in_array(float *p)   //���������е�����
{
	u32 max=0,most = 0;
	u32 i = 0, j = 0;
	u32 cnt_arr[CHOICE_LEN] = { 0, };
	for (i = 0; i < CHOICE_LEN; i++)
	{
		for (j = i; j < CHOICE_LEN; j++)
		{
			if (p[i] == p[j])
				cnt_arr[i]++;
		}
	}
	for (i = 0; i < CHOICE_LEN; i++)
	{
		if (max < cnt_arr[i])
		{
			max = cnt_arr[i];
			most = i;
		}
	}
	return p[most];
}
void Sort(float *a, u8 n)   //����nλ�����������������
{
	u32 i, j, imin;
    float s;
	for (i = 0; i < n - 1; i++)
	{
		imin = i;
		for (j = i + 1; j<n; j++)
            if (a[imin]>a[j]) imin = j;
		if (i != imin){
			s = a[i]; a[i] = a[imin]; a[imin] = s;
		}
	}
}
/*
�жϵ�ǰ�����Ƿ��϶�Ϊƽ��:���ֵ\��Сֵ��δ������ֵ
��С����ֵ����ƽ��ֵ,�϶��������������Ѿ�ƽ��
*/
float is_Smooth(float* arr,u32 len)
{
    float result,max,min,useless;
    arm_mean_f32(arr,len,&result);
    arm_max_f32(arr,len,&max,(u32*)&useless);  //�������ֵ
    arm_min_f32(arr,len,&min,(u32*)&useless);  //������Сֵ
    if(\
        ( (max-result)/result<THRESHOLD )\
      &&( (result-min)/result<THRESHOLD )\
    ) return result;
    else return 0;
}
/*
֧�ֶ�·����
*/
float Make_Choices(float choice,u16 row,u16 mode)
{
    /* ����/��̬������ʼ�� */
    static float Choices[ROW][CHOICE_LEN]={0,};
    static float temp_arr[CHOICE_LEN]={0,};
    static float good_choice=0;
	static u16 i[ROW]={0,};
    float result=0,useless;

    /* ѭ��ˢ�»��� */
	Choices[row][i[row]]=choice;
	if(++i[row]==CHOICE_LEN)i[row]=0;

    /* �˲��������� */
    if(mode == AVER)    /*��ȥ���ֵ\��Сֵ��ƽ��ֵ*/
    {
        float max,min;
        arm_mean_f32(Choices[row],CHOICE_LEN,&result);			   //����ƽ��ֵ
        arm_max_f32(Choices[row],CHOICE_LEN,&max,(u32*)&useless);  //�������ֵ
        arm_min_f32(Choices[row],CHOICE_LEN,&min,(u32*)&useless);  //������Сֵ
        result = (result*CHOICE_LEN-max-min)/(CHOICE_LEN-2);
    }
    else if(mode == MANY)   /* �õ����� */
    {
        return many_in_array(Choices[row]);
    }
    else if(mode == MID)   /* �õ���λ�� */
    {
        arm_copy_f32(Choices[row], temp_arr,CHOICE_LEN);
        Sort(temp_arr,CHOICE_LEN);
        return temp_arr[CHOICE_LEN/2];
    }
    else if(mode == SMOOTH)   /* �ж�ƽ��?��������:��ʾ��ƽ������ */
    {
        result=is_Smooth(Choices[row],CHOICE_LEN);
        if (result>0) //���ƽ��,������ʾ
        {
            good_choice=result;
        }
        return good_choice;
    }
    else if(mode == MAX)   /* �õ����ֵ */
    {
        arm_max_f32(Choices[row],CHOICE_LEN,&result,(u32*)&useless);  //�������ֵ
        return result;
    }
    else if(mode == MIN)   /* �õ���Сֵ */
    {
        arm_min_f32(Choices[row],CHOICE_LEN,&result,(u32*)&useless);  //������Сֵ
        return result;
    }
    else if(mode == NON)   /* �����κβ���,���� 0 */
    {
        return 0;
    }
	return result;
}

void quickSort(float *arr, int star, int end)
{
	int mid;//��׼
	int left = star;
	int right = end;
	mid = arr[(left+right)/2];
	if(left > right)
	{
		return;
	}
	else
	{
		while(left <= right)
		{
			while(arr[left] > mid)//����ȡ���ںţ���Ϊ������ʹ�������������mid��ʱ�򲻻��˳��������ٴ��ƶ�һ����˳� ������ʹ������mid�����λ��
		{
			left++;
		}
		while(arr[right] < mid)//ͬ����ȡ�Ⱥţ�����������λ�Ƶ�mid�����ʱ�����ƶ���
		{
			right--;//4 5 8 9 6 3 5 8 5 8
		}

		if(left <= right)
		//�������Ӧ���� �����������ʹ �� ��Ϊ����˵�ˣ�������󶼻�λ�Ƶ�mid���λ���ϣ���������±�ҪôС���ұߣ�Ҫô���ڣ������������ȡ�Ⱥ�
		{
			swap(&arr[left], &arr[right]);//�������±�С���ұ��±��򽻻�
			left++;
			right--;
		}

		}
	}

	if(star < right) quickSort(arr, star, right);//��Ϊ�ո�right--�ˣ��������ƶ���һ�����������right��ʵ�Ѿ�С��left�ˣ������������right
	/*��������� ��ȡ�Ⱥŵ�ԭ������return���������û��ȡ�Ⱥţ�
	�����ڵ��ڵ�ʱ�򲻻᷵�أ������ڴ���ȥ��ʱ��Ͳ���ȡ�Ⱥţ�
	�Ͳ��ᵼ����Ӧ�ý�����ʱ�򲻻�return����
	*/
	if(left < end) quickSort(arr, left, end);//ͬ����Ϊ left�Ѿ�����right�ˣ��������ұ���left
	//���ұ�����

}

void swap(float *left, float *right)
{
	float t;
	t = *left;
	*left = *right;
	*right = t;
}

/* ���������� */
/* PURPOSE: Evaluate modified Bessel function In(x) and n=0. */
double bessi0(double x)
{

	double ax, ans;
	double y;

	if ((ax = fabs(x)) < 3.75) {
		y = x / 3.75, y = y * y;
		ans = 1.0 + y * (3.5156229 + y * (3.0899424 + y * (1.2067492
			+ y * (0.2659732 + y * (0.360768e-1 + y * 0.45813e-2)))));
	}
	else {
		y = 3.75 / ax;
		ans = (exp(ax) / sqrt(ax))*(0.39894228 + y * (0.1328592e-1
			+ y * (0.225319e-2 + y * (-0.157565e-2 + y * (0.916281e-2
				+ y * (-0.2057706e-1 + y * (0.2635537e-1 + y * (-0.1647633e-1
					+ y * 0.392377e-2))))))));
	}
	return ans;
}

/* PURPOSE: Evaluate modified Bessel function In(x) and n=1. */
double bessi1(double x)
{
	double ax, ans;
	double y;

	if ((ax = fabs(x)) < 3.75) {
		y = x / 3.75, y = y * y;
		ans = ax * (0.5 + y * (0.87890594 + y * (0.51498869 + y * (0.15084934
			+ y * (0.2658733e-1 + y * (0.301532e-2 + y * 0.32411e-3))))));
	}
	else {
		y = 3.75 / ax;
		ans = 0.2282967e-1 + y * (-0.2895312e-1 + y * (0.1787654e-1
			- y * 0.420059e-2));
		ans = 0.39894228 + y * (-0.3988024e-1 + y * (-0.362018e-2
			+ y * (0.163801e-2 + y * (-0.1031555e-1 + y * ans))));
		ans *= (exp(ax) / sqrt(ax));
	}
	return x < 0.0 ? -ans : ans;
}

/* PURPOSE: Evaluate modified Bessel function In(x) for n >= 0*/
double bessi(int n, double x)
{
	float ACC = 40.0;
	float BIGNO = 1.0e10;
	float BIGNI = 1.0e-10;

	int j;
	double bi, bim, bip, tox, ans;

	if (n < 0)
	{
		return -INFINITY;
	}
	if (n == 0)
		return(bessi0(x));
	if (n == 1)
		return(bessi1(x));


	if (x == 0.0)
		return 0.0;
	else {
		tox = 2.0 / fabs(x);
		bip = ans = 0.0;
		bi = 1.0;
		for (j = 2 * (n + (int)sqrt(ACC*n)); j > 0; j--) {
			bim = bip + j * tox*bi;
			bip = bi;
			bi = bim;
			if (fabs(bi) > BIGNO) {
				ans *= BIGNI;
				bi *= BIGNI;
				bip *= BIGNI;
			}
			if (j == n) ans = bip;
		}
		ans *= bessi0(x) / bi;
		return  x < 0.0 && n % 2 == 1 ? -ans : ans;
	}
}



/* FIR�˲��� */
//uint16_t FIR_Shift=FIR_SHIFT ;                  //��λ������һ��
//uint16_t calc_FIR_W(uint32_t Ts,uint32_t Tc)   //�������ڡ������ź�����
//{
//    uint16_t w;
//    w=20*Ts/Tc+2;
//    if(w>9)return 9;
//    else return w;
//}   //����ֵ��1-9֮��
//static float FIR_State[BLOCK_SIZE+FIR_ORDER] ;   //FIR�˲���״̬����
//static arm_fir_instance_f32 FIR_Sta; //����ṹ��S
//void calc_FIR(float *input,uint16_t w) //�ɿ��ƽ�ֹƵ�ʵ�FIR
//{
//    arm_fir_init_f32(&FIR_Sta, FIR_LEN, (float*)LowPassFIR[w-1], FIR_State, BLOCK_SIZE); //��ʼ���ṹ��
//    for(u16 i=0;i<NUMBLOCKS;i++)
//    {
//        arm_fir_f32(&FIR_Sta, input + (i * BLOCK_SIZE), input + (i * BLOCK_SIZE),BLOCK_SIZE);
//    }
//}
//void FIR_calc(float *input) //�̶���ֹƵ�ʵ�FIR
//{
//    u16 i;
//    float FIR_State[BLOCK_SIZE+FIR_ORDER] ;   //FIR�˲���״̬����
//    arm_fir_instance_f32 FIR_Sta; //����ṹ��S
//    arm_fir_init_f32(&FIR_Sta, FIR_LEN, (float*)LowPass64, FIR_State, BLOCK_SIZE); //��ʼ���ṹ��
//    for(i=0;i<NUMBLOCKS;i++)
//    {
//        arm_fir_f32(&FIR_Sta, input + (i * BLOCK_SIZE), input + (i * BLOCK_SIZE),BLOCK_SIZE);
//    }
//    for(i=0;i<AD_Size;i++) //������λ
//    {
//        input[i] = input[i+FIR_SHIFT];
//    }
//    for(i=AD_Size;i<AD_Size+FIR_SHIFT;i++) //������λ
//    {
//        input[i] = 0;
//    }
//}
