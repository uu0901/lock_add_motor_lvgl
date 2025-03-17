#include "stm32f10x.h"                  // Device header
#include "bldc_motor.h"
#include "timer8.h"
void ShutDown_Init(void)
{ 
     GPIO_InitTypeDef  GPIO_InitStructure;
        
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);  //使能PB,PE端口时钟
        
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              //LED0-->PB.5 端口配置
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO口速度为50MHz
     GPIO_Init(GPIOG, &GPIO_InitStructure);                  //根据设定参数初始化GPIOB.5
     GPIO_ResetBits(GPIOG,GPIO_Pin_15);                      //PB.5 输出高

}
void Hall_Init(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure;
        
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
        
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; //PC10：HALL-U   PC11：HALL-V PC12：HALL-W 引脚都是FT容忍5V输入
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //上拉输入
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO口速度为50MHz
     GPIO_Init(GPIOB, &GPIO_InitStructure);                 //根据设定参数初始化GPIOB.5
     GPIO_ResetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12); //PB.5 输出高

}
uint32_t Get_HallSensor(void)
{
     uint32_t state = 0;//__IO就是volatile
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
    //刹车引脚  关闭半桥芯片输出
    GPIO_ResetBits(GPIOG,GPIO_Pin_15);
    //上桥臂   关闭PWM输出
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_1,TIM_CCx_Disable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_2,TIM_CCx_Disable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_3,TIM_CCx_Disable);
    //设置比较值
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,0);
    //下桥臂   关闭      
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);

}
void Motor_Start(void)
{
    //刹车引脚 开启半桥芯片输出
    GPIO_SetBits(GPIOG,GPIO_Pin_15);
    //上桥臂    开启PWM输出
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_1,TIM_CCx_Enable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_2,TIM_CCx_Enable);
    TIM_CCxCmd(Advanced_TIM8, TIM_Channel_3,TIM_CCx_Enable);
    
    //设置比较值和下桥臂的动作，在六步换向函数里操作
    
//    //设置比较值
    //设置比较值
//    TIM_SetCompare1(TIM8,0);
//    TIM_SetCompare2(TIM8,0);
//    TIM_SetCompare3(TIM8,0);
//    //下桥臂   关闭      
//    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
//    GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
//    GPIO_ResetBits(GPIOB,GPIO_Pin_1); 

   //设置比较值和下桥臂的动作，在六步换向函数里操作    
}

void UH_VL(void)
{
    //设置比较值
    TIM_SetCompare1(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,0);
    //下桥臂             
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);   
    GPIO_SetBits(GPIOB,GPIO_Pin_7); 
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);   
}
void UH_WL(void)
{
    //设置比较值
    TIM_SetCompare1(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,0);
    //下桥臂
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);   
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_SetBits(GPIOB,GPIO_Pin_1); 
}
void VH_UL(void)
{
    //设置比较值
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare3(TIM8,0);
    //下桥臂
    GPIO_SetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
void VH_WL(void)
{
    //设置比较值
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,g_bldc_motor1.pwm_duty);
    TIM_SetCompare3(TIM8,0);
    //下桥臂
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
}
void WH_UL(void)
{
    //设置比较值
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,g_bldc_motor1.pwm_duty);
    //下桥臂
    GPIO_SetBits(GPIOB,GPIO_Pin_9);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
void WH_VL(void)
{
    //设置比较值
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
    TIM_SetCompare3(TIM8,g_bldc_motor1.pwm_duty);
    //下桥臂
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
    GPIO_SetBits(GPIOB,GPIO_Pin_7);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
typedef void(*pctr) (void);
/*  六步换向函数指针数组 */
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
        if(g_bldc_motor1.run_flag == RUN)//运行
        {
            if(g_bldc_motor1.dir == CW)//正转
            {
                g_bldc_motor1.step_sta = Get_HallSensor();//获取霍尔传感器的值，检测电机转到了哪个位置   顺序6,2,3,1,5,4 
            }
            else if(g_bldc_motor1.dir == CCW)//反转
            {
                g_bldc_motor1.step_sta = 7 - Get_HallSensor();//获取霍尔传感器的值，检测电机转到了哪个位置   顺序5,1,3,2,6,4 。使用7减完后可与数组pfunclist_m1对应上顺序 实际霍尔值为：2,6,4,5,1,3    
            }
            if((g_bldc_motor1.step_sta >= 1) && (g_bldc_motor1.step_sta <= 6))//6步之内
            {
                pfunclist_m1[g_bldc_motor1.step_sta-1]();                   /* 通过数组成员查找对应的函数指针 */
            }
            else //  霍尔传感器错误、接触不良、断开等情况 停止
            {
                Motor_Stop();
                g_bldc_motor1.run_flag = STOP;
            }
        }
    
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
}
