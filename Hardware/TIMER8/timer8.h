#ifndef __TIMR8___
#define __TIMR8___
#include "sys.h"

#define Advanced_TIM8                    TIM8
#define Advanced_TIM8_CLK                RCC_APB2Periph_TIM8

/**************************************************************************
函数名：AdvancedTIM_GPIO_Init()
函数功能：高级定时器GPIO的初始化
入口参数：无
引脚:     
        PA6(TIM8_BKIN)
        PC6(TIM8_CH1),PA7(TIM8_CH1N)
        PC7(TIM8_CH2),PB0(TIM8_CH2N)
        PC8(TIM8_CH3),PB1(TIM8_CH3N)
        
        

        PB12(TIM1_BKIN)
        PA8(TIM1_CH1),PB13(TIM1_CH1N)
        PA9(TIM1_CH2),PB14(TIM1_CH2N)
        PA10(TIM1_CH3),PB15(TIM1_CH3N)
**************************************************************************/
 /* TIMX PWM 定义 
 * 默认使用的是TIM8_CH1.
 * 注意: 通过修改这几个宏定义, 可以支持TIM1/TIM8定时器
 */
#define Advanced_TIM8_CH1_GPIO_CLK       RCC_APB2Periph_GPIOC
#define Advanced_TIM8_CH1_PORT           GPIOC
#define Advanced_TIM8_CH1_PIN            GPIO_Pin_6

#define Advanced_TIM8_CH2_GPIO_CLK       RCC_APB2Periph_GPIOC
#define Advanced_TIM8_CH2_PORT           GPIOC
#define Advanced_TIM8_CH2_PIN            GPIO_Pin_7

#define Advanced_TIM8_CH3_GPIO_CLK       RCC_APB2Periph_GPIOC
#define Advanced_TIM8_CH3_PORT           GPIOC
#define Advanced_TIM8_CH3_PIN            GPIO_Pin_8

/* 互补通道IO */
#define Advanced_TIM8_CHN1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define Advanced_TIM8_CHN1_PORT          GPIOA
#define Advanced_TIM8_CHN1_PIN           GPIO_Pin_7

#define Advanced_TIM8_CHN2_GPIO_CLK      RCC_APB2Periph_GPIOB
#define Advanced_TIM8_CHN2_PORT          GPIOB
#define Advanced_TIM8_CHN2_PIN           GPIO_Pin_0

#define Advanced_TIM8_CHN3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define Advanced_TIM8_CHN3_PORT          GPIOB
#define Advanced_TIM8_CHN3_PIN           GPIO_Pin_1

/* 刹车引脚IO */
#define Advanced_TIM8_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOA
#define Advanced_TIM8_BKIN_PORT          GPIOA
#define Advanced_TIM8_BKIN_PIN           GPIO_Pin_6


void TIM8_Int_Init(void);

#endif
