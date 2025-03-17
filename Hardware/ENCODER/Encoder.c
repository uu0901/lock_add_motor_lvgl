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
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     //������ģʽ�ɱ������ӿ��й�
    TIM_TimeBaseInitStructure.TIM_Period = arr;                           //ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                       //PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;         //�˴��ļ��Բ��������ñ������ӿ�Ҳ�����ã��������������ظ�����
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 10;
//    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�˴��ļ��Բ��������ñ������ӿ�Ҳ�����ã��������������ظ�����
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                           //ʹ��ָ����TIM3�ж�,��������ж�
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    
    NVIC_InitTypeDef NVIC_InitStructure;                                //����ṹ�����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	                    //ѡ������NVIC��EXTI0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	                    //ָ��NVIC��·ʹ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;         //ָ��NVIC��·����ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                 //ָ��NVIC��·����Ӧ���ȼ�Ϊ1
    NVIC_Init(&NVIC_InitStructure);	
    
    
    TIM_Cmd(TIM3, ENABLE);
}
    
volatile int EncodeCount = 0;                               /* ���ڼ�¼������������������-1�����������+1 */
int kk = 0;    
int jian = 0; 
int zeng = 0;  
void TIM3_IRQHandler(void)
{
    kk ++;
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        if( (TIM3 -> CR1 & (1 <<4 )) )//ȡ��TIM3��CR1���ƼĴ�����DIRλ����λΪ0Ϊ���ϼ�������λΪ1Ϊ���¼���
//        if( (TIM3->CR1 & 0x0010) == 1)//ȡ��TIM3��CR1���ƼĴ�����DIRλ����λΪ0Ϊ���ϼ�������λΪ1Ϊ���¼���
        {
            EncodeCount -- ;  /* DIRλΪ1��Ҳ���ǵݼ����� */
              jian ++;
        }
        else 
        {
            EncodeCount ++ ;  /* DIRλΪ0��Ҳ���ǵ������� */
            zeng ++;
        }
    }
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    
}
/**
 * @brief       ����������
 * @param       ��
 * @retval      ��ǰ�ܼ���ֵ
 */
int32_t Get_Encode(void)
{
    /* ���㵱ǰ�ܼ���ֵ����ǰ�ܼ���ֵ = ��������ǰֵ + �������*65536  */
//    return ( int16_t )(TIM_GetCounter(TIM3) + EncodeCount * 65536);
        return ( int32_t )(TIM_GetCounter(TIM3)) + EncodeCount * 65536;
}

//int32_t encode_old,encode_now,var;
///**
// * @brief       ����ٶȼ���
// * @param       ��
// * @retval      ����ٶ�
// */
//int32_t Speed_Computer(void)
//{
//    
//    encode_now = Get_Encode();                         /* ��ȡ��ǰ����ֵ */
//    var = encode_now - encode_old;                          /* ��������仯�� */
//    encode_old = encode_now;                                /* ������һ�εļ���ֵ */
//    
//    return ( int32_t )( var * 5 * 60 / 4 / 11 / 30 );       /* ���ص���ٶ�ֵ */

//}

//��ʱ��2�жϷ�����򣬶����ǲ�������ע�ʹ�������ѯ���ɼ��ٶȣ������ʼ����timer.c
//1ms����һ���жϣ�������������ٶ�
int32_t EncoderNow;
int32_t EncoderOld;
float EncoderSpeed;
#define ROTO_RATIO      44  /* ת��ת��:ת����תһ������������11��������ʹ��˫�߼��Ҳ����4��Ƶ */
#define REDUCTION_RATIO 30  /* ���ٱ�1:30 */

