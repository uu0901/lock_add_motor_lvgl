#include "stm32f10x.h"                  // Device header
#include "encoder.h" 
#include "pid.h"  
#include "debug.h"  
#include "dc_motor.h"  
void Encoder_Init(uint16_t arr, uint16_t psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计数器模式由编码器接口托管
    TIM_TimeBaseInitStructure.TIM_Period = arr;                           //ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                       //PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;         //此处的极性参数，配置编码器接口也有配置，这里配置属于重复配置
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 10;
//    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//此处的极性参数，配置编码器接口也有配置，这里配置属于重复配置
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                           //使能指定的TIM3中断,允许更新中断
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    
    NVIC_InitTypeDef NVIC_InitStructure;                                //定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	                    //选择配置NVIC的EXTI0线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	                    //指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;         //指定NVIC线路的抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                 //指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure);	
    
    
    TIM_Cmd(TIM3, ENABLE);
}
    
volatile int EncodeCount = 0;                               /* 用于记录溢出次数，向下溢出就-1，向上溢出就+1 */
int kk = 0;    
int jian = 0; 
int zeng = 0;  
void TIM3_IRQHandler(void)
{
    kk ++;
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        if( (TIM3 -> CR1 & (1 <<4 )) )//取出TIM3的CR1控制寄存器的DIR位，改位为0为向上计数，该位为1为向下计数
//        if( (TIM3->CR1 & 0x0010) == 1)//取出TIM3的CR1控制寄存器的DIR位，改位为0为向上计数，该位为1为向下计数
        {
            EncodeCount -- ;  /* DIR位为1，也就是递减计数 */
              jian ++;
        }
        else 
        {
            EncodeCount ++ ;  /* DIR位为0，也就是递增计数 */
            zeng ++;
        }
    }
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    
}
/**
 * @brief       编码器计数
 * @param       无
 * @retval      当前总计数值
 */
int32_t Get_Encode(void)
{
    /* 计算当前总计数值，当前总计数值 = 计数器当前值 + 溢出次数*65536  */
//    return ( int16_t )(TIM_GetCounter(TIM3) + EncodeCount * 65536);
        return ( int32_t )(TIM_GetCounter(TIM3)) + EncodeCount * 65536;
}

//int32_t encode_old,encode_now,var;
///**
// * @brief       电机速度计算
// * @param       无
// * @retval      电机速度
// */
//int32_t Speed_Computer(void)
//{
//    
//    encode_now = Get_Encode();                         /* 获取当前计数值 */
//    var = encode_now - encode_old;                          /* 计算计数变化量 */
//    encode_old = encode_now;                                /* 保存这一次的计数值 */
//    
//    return ( int32_t )( var * 5 * 60 / 4 / 11 / 30 );       /* 返回电机速度值 */

//}

//定时器2中断服务程序，而不是采用上面注释代码用轮询来采集速度，具体初始化在timer.c
//1ms进入一次中断，计算编码器的速度
int32_t EncoderNow;
int32_t EncoderOld;
float EncoderSpeed;
#define ROTO_RATIO      44  /* 转子转速:转子旋转一周所需脉冲数11，编码器使用双线检测也就是4倍频 */
#define REDUCTION_RATIO 30  /* 减速比1:30 */

