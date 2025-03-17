#include "timer8.h"
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
static void TIM8_GPIO_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //输出比较通道GPIO初始化，上桥臂
  RCC_APB2PeriphClockCmd(Advanced_TIM8_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin=Advanced_TIM8_CH1_PIN;
  GPIO_Init(Advanced_TIM8_CH1_PORT,&GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(Advanced_TIM8_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin=Advanced_TIM8_CH2_PIN;
  GPIO_Init(Advanced_TIM8_CH2_PORT,&GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(Advanced_TIM8_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin=Advanced_TIM8_CH3_PIN;
  GPIO_Init(Advanced_TIM8_CH3_PORT,&GPIO_InitStructure);


  //输出比较互补通道GPIO初始化，此处没有复用为互补通道，下桥臂用普通IO控制
  
//PA7[更换PB9，因为PA7用作编码器接口了]
  
//  RCC_APB2PeriphClockCmd(Advanced_TIM8_CHN1_GPIO_CLK, ENABLE);
////  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;

//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Pin=Advanced_TIM8_CHN1_PIN;
//  GPIO_Init(Advanced_TIM8_CHN1_PORT,&GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;

  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
//PA7[更换PB9，因为PA7用作编码器接口了] 


//PB0 [更换PB7，因为PB0用作屏幕背光了]

//  RCC_APB2PeriphClockCmd(Advanced_TIM8_CHN2_GPIO_CLK, ENABLE);
////  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Pin=Advanced_TIM8_CHN2_PIN;
//  GPIO_Init(Advanced_TIM8_CHN2_PORT,&GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;

  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
//PB0 [更换PB7，因为PB0用作屏幕背光了]




  RCC_APB2PeriphClockCmd(Advanced_TIM8_CHN3_GPIO_CLK, ENABLE);
//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin=Advanced_TIM8_CHN3_PIN;
  GPIO_Init(Advanced_TIM8_CHN3_PORT,&GPIO_InitStructure);
  

  //输出比较通道的刹车通道，没用到
//  RCC_APB2PeriphClockCmd(Advanced_TIM8_BKIN_GPIO_CLK, ENABLE);
//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Pin=Advanced_TIM8_BKIN_PIN;
//  GPIO_Init(Advanced_TIM8_BKIN_PORT,&GPIO_InitStructure);
}



/**************************************************************************
函数名：AdvancedTIM_Base_Init()
函数功能：高级定时器时基的初始化
入口参数：无
引	 脚：无
**************************************************************************/
static void TIM8_Base_Init(uint16_t arr,  uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    RCC_APB2PeriphClockCmd(Advanced_TIM8_CLK, ENABLE);

    TIM_BaseInitStructure.TIM_Prescaler = psc;
    TIM_BaseInitStructure.TIM_Period = arr;
    TIM_BaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分频因子
    TIM_BaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//设置为向上计数
    TIM_BaseInitStructure.TIM_RepetitionCounter=0;//设置重复计数器的值，这里不需要重复计数，所以为0

    TIM_TimeBaseInit(Advanced_TIM8,&TIM_BaseInitStructure);
}



/**************************************************************************
函数名：AdvancedTIM_OC_Init()
函数功能：高级定时器输出比较结构体的初始化
入口参数：无
引	 脚：无
**************************************************************************/
static void TIM8_OC_Init(uint16_t ccr)
{
  TIM_OCInitTypeDef TIM_OCInitStructure;
    //注意这里的配置，主通道是不带N,互补通道是带N的，如果复制错了配置就有问题了
  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//配置为PWM1模式
  TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Disable;//允许输出通过IO与外界相连
  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//允许互补输出通过IO与外界相连
  TIM_OCInitStructure.TIM_Pulse= ccr;//设置占空比
  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//设置定时器输出的是高电平还是低电平
  TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;//设置在空闲状态下，定时器输出什么电平
  TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
 
  TIM_OC1Init(Advanced_TIM8,&TIM_OCInitStructure);
  TIM_OC2Init(Advanced_TIM8,&TIM_OCInitStructure);
  TIM_OC3Init(Advanced_TIM8,&TIM_OCInitStructure);
  
  TIM_OC1PreloadConfig(Advanced_TIM8,TIM_OCPreload_Disable);
  TIM_OC2PreloadConfig(Advanced_TIM8,TIM_OCPreload_Disable);
  TIM_OC3PreloadConfig(Advanced_TIM8,TIM_OCPreload_Disable);
      
  TIM_OC1FastConfig(Advanced_TIM8, TIM_OCFast_Disable);  
  TIM_OC2FastConfig(Advanced_TIM8, TIM_OCFast_Disable);  
  TIM_OC3FastConfig(Advanced_TIM8, TIM_OCFast_Disable);   
  
  TIM_Cmd(Advanced_TIM8, ENABLE);

  //主输出使能，当使用的是通用定时器，这句不需要
  TIM_CtrlPWMOutputs(Advanced_TIM8, ENABLE);
}

void NVIC_SET(uint8_t pre, uint8_t sub)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ITConfig(Advanced_TIM8,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置

        
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

}

/**************************************************************************
函数名：AdvancedTIM_BDTR_Init()
函数功能：高级定时器刹车和死区结构体的初始化
入口参数：无
引	 脚：无
**************************************************************************/
//static void TIM8_BDTR_Init(uint16_t DeadTime)
//{
//  TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
//  
//  TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Disable;//空闲模式下的关闭状态选择
//  TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;//运行模式下的关闭状态选择
//  TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;//锁存配置

//  TIM_BDTRInitStructure.TIM_DeadTime = DeadTime;//死区时间
//  TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;//断路输入使能控制

//  TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_Low;//断路输入极性
//  TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;//自动输出极性
//  TIM_BDTRConfig(Advanced_TIM8,&TIM_BDTRInitStructure);
//}

/**************************************************************************
函数名：AdvancedTIM_Init
函数功能：高级定时器总的初始化
入口参数：无
引	 脚：无
**************************************************************************/
void TIM8_Int_Init()
{
  TIM8_GPIO_Init();
  TIM8_Base_Init(10000 - 1 , 1 - 1);
  NVIC_SET(0,0);
  TIM8_OC_Init(0);
//  TIM8_BDTR_Init(0x0F);//没用到刹车引脚
}

