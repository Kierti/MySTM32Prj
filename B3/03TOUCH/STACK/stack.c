#include<stdio.h>
#include<stdlib.h>
#include<lcd.h>
#include<malloc.h>
#include<math.h>
#include<stack.h>
//ջ
struct dataStack
{
	int data[max];
	int top;//ָ��ָ��ջ����ַ
};

struct symStack
{
	char symbol[max];
	int top;
};


int X;
//ջ�ĳ�ʼ��
void InitDataStack(struct dataStack* Stacknum)
{
	Stacknum->top = -1;
}

void InitSymStack(struct symStack* Stacksym)
{
	Stacksym->top = -1;
}


//��ջ
void Popdata(struct dataStack* Stacknum, int num)
{
	Stacknum->top++;
	Stacknum->data[Stacknum->top] = num;
}

void Popsym(struct symStack* Stacksym, char sym)
{
	Stacksym->top++;
	Stacksym->symbol[Stacksym->top] = sym;
}


//��ջ
int Pushdata(struct dataStack* Stacknum)
{
	int x = Stacknum->data[Stacknum->top--];
	return x;
}

char Pushsym(struct symStack* Stacksym)
{
	char c = Stacksym->symbol[Stacksym->top--];
	return c;
}


//�鿴ջ��
int loaddata(struct dataStack* Stacknum)
{
	return Stacknum->data[Stacknum->top];
}

char loadsym(struct symStack* Stacksym)
{
	return Stacksym->symbol[Stacksym->top];
}


//���ȼ�
int sympriority(char ch)
{
	if (ch == '(')  return 1;
	else if (ch == '+' || ch == '-')  return 2;
	else if (ch == '*' || ch == '/')  return 3;
	else if (ch == ')')  return 4;
	return 0;
}


//����
int math(int x, int y,char ch)
{
	int sum = 0;
	switch (ch)
	{
	case'+':sum = x + y; break;
	case'-':sum = x - y; break;
	case'*':sum = x * y; break;
	case'/':sum = x / y; break;
	}
	return sum;
}


int STACKS(char *t)
{
	struct dataStack data;
	struct symStack symbol;
	InitDataStack(&data);
	InitSymStack(&symbol);
	int i, j;
	float sum, x, y;
	char *ch=t;
	i = 0, j = 0, sum = 0;
	char v[max] = { 0 };//�ݴ��ַ���v
	char str[max];
	char *c=str;
	for(i=0;*(ch+i)!='\0';i++)
	{
		*(c+i)=*(ch+i);
	}
	str[i] = '\0';//ĩβ����ֹ����
	for (i = 0; str[i] != '\0'; i++)//���е��ַ���ĩβ����ѭ��
	{
		if (i == 0 && str[i] == '-')
		{
			v[j++] = str[i];
		}
		else if (str[i] == '(' && str[i + 1] == '-')
		{
			i++;
			v[j++] = str[i++];
			while ((str[i] >= '0' && str[i] <= '9')||str[i]=='.')
			{
				v[j] = str[i];
				j++; i++;
			}
			Popdata(&data, atof(v));//atof����ɨ���ַ��е�int��������
			while (j > 0)//����ݴ��ַ���v
			{
				v[j] = 0;
				j--;
			}
			if (str[i] != ')')
			{
				i--;
				Popsym(&symbol, '(');
			}
		}
		else if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
		{
			while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
			{
				v[j] = str[i];
				i++; j++;
			}
			Popdata(&data, atof(v));
			while (j > 0)//����ݴ��ַ���v
			{
				v[j] = 0;
				j--;
			}
			i--;
		}
		else
		{
			//����ջ
			if (symbol.top == -1)
			{
				Popsym(&symbol, str[i]);
			}
			//��Ϊ����'('
			else if (sympriority(str[i]) == 1)
			{
				Popsym(&symbol, str[i]);//��ջ
			}
			//��Ϊ����'+'or'-'
			else if (sympriority(str[i]) == 2)
			{
				if (sympriority(loadsym(&symbol)) == 1)//��Ϊջ��'('��ջ
				{
					Popsym(&symbol, str[i]);
				}
				else if (sympriority(loadsym(&symbol)) == 2)//��ջ��Ϊ'+'��'-'��ջ����
				{
					while (symbol.top >= 0 && data.top >= 1)
					{
						x = Pushdata(&data);
						y = Pushdata(&data);
						sum = math(y, x, Pushsym(&symbol));
						Popdata(&data, sum);//sum������ջ
					}
					Popsym(&symbol, str[i]);//�·��������ջ
				}
				else if (sympriority(loadsym(&symbol)) == 3)//��ջ��Ϊ'*'��'/'��ջ����
				{
					while (symbol.top >= 0 && data.top >= 1)
					{
						x = Pushdata(&data);
						y = Pushdata(&data);
						sum = math(y, x, Pushsym(&symbol));
						Popdata(&data, sum);//sum������ջ
					}
					Popsym(&symbol, str[i]);//�·��������ջ
				}
			}
			//��Ϊ'*'or'/'
			else if (sympriority(str[i]) == 3)
			{
				if (sympriority(loadsym(&symbol)) == 1)//��Ϊջ��'('��ջ
				{
					Popsym(&symbol, str[i]);
				}
				else if (sympriority(loadsym(&symbol)) == 2)//��ջ��Ϊ'+'��'-'��ջ����
				{
					Popsym(&symbol, str[i]);
				}
				else if (sympriority(loadsym(&symbol)) == 3)//��ջ��Ϊ'*'��'/'��ջ����
				{
					while (symbol.top >= 0 && data.top >= 1)
					{
						x = Pushdata(&data);
						y = Pushdata(&data);
						sum = math(y, x, Pushsym(&symbol));
						Popdata(&data, sum);//sum������ջ
					}
					Popsym(&symbol, str[i]);//�·��������ջ
				}
			}
			//��Ϊ')'
			else if (sympriority(str[i]) == 4)
			{
				while (sympriority(loadsym(&symbol)) != 1)//ѭ����ջֱ������'('
				{
					x = Pushdata(&data);
					y = Pushdata(&data);
					sum = math(y, x, Pushsym(&symbol));
					Popdata(&data, sum);//sum������ջ
				}
				Pushsym(&symbol);//��ʱָ��ָ��'(',ʹ���ջ
			}
		}

	}

	while (symbol.top != -1)
	{
		x = Pushdata(&data);
		y = Pushdata(&data);
		sum = math(y, x, Pushsym(&symbol));
		Popdata(&data, sum);//sum������ջ
	}
	
	char *a=(char*)malloc(sizeof(char));//���ٶ�̬���飬��ֹ�޷���������
	int l = Pushdata(&data);
	X=l;
	printf("\t[���]result:%d\r\n",X);
	sprintf(a,"%d",l);//תintΪ�ַ��������
	LCD_ShowString(2,80,200,24,24,(u8*)a);//��ʾ���
	free(a);
	free(str);
	return 0;
}
