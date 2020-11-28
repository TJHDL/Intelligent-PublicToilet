#include "gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//��������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
  
//������ IO��ʼ��
void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_13; 			//PC13
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	 __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIODʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_2; 			//PD2
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_10; 			//PB10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //���
    GPIO_Initure.Pull=GPIO_NOPULL;          
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    //HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);	//��������Ӧ����GPIOB8���ߣ�
}
