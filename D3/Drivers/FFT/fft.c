#include "fft.h"
#include "math.h"
#include "lcd.h"
#include "base.h"
u32 lBufInArray[NPT];
u32 lBufOutArray[NPT];
u32 lBufMagArray[NPT];
u32 lBUFPHASE[NPT];
u32 MagBufArray[NPT/2];
float PI2=6.28318530717959;
float PI=3.14159265358979;
u32 Fs=2000;              
u32 Wave;
/******************************************************************
��������:InitBufInArray()
��������:ģ��������ݣ����������а���3��Ƶ�����Ҳ�
����˵��:
��    ע:��lBufInArray�����У�ÿ�����ݵĸ�16λ�洢�������ݵ�ʵ����
          ��16λ�洢�������ݵ��鲿(����Ϊ0)
*******************************************************************/
void InitBufInArray(void)
{
    unsigned short i;
    float fx;                                       //Fn=i*Fs/NPT		//���ڴ˴�i�Ǵ�0��ʼ�ģ����Բ���Ҫ�ټ�1
    for(i=0; i<NPT; i++)                            //Ƶ�ʷֱ���Fs/1024=187.5Hz,����ÿһ��i�ʹ���187.5Hz
    {                                               //��Ϊ����Ƶ��=Fs��������Ļ�ϵ�һ�����ӵĳ��ȴ���(1/Fs)��
        fx = 1000 * sin(PI2 * i * 350.0 / Fs) +     //2pi*f*t=2pi*i*f1/Fs,��ôf=f1
             3000 * sin(PI2 * i * 8400.0 / Fs) +
             4000 * sin(PI2 * i * 10000.0 / Fs);
        lBufInArray[i] = ((signed short)fx) << 16;
    }
} 

