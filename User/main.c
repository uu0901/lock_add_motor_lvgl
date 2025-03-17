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
//#include "st1ep_motor.h"           //步进电机的初始化放在进入系统的回调函数内，放在main初始化会导致步进电机无法转动，具体原因不详
#include "timer5.h"
//#include "timer8.h"
//#include "bldc_motor.h"           //无刷电机的初始化放在进入系统的回调函数内，此处有个bug是触摸按键按下好像会打断无刷动作函数的定时器，导致驱动板过流，但按键控制是不会过流的

int main(void)
{
    delay_init();//延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);     //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);                                      //串口1PID上位机
    uart2_init(115200);                                     //串口2打印
    FSMC_SRAM_Init();
    printf("Hello\r\n");
    LED_Init();                                             //LED端口初始化
    LCD_Init();
    KEY_Init();
    my_mem_init(SRAMIN);

    AdvancedTIM_Init();                                     //高级定时器1初始化
    dcmotor_init();                                         //直流电机初始化
    Encoder_Init(65536 -1 ,1 -1);                           //编码器计数初始化
    TIM2_Int_Init(2000 -1 ,72 -1);                          /* 通用定时器初始化，72000000/72/2000 = 0.5ms计数周期 *///【此处f103需要0.5ms进中断，而f407只需要1ms】    
    pid_init();                                             /* 初始化PID参数 */

#if DEBUG_ENABLE                                            /* 开启调试 */
    debug_init();                                           /* 初始化上位机调试 */
    /* 同步数据（选择第1组PID,目标速度地址,P,I,D参数）到上位机 */
    debug_send_initdata(TYPE_PID1, (float *)(&g_speed_pid.SetPoint), KP, KI, KD);
    debug_send_motorcode(DC_MOTOR);                      /* 直流电机 */
    debug_send_motorstate(IDLE_STATE);                   /* 电机空闲 */
#endif


    Timer5_Init(200 - 1, 7200 - 1);                       /* 72 000 000 / 7200 = 10 000 10Khz的计数频率，计数一次为1/10k = 0.1ms,计数200次为20ms */



    lvgl_demo();

}


