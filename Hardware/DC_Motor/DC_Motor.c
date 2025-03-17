#include "DC_Motor.h"
#include "stm32f10x.h"                  // Device header
#include "advanced_tim1.h"
void dcmotor_init(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure;
    
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
        
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //LED0-->PB.5 �˿�����
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //IO���ٶ�Ϊ50MHz
     GPIO_Init(GPIOF, &GPIO_InitStructure); //�����趨������ʼ��GPIOB.5
     GPIO_ResetBits(GPIOF,GPIO_Pin_10);  //PB.5 �����

}
void dcmotor_start(void)
{
    TIM_Cmd(Advanced_TIM, ENABLE);

    TIM_CtrlPWMOutputs(Advanced_TIM, ENABLE);
    
    GPIO_SetBits(GPIOF,GPIO_Pin_10);    
}

void dcmotor_stop(void)
{
    TIM_Cmd(Advanced_TIM, DISABLE);

    TIM_CtrlPWMOutputs(Advanced_TIM, DISABLE);
    
    GPIO_ResetBits(GPIOF,GPIO_Pin_10);  
     
}
void dcmotor_dir(uint8_t para)
{
    TIM_CCxCmd(Advanced_TIM, TIM_Channel_1,TIM_CCx_Disable);
    TIM_CCxNCmd(Advanced_TIM, TIM_Channel_1,TIM_CCxN_Disable);
    if(para == 0)
    {
      TIM_CCxCmd(Advanced_TIM, TIM_Channel_1,TIM_CCx_Enable);
    }
    else if(para == 1)
    {
          TIM_CCxNCmd(Advanced_TIM, TIM_Channel_1,TIM_CCxN_Enable);
    }
}
void dcmotor_speed(uint16_t para)
{
    TIM_SetCompare1(TIM1,para);
}
void motor_pwm_set(float para)
{
    int val = (int)para;

    if (val >= 0) 
    {
        dcmotor_dir(0);           /* ��ת */
        dcmotor_speed(val);
    } 
    else 
    {
        dcmotor_dir(1);           /* ��ת */
        dcmotor_speed(-val);
    }
}
