#include "lvgl_demo.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_mainstart.h"

#include "advanced_tim1.h"
#include "dc_motor.h"
#include "pid.h"
#include "debug.h"
uint8_t debug_cmd = 0;
extern uint8_t  g_run_flag;  /*�����������*/
extern Motor_TypeDef MotorData;  /*�����������*/
/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;         /* ������ */
void start_task(void *pvParameters);    /* ������ */

/* LV_DEMO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LV_DEMO_TASK_PRIO   3           /* �������ȼ� */
#define LV_DEMO_STK_SIZE    1024        /* �����ջ��С */
TaskHandle_t LV_DEMOTask_Handler;       /* ������ */
void lv_demo_task(void *pvParameters);  /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO       4           /* �������ȼ� */
#define LED_STK_SIZE        128         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;           /* ������ */
void led_task(void *pvParameters);      /* ������ */
/******************************************************************************************************/

/**
 * @brief       lvgl_demo��ں���
 * @param       ��
 * @retval      ��
 */
void lvgl_demo(void)
{
    lv_init();                                          /* lvglϵͳ��ʼ�� */
    lv_port_disp_init();                                /* lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ��� */
    lv_port_indev_init();                               /* lvgl����ӿڳ�ʼ��,����lv_init()�ĺ��� */

    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */

    vTaskStartScheduler();                              /* ����������� */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����LVGL���� */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
}

/**
 * @brief       LVGL��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lv_demo_task(void *pvParameters)
{
    lv_mainstart();  /* ���Ե�demo */
    
    while(1)
    {
        lv_timer_handler(); /* LVGL��ʱ�� */
        vTaskDelay(5);
    }
}

/**
 * @brief       ϵͳ������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
extern uint8_t a;
extern bool touchpad_is_pressed(void);
extern uint16_t g_ccr1 ;                //g_ccr150Ϊ�����     g_ccr1 500Ϊ�����
void led_task(void *pvParameters)
{
    
    while(1)
    {
#if DEBUG_ENABLE
       /* ��ѯ����PID���ֵ�PID���� */
        debug_receive_pid(TYPE_PID1, (float *)&g_speed_pid.Proportion,(float *)&g_speed_pid.Integral, (float *)&g_speed_pid.Derivative);

        debug_set_point_range(300, -300, 300);              /* ����Ŀ����ڷ�Χ���ٶ�-300~300��ͻ�䲻������300 */
        
        debug_cmd = debug_receive_ctrl_code();              /* ��ȡ���� */
        
        if (debug_cmd == BREAKED)                           /* ���ɲ�� */
        {
            dcmotor_stop();                                 /* ֹͣ��� */
            pid_init();                                     /* pid�������� */
            g_run_flag = 0;                                  /* ��ǵ��ֹͣ */
            MotorData.motor_pwm = 0;
            motor_pwm_set(MotorData.motor_pwm);          /* ���õ��ת���ٶ� */
            debug_send_motorstate(BREAKED_STATE);           /* �ϴ����״̬��ɲ���� */
            debug_send_initdata(TYPE_PID1, (float *)(&g_speed_pid.SetPoint), KP, KI, KD);
        } 
        else if (debug_cmd == RUN_CODE)                     /* ������� */
        {  
            dcmotor_start();                                /* ������� */
            g_speed_pid.SetPoint = 30;                      /* ����Ŀ���ٶ� */
            g_run_flag = 1;                                 /* ��ǵ������ */
            debug_send_motorstate(RUN_STATE);               /* �ϴ����״̬�����У� */
        }  
#endif 
        

        LED0 = !LED0;
        vTaskDelay(1000);
    }
}


