#include "stm32f10x.h"                  // Device header
#include "bldc_motor.h"
#include "timer8.h"
void ShutDown_Init(void)
{ 
     GPIO_InitTypeDef  GPIO_InitStructure;
        
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);  //ʹ��PB,PE�˿�ʱ��
        
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              //LED0-->PB.5 �˿�����
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //�������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO���ٶ�Ϊ50MHz
     GPIO_Init(GPIOG, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOB.5
     GPIO_ResetBits(GPIOG,GPIO_Pin_15);                      //PB.5 �����

}
void Hall_Init(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure;
        
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
        
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; //PC10��HALL-U   PC11��HALL-V PC12��HALL-W ���Ŷ���FT����5V����
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //��������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO���ٶ�Ϊ50MHz
     GPIO_Init(GPIOB, &GPIO_InitStructure);                 //�����趨������ʼ��GPIOB.5
     GPIO_ResetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12); //PB.5 �����

}
uint32_t Get_HallSensor(void)
{
     uint32_t state = 0;//__IO����volatile
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) != RESET)//hall-u
    {
        state |= 0x01;
    }
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) != RESET)//hall-v
    {
        state |= 0x02;
    }
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) != RESET)//hall-w
    {
        state |= 0x04;
    }
    return   state; 
}
void Motor_Stop(void)
{
    //ɲ������  �رհ���оƬ���
    GPIO_ResetBits(GPIOG,GPIO_Pin_15);
    //���ű�   �ر�PWM���
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_1,TIM_CCx_Disable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_2,TIM_CCx_Disable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_3,TIM_CCx_Disable);
    //���ñȽ�ֵ
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,0);
    //���ű�   �ر�      
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);

}
void Motor_Start(void)
{
    //ɲ������ ��������оƬ���
    GPIO_SetBits(GPIOG,GPIO_Pin_15);
    //���ű�    ����PWM���
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_1,TIM_CCx_Enable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_2,TIM_CCx_Enable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_3,TIM_CCx_Enable);
    
    //���ñȽ�ֵ�����ű۵Ķ����������������������
    
//    //���ñȽ�ֵ
    //���ñȽ�ֵ
//    TIM_SetCompare1(TIM8,0);
//    TIM_SetCompare2(TIM8,0);
//    TIM_SetCompare3(TIM8,0);
//    //���ű�   �ر�      
//    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
//    GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
//    GPIO_ResetBits(GPIOB,GPIO_Pin_1); 

   //���ñȽ�ֵ�����ű۵Ķ����������������������    
}

void UH_VL(void)
{
    //���ñȽ�ֵ
    TIM_SetCompare1(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,0);
    //���ű�             
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);   
    GPIO_SetBits(GPIOB,GPIO_Pin_7); 
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);   
}
void UH_WL(void)
{
    //���ñȽ�ֵ
    TIM_SetCompare1(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,0);
    //���ű�
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);   
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_SetBits(GPIOB,GPIO_Pin_1); 
}
void VH_UL(void)
{
    //���ñȽ�ֵ
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare3(TIM8,0);
    //���ű�
    GPIO_SetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
void VH_WL(void)
{
    //���ñȽ�ֵ
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare3(TIM8,0);
    //���ű�
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
}
void WH_UL(void)
{
    //���ñȽ�ֵ
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,g_bldc_motor1.pwm_duty);
    //���ű�
    GPIO_SetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
void WH_VL(void)
{
    //���ñȽ�ֵ
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,g_bldc_motor1.pwm_duty);
    //���ű�
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
    GPIO_SetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
typedef void(*pctr) (void);
/*  ����������ָ������ */
pctr pfunclist_m1[6] =
{
    &UH_WL, &VH_UL, &VH_WL,
    &WH_VL, &UH_VL, &WH_UL
};

uint8_t ldq;
void TIM8_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)
    {
        ldq++;
        if(g_bldc_motor1.run_flag == RUN)//����
        {
            if(g_bldc_motor1.dir == CW)//��ת
            {
                g_bldc_motor1.step_sta = Get_HallSensor();//��ȡ������������ֵ�������ת�����ĸ�λ��   ˳��6,2,3,1,5,4 
            }
            else if(g_bldc_motor1.dir == CCW)//��ת
            {
                g_bldc_motor1.step_sta = 7 - Get_HallSensor();//��ȡ������������ֵ�������ת�����ĸ�λ��   ˳��5,1,3,2,6,4 ��ʹ��7������������pfunclist_m1��Ӧ��˳�� ʵ�ʻ���ֵΪ��2,6,4,5,1,3    
            }
            if((g_bldc_motor1.step_sta >= 1) && (g_bldc_motor1.step_sta <= 6))//6��֮��
            {
                pfunclist_m1[g_bldc_motor1.step_sta-1]();                   /* ͨ�������Ա���Ҷ�Ӧ�ĺ���ָ�� */
            }
            else //  �������������󡢽Ӵ��������Ͽ������ ֹͣ
            {
                Motor_Stop();
                g_bldc_motor1.run_flag = STOP;
            }
        }
    
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
}