void InitBufInArray2(void)
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 4000 * sin(PI2 * i * 375.2 / Fs) +
             1000 * sin(PI2 * i * 564.7 / Fs) +
             1500 * sin(PI2 * i * 937.1 / Fs)+1000;   //����1000ֻ��ֱ����������1000������������
        lBufInArray[i] = ((signed short)fx) << 16;
    }
}
/******************************************************************
��������:GetPowerMag()
��������:�������г����ֵ                          short �ķ�Χ����-32767 �� 32767 ��Ҳ���� -(2^15 - 1)��(2^15 - 1)��
����˵��:
������ע:�Ƚ�lBufOutArray�ֽ��ʵ��(X)���鲿(Y)��Ȼ������ֵ(sqrt(X*X+Y*Y)
*******************************************************************/
void GetPowerMag(void)
{
    signed short lX,lY;                                                  //��Ƶ�ʵĻ�Fn=i*Fs/NPT		//���ڴ˴�i�Ǵ�0��ʼ�ģ����Բ���Ҫ�ټ�1
    float X,Y,Mag;                                                      
    unsigned short i;
    for(i=0; i<NPT/2; i++)                                                  //����FFT��ÿ��Ƶ�ʵ㴦����ʵ��ֵ  A0=lBufOutArray[0]/NPT
    {                                                                       //                                 Ai=lBufOutArray[i]*2/NPT
        lX  = (lBufOutArray[i] << 16) >> 16;  //lX  = lBufOutArray[i];
        lY  = (lBufOutArray[i] >> 16);
			                                 
        X = NPT * ((float)lX) / 32768;//����32768�ٳ�65536��Ϊ�˷��ϸ������������,������
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            lBufMagArray[i] = (unsigned long)(Mag * 32768);   //0Hz��ֱ��������ֱ����������Ҫ����2
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}


void PowerPhase(u16 nfill)

{
    unsigned short i;
    signed short lX,lY;  

		for (i=0; i < NPT/2; i++)
		{
						lX= (lBufOutArray[i]<<16)>>16; /* ȡ��16bit��sine_cosine --> cos */
						lY= (lBufOutArray[i] >> 16);   /* ȡ��16bit��sine_cosine --> sin */    
						{
								float X=  NPT*((float)lX)/32768;
								float Y = NPT*((float)lY)/32768;
								float phase = atan(Y/X);
								 if (Y>=0)
								 {
										if (X>=0)
											;
										else
										 phase+=PI;  
								 }
								 else
								 {             
										if (X>=0)
											phase+=PI2;                  
										else 
											phase+=PI;                    
								 }                            
								lBUFPHASE[i] = phase*180.0/PI;
						}    
				}
}

void lcd_show_fft(unsigned int *p)
{
	unsigned int *pp = p+1;             //p+1�൱����ֱ�Ӱ�0HZ�����˵���
	unsigned int i = 0;
	for(i = 0;i<480;i++)
	{
		//�ֱ���hz
		//ÿ��С���ο��Ϊ1����ʵ����û����ʾ�����е�
    //512��ֵ��Ƶ�ʿɴﵽ Hz	
    //0.11���Ҹ�����Ļ��ʾ�߶ȵ�����һ��ֵ,Ƶ�����Ļ��ǵø����ֵ���������� 320*240��Ļ		320*=780
		LCD_Fill(0,        i, *pp*0.11, (i+1), WHITE);     //��Ч���ְ�ɫ       
		LCD_Fill(*pp*0.11, i, 270,       (i+1), BLACK);   //�����ͺ�ɫ
    pp++;
	}
	
}
/***********************************************
�����ֵ���δ�ֵ������Ӧ��Ƶ�ʣ���������
*************************************************/
void select_max(float *f,float *a)
{
	  int i;
	  float k,k1,m;
	  float aMax =0.0,aSecondMax = 0.0,aThirdMax = 0.0,aFourthMax=0.0;
      float fMax =0.0,fSecondMax = 0.0,fThirdMax = 0.0,fFourthMax=0.0;
	  int nMax=0,nSecondMax=0,nThirdMax=0,nFourthMax=0;
	  for ( i = 1; i < NPT/2; i++)//i������1����0�Ļ������ֱ�������ӽ�ȥ��������
    {
        if (a[i]>aMax)
        {
            aMax = a[i]; 
			nMax=i;
			fMax=f[nMax];
        }
    }
		for ( i=1; i < NPT/2; i++)
		{
				if (nMax == i)
				{
						continue;//����ԭ�����ֵ���±ֱ꣬�ӿ�ʼi+1��ѭ��
				}
        if (a[i]>aSecondMax&&a[i]>a[i+1]&&a[i]>a[i-1])
        {
            aSecondMax = a[i]; 
					  nSecondMax=i;
					  fSecondMax=f[nSecondMax];
        }
		}
		for ( i=1; i < NPT/2; i++)
    {
				if (nMax == i||nSecondMax==i)
				{
						continue;//����ԭ�����ֵ���±ֱ꣬�ӿ�ʼi+1��ѭ��
				}
        if (a[i]>aThirdMax&&a[i]>a[i+1]&&a[i]>a[i-1])
        {
            aThirdMax = a[i]; 
					  nThirdMax=i;
					  fThirdMax=f[nThirdMax];
        }
    }
		for ( i=1; i < NPT/2; i++)
    {
				if (nMax == i||nSecondMax==i||nThirdMax==i)
				{
						continue;//����ԭ�����ֵ���±ֱ꣬�ӿ�ʼi+1��ѭ��
				}
        if (a[i]>aFourthMax&&a[i]>a[i+1]&&a[i]>a[i-1])
        {
            aFourthMax = a[i]; 
					  nFourthMax=i;
					  fFourthMax=f[nFourthMax];
        }
    }
		
		k=fabs(2*fMax-fSecondMax);
		k1=fabs(3*fMax-fSecondMax);
		m=fabs((float)(aMax-3.0*aSecondMax));
		
		if(k<=5)
		{
			LCD_ShowString(100,00,12*4,16,16,"Jag      ");
			Wave=1;
		}
		else if(k1<=5&&m<0.3)
		{			
			LCD_ShowString(100,00,12*4,16,16,"Square  ");
			Wave=2;
		}
		else if(k1<=5&&m>=0.3)
		{
			LCD_ShowString(100,00,12*4,16,16,"Triangle");
			Wave=3;
		}
		else 
		{				
			LCD_ShowString(100,00,12*4,16,16,"Sin     ");
			Wave=4;
		}
}


void lcd_print_fft(unsigned int *p)
{
	unsigned int *pp = p;             //p+1�൱����ֱ�Ӱ�0HZ�����˵���(�ĳ��˲�����)
	unsigned int i = 0,j = 0;
	float f[NPT/2]={0.00},a[NPT/2]={0.00};
	for(i=0;i<NPT/2;i++)
  {
	  if(*pp>80)//�����������������̫���͵���,��С�ķ�ֵ���ˣ��Է�����
		{
			f[j]=(float)i*Fs/NPT;
			//LCD_ShowFloat4(0,j*12,f[j],6,12);
			a[j]=(float)*pp*(3.3/4096);
			//LCD_ShowFloat4(100,j*12,a[j],2,12);
			j++;
		}
		pp++;
  }
	select_max(f,a);
}

