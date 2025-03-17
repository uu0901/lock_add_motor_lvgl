#include "timer4.h"
#include "stm32f10x.h"                  // Device header

void Timer4_Init(uint16_t arr, uint16_t psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//注意PB8硬件上有蜂鸣器，若要使用，就把蜂鸣器三极管拆了
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB6 TIM4_CH1    PB7 TIM4_CH2    PB8 TIM4_CH3    PB9 TIM4_CH4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
		
    
    
    TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE ); //使能指定的TIM3中断,允许更新中断
//	TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE ); //使能指定的TIM3中断,允许更新中断
//    TIM_ITConfig(TIM4,TIM_IT_CC3,ENABLE ); //使能指定的TIM3中断,允许更新中断
//    TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE ); //使能指定的TIM3中断,允许更新中断

    NVIC_InitTypeDef NVIC_InitStructure;

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = arr / 2;		//CCR
	
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
//    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
                     
    TIM_Cmd(TIM4, ENABLE);
}
extern uint16_t g_ccr1;
volatile uint32_t accc;
void TIM4_IRQHandler(void)
{
    uint16_t val1 = 0;
    accc++;
//    printf("accc=%d\r\n",accc);
//    uint16_t val2 = 0;
//    uint16_t val3 = 0;
//    uint16_t val4 = 0;
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
    {
            accc++;
        val1 = TIM_GetCapture1(TIM4);
        TIM_SetCompare1(TIM4, (val1+ g_ccr1)%0xffff);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    }
//    if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
//    {
//        val2 = TIM_GetCapture2(TIM4);
//        TIM_SetCompare1(TIM4, (val1+ g_ccr1)%0XFFFF);
//        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
//    }
//    if(TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET)
//    {
//        val3 = TIM_GetCapture3(TIM4);
//        TIM_SetCompare1(TIM4, (val1+ g_ccr1)%0XFFFF);
//        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
//    }
//    if(TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET)
//    {
//        val4 = TIM_GetCapture4(TIM4);
//        TIM_SetCompare1(TIM4, (val4+ g_ccr1)%0XFFFF);
//        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
//    }    
}