Motor_TypeDef MotorData;  /*电机参数变量*/
void speed_computer(int32_t encode_now, uint8_t ms)
{
    uint8_t i = 0, j = 0;
    float temp = 0.0;
    static uint8_t sp_count = 0, k = 0;
    static float speed_arr[10] = {0.0};                     /* 存储速度进行滤波运算 */

    if (sp_count == ms)                                     /* 50ms计算一次速度 */
    {
        /* 计算电机转速 
           第一步：计算50ms计数变化量
           第二步 ；计算1min内计数变化量：g_encode.speed * ((1000 / 50) * 60 ，
           第三步 ：除以编码器旋转一圈的计数次数（倍频倍数 * 编码器分辨率）
           第四步 ：除以减速比即可得出电机转速
        */
        EncoderNow = encode_now;                                /* 取出编码器当前计数值 */
        EncoderSpeed = (EncoderNow - EncoderOld);           /* 计算编码器计数值的变化量 */
        
        speed_arr[k++] = (float)(EncoderSpeed * ((1000 / ms) * 60.0) / REDUCTION_RATIO / ROTO_RATIO );    /* 保存电机转速 */
        
        EncoderOld = EncoderNow;          /* 保存当前编码器的值 */

        /* 保存了10次速度值，后续进行1冒泡排序2求平均值3一阶低通滤波*/
        if (k == 10)
        {
            for (i = 10; i >= 1; i--)                       /* 1冒泡排序*/
            {
                for (j = 0; j < (i - 1); j++) 
                {
                    if (speed_arr[j] > speed_arr[j + 1])    /* 数值比较 */
                    { 
                        temp = speed_arr[j];                /* 数值换位 */
                        speed_arr[j] = speed_arr[j + 1];
                        speed_arr[j + 1] = temp;
                    }
                }
            }
            
            temp = 0.0;//中间变量清0，开启下面的2求平均值
            
            for (i = 2; i < 8; i++)                         /* 去除两边高低数据 */
            {
                temp += speed_arr[i];                       /* 将中间数值累加 */
            }
            
            temp = (float)(temp / 6);                       /*求速度平均值*/
            
            /*3 一阶低通滤波
             * 公式为：Y(n)= qX(n) + (1-q)Y(n-1)
             * 其中X(n)为本次采样值；Y(n-1)为上次滤波输出值；Y(n)为本次滤波输出值，q为滤波系数
             * q值越小则上一次输出对本次输出影响越大，整体曲线越平稳，但是对于速度变化的响应也会越慢
             */
            MotorData.speed = (float)( ((float)0.48 * temp) + (EncoderSpeed * (float)0.52) );//记录滤波后的值到 MotorData.speed，最后在主循环打印此变量
            k = 0;//将速度数组的指标赋值为0，等待下一次数组保存，将是从speed_arr[0]开始
        }
        sp_count = 0;//计数清0，等待下一次速度计算
    }
    sp_count ++;//记录进入这个中断的次数，此中断每1ms进入一次
}
//uint8_t c;
uint8_t  g_run_flag = 0; 
void TIM2_IRQHandler(void)   //TIM2中断
{
      static uint8_t val = 0;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
        {
//            c ++;
            int32_t Encode_now = Get_Encode();                             /* 获取编码器值，用于计算速度 */
            speed_computer(Encode_now, 5);                                 /* 中位平均值滤除编码器抖动数据，5ms计算一次速度*/
            if (val % SMAPLSE_PID_SPEED == 0)                                              /* 50ms进行一次pid计算 */
            {
                if (g_run_flag)                                             /* 判断电机是否启动了*/
                { 
                    /* PID计算，输出比较值 */
                    MotorData.motor_pwm = increment_pid_ctrl(&g_speed_pid, MotorData.speed);

                            if (MotorData.motor_pwm >= 8200)                 /* 限速 */
                            {
                                MotorData.motor_pwm = 8200;
                            }
                            else if (MotorData.motor_pwm <= -8200)
                            {
                                MotorData.motor_pwm = -8200;
                            }
#if DEBUG_ENABLE  /* 发送基本参数*/
                            
                    debug_send_wave_data( 1 ,MotorData.speed);                       /* 选择通道1 发送实际速度*/
                    debug_send_wave_data( 2 ,g_speed_pid.SetPoint);                 /* 选择通道2 发送目标速度*/
                    debug_send_wave_data( 3 ,MotorData.motor_pwm *100 / 8400);      /* 选择通道3，发送占空比（波形显示）*/
#endif                 
                    motor_pwm_set(MotorData.motor_pwm);                  /* 赋值PWM到电机 */
                }
                val = 0;
            }
            val ++;    
        }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
}
