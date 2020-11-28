#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "exti.h"
#include "key.h"
#include "beep.h"
#include "gpio.h"

/************************************************
 ALIENTEK NANO��STM32F103������ ʵ��9
 PWM���ʵ��-HAL��汾
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define ang_0 50     //0��
#define ang_dry 83   //30�� 
#define ang_wet 117  //70��
#define ang_har 172  //110��
#define ang_rec 217  //150��
#define ang_180 250  //180��

//u16 garbage_pwmval(char garbage,int flag)  //4��������Ӧ4�ֲ�ͬ��pwmռ�ձ�,���趨��Ӧ��ռ�ձ�
//{
//	u16 pwmval;
//	switch(garbage){
//		case 'B':pwmval = ang_dry; break;  //������
//		case 'G':pwmval = ang_wet; break;  //ʪ����
//		case 'S':pwmval = ang_har; break;  //�к�����
//		case 'K':pwmval = ang_rec; break;  //�ɻ�������
//		default:pwmval=ang_0;break;
//	}
//	
//	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) != flag)
//	{
//		pwmval=ang_0;
//		garbage='Y';
//		flag=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
//	}
//	
//	return pwmval;
//}
	
int main(void)
{
	u16 pwmval=215;   
  //char garbage='S';                  //��WIFIģ����յ�����������
	int dir=1;
	HAL_Init();                    	//��ʼ��HAL��    
	Stm32_Clock_Init(RCC_PLL_MUL9); //����ʱ��,72M
	delay_init(72);                 //��ʼ����ʱ����
	uart_init(115200);				//��ʼ������
	//KEY_Init();						//��ʼ������
	//BEEP_Init();		            //��ʼ��BEEP
	GPIO_Init();          //
	//EXTI_Init();					//��ʼ���ⲿ�ж�
	//LED_Init();                     //��ʼ��LED 
	TIM1_PWM_Init(1999,719);			//PWMƵ��=72000000/(1999+1)/(719+1)=50hz 
	//LED6=1;
	
	//int flag=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
	int cnt=1;
	
	while(1)
	{
		
		delay_ms(10);	 
		/*if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==1)
			printf("1");
		else
			printf("0");*/
		//pwmval=garbage_pwmval(garbage,flag);
//		if(dir)pwmval++;  //dir==1 led6pwmval����
//		else pwmval--;	  //dir==0 led6pwmval�ݼ�
// 		if(pwmval>250)dir=0;//led6pwmval����300�󣬷����Ϊ�ݼ�
//		if(pwmval==50)dir=1;	//led6pwmval�ݼ���0�󣬷����Ϊ����	  
//    TIM_SetTIM1Compare1(pwmval);		
    if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1)
		{
        TIM_SetTIM1Compare1(250);		//�޸ıȽ�ֵ���޸�ռ�ձ�			
        delay_ms(500);
		}
    else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==0)
		{
        TIM_SetTIM1Compare1(50);		//�޸ıȽ�ֵ���޸�ռ�ձ�			
        delay_ms(500);
		}
		
//		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==1)
//		{
//        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);			
//        delay_ms(500);
//		}
//    else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==0)
//		{
//        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1);			
//        delay_ms(500);
//		}

    		
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1);

		
	}
}

