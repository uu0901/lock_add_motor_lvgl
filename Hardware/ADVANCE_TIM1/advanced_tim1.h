#ifndef __ADCANCED_TIM1___
#define __ADCANCED_TIM1___
#include "sys.h"

#define Advanced_TIM                    TIM1
#define Advanced_TIM_CLK                RCC_APB2Periph_TIM1

#define Advanced_TIM_CH1_GPIO_CLK       RCC_APB2Periph_GPIOA
#define Advanced_TIM_CH1_PORT           GPIOA
#define Advanced_TIM_CH1_PIN            GPIO_Pin_8

#define Advanced_TIM_CHN1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define Advanced_TIM_CHN1_PORT          GPIOB
#define Advanced_TIM_CHN1_PIN           GPIO_Pin_13

#define Advanced_TIM_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
#define Advanced_TIM_BKIN_PORT          GPIOB
#define Advanced_TIM_BKIN_PIN           GPIO_Pin_12


void AdvancedTIM_Init(void);

#endif
