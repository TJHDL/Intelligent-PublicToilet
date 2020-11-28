#include "exti.h"
#include "delay.h"
#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//�ⲿ�ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/6/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();               //����GPIOCʱ��
	  //__HAL_RCC_AFIO_CLK_ENABLE();                //����AFIOʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_13;                //PC13
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;      //����/�½��ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    //�ж���8-PA8
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,1,0);       //��ռ���ȼ�Ϊ1�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);             //ʹ���ж���9
    
}


void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);		//�����жϴ����ú���
}



//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(10);      	//����
    switch(GPIO_Pin)
    {
        case GPIO_PIN_13:
				LED6=!LED6;	//����LED6����
            break;	
    }
}
