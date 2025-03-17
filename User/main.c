#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "sram.h"
#include "lvgl_demo.h"
#include "malloc.h"
#include "advanced_tim1.h"
#include "dc_motor.h"
#include "encoder.h"
#include "timer2.h"
#include "pid.h"
#include "debug.h"
//#include "timer4.h"
//#include "st1ep_motor.h"           //��������ĳ�ʼ�����ڽ���ϵͳ�Ļص������ڣ�����main��ʼ���ᵼ�²�������޷�ת��������ԭ����
#include "timer5.h"
//#include "timer8.h"
//#include "bldc_motor.h"           //��ˢ����ĳ�ʼ�����ڽ���ϵͳ�Ļص������ڣ��˴��и�bug�Ǵ����������º��������ˢ���������Ķ�ʱ������������������������������ǲ��������

int main(void)
{
    delay_init();//��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);     //�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);                                      //����1PID��λ��
    uart2_init(115200);                                     //����2��ӡ
    FSMC_SRAM_Init();
    printf("Hello\r\n");
    LED_Init();                                             //LED�˿ڳ�ʼ��
    LCD_Init();
    KEY_Init();
    my_mem_init(SRAMIN);

    AdvancedTIM_Init();                                     //�߼���ʱ��1��ʼ��
    dcmotor_init();                                         //ֱ�������ʼ��
    Encoder_Init(65536 -1 ,1 -1);                           //������������ʼ��
    TIM2_Int_Init(2000 -1 ,72 -1);                          /* ͨ�ö�ʱ����ʼ����72000000/72/2000 = 0.5ms�������� *///���˴�f103��Ҫ0.5ms���жϣ���f407ֻ��Ҫ1ms��    
    pid_init();                                             /* ��ʼ��PID���� */

#if DEBUG_ENABLE                                            /* �������� */
    debug_init();                                           /* ��ʼ����λ������ */
    /* ͬ�����ݣ�ѡ���1��PID,Ŀ���ٶȵ�ַ,P,I,D����������λ�� */
    debug_send_initdata(TYPE_PID1, (float *)(&g_speed_pid.SetPoint), KP, KI, KD);
    debug_send_motorcode(DC_MOTOR);                      /* ֱ����� */
    debug_send_motorstate(IDLE_STATE);                   /* ������� */
#endif


    Timer5_Init(200 - 1, 7200 - 1);                       /* 72 000 000 / 7200 = 10 000 10Khz�ļ���Ƶ�ʣ�����һ��Ϊ1/10k = 0.1ms,����200��Ϊ20ms */



    lvgl_demo();

}


