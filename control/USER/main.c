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
 ALIENTEK NANO板STM32F103开发板 实验9
 PWM输出实验-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#define ang_0 50     //0°
#define ang_dry 83   //30° 
#define ang_wet 117  //70°
#define ang_har 172  //110°
#define ang_rec 217  //150°
#define ang_180 250  //180°

//u16 garbage_pwmval(char garbage,int flag)  //4种垃圾对应4种不同的pwm占空比,并设定对应的占空比
//{
//	u16 pwmval;
//	switch(garbage){
//		case 'B':pwmval = ang_dry; break;  //干垃圾
//		case 'G':pwmval = ang_wet; break;  //湿垃圾
//		case 'S':pwmval = ang_har; break;  //有害垃圾
//		case 'K':pwmval = ang_rec; break;  //可回收垃圾
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
  //char garbage='S';                  //从WIFI模块接收到的垃圾种类
	int dir=1;
	HAL_Init();                    	//初始化HAL库    
	Stm32_Clock_Init(RCC_PLL_MUL9); //设置时钟,72M
	delay_init(72);                 //初始化延时函数
	uart_init(115200);				//初始化串口
	//KEY_Init();						//初始化按键
	//BEEP_Init();		            //初始化BEEP
	GPIO_Init();          //
	//EXTI_Init();					//初始化外部中断
	//LED_Init();                     //初始化LED 
	TIM1_PWM_Init(1999,719);			//PWM频率=72000000/(1999+1)/(719+1)=50hz 
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
//		if(dir)pwmval++;  //dir==1 led6pwmval递增
//		else pwmval--;	  //dir==0 led6pwmval递减
// 		if(pwmval>250)dir=0;//led6pwmval到到300后，方向改为递减
//		if(pwmval==50)dir=1;	//led6pwmval递减到0后，方向改为递增	  
//    TIM_SetTIM1Compare1(pwmval);		
    if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1)
		{
        TIM_SetTIM1Compare1(250);		//修改比较值，修改占空比			
        delay_ms(500);
		}
    else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==0)
		{
        TIM_SetTIM1Compare1(50);		//修改比较值，修改占空比			
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