Motor_TypeDef MotorData;  /*�����������*/
void speed_computer(int32_t encode_now, uint8_t ms)
{
    uint8_t i = 0, j = 0;
    float temp = 0.0;
    static uint8_t sp_count = 0, k = 0;
    static float speed_arr[10] = {0.0};                     /* �洢�ٶȽ����˲����� */

    if (sp_count == ms)                                     /* 50ms����һ���ٶ� */
    {
        /* ������ת�� 
           ��һ��������50ms�����仯��
           �ڶ��� ������1min�ڼ����仯����g_encode.speed * ((1000 / 50) * 60 ��
           ������ �����Ա�������תһȦ�ļ�����������Ƶ���� * �������ֱ��ʣ�
           ���Ĳ� �����Լ��ٱȼ��ɵó����ת��
        */
        EncoderNow = encode_now;                                /* ȡ����������ǰ����ֵ */
        EncoderSpeed = (EncoderNow - EncoderOld);           /* �������������ֵ�ı仯�� */
        
        speed_arr[k++] = (float)(EncoderSpeed * ((1000 / ms) * 60.0) / REDUCTION_RATIO / ROTO_RATIO );    /* ������ת�� */
        
        EncoderOld = EncoderNow;          /* ���浱ǰ��������ֵ */

        /* ������10���ٶ�ֵ����������1ð������2��ƽ��ֵ3һ�׵�ͨ�˲�*/
        if (k == 10)
        {
            for (i = 10; i >= 1; i--)                       /* 1ð������*/
            {
                for (j = 0; j < (i - 1); j++) 
                {
                    if (speed_arr[j] > speed_arr[j + 1])    /* ��ֵ�Ƚ� */
                    { 
                        temp = speed_arr[j];                /* ��ֵ��λ */
                        speed_arr[j] = speed_arr[j + 1];
                        speed_arr[j + 1] = temp;
                    }
                }
            }
            
            temp = 0.0;//�м������0�����������2��ƽ��ֵ
            
            for (i = 2; i < 8; i++)                         /* ȥ�����߸ߵ����� */
            {
                temp += speed_arr[i];                       /* ���м���ֵ�ۼ� */
            }
            
            temp = (float)(temp / 6);                       /*���ٶ�ƽ��ֵ*/
            
            /*3 һ�׵�ͨ�˲�
             * ��ʽΪ��Y(n)= qX(n) + (1-q)Y(n-1)
             * ����X(n)Ϊ���β���ֵ��Y(n-1)Ϊ�ϴ��˲����ֵ��Y(n)Ϊ�����˲����ֵ��qΪ�˲�ϵ��
             * qֵԽС����һ������Ա������Ӱ��Խ����������Խƽ�ȣ����Ƕ����ٶȱ仯����ӦҲ��Խ��
             */
            MotorData.speed = (float)( ((float)0.48 * temp) + (EncoderSpeed * (float)0.52) );//��¼�˲����ֵ�� MotorData.speed���������ѭ����ӡ�˱���
            k = 0;//���ٶ������ָ�긳ֵΪ0���ȴ���һ�����鱣�棬���Ǵ�speed_arr[0]��ʼ
        }
        sp_count = 0;//������0���ȴ���һ���ٶȼ���
    }
    sp_count ++;//��¼��������жϵĴ��������ж�ÿ1ms����һ��
}
//uint8_t c;
uint8_t  g_run_flag = 0; 
void TIM2_IRQHandler(void)   //TIM2�ж�
{
      static uint8_t val = 0;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
        {
//            c ++;
            int32_t Encode_now = Get_Encode();                             /* ��ȡ������ֵ�����ڼ����ٶ� */
            speed_computer(Encode_now, 5);                                 /* ��λƽ��ֵ�˳��������������ݣ�5ms����һ���ٶ�*/
            if (val % SMAPLSE_PID_SPEED == 0)                                              /* 50ms����һ��pid���� */
            {
                if (g_run_flag)                                             /* �жϵ���Ƿ�������*/
                { 
                    /* PID���㣬����Ƚ�ֵ */
                    MotorData.motor_pwm = increment_pid_ctrl(&g_speed_pid, MotorData.speed);

                            if (MotorData.motor_pwm >= 8200)                 /* ���� */
                            {
                                MotorData.motor_pwm = 8200;
                            }
                            else if (MotorData.motor_pwm <= -8200)
                            {
                                MotorData.motor_pwm = -8200;
                            }
#if DEBUG_ENABLE  /* ���ͻ�������*/
                            
                    debug_send_wave_data( 1 ,MotorData.speed);                       /* ѡ��ͨ��1 ����ʵ���ٶ�*/
                    debug_send_wave_data( 2 ,g_speed_pid.SetPoint);                 /* ѡ��ͨ��2 ����Ŀ���ٶ�*/
                    debug_send_wave_data( 3 ,MotorData.motor_pwm *100 / 8400);      /* ѡ��ͨ��3������ռ�ձȣ�������ʾ��*/
#endif                 
                    motor_pwm_set(MotorData.motor_pwm);                  /* ��ֵPWM����� */
                }
                val = 0;
            }
            val ++;    
        }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
}
