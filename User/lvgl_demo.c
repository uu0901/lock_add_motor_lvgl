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
extern uint8_t  g_run_flag;  /*电机参数变量*/
extern Motor_TypeDef MotorData;  /*电机参数变量*/
/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO     1           /* 任务优先级 */
#define START_STK_SIZE      128         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;         /* 任务句柄 */
void start_task(void *pvParameters);    /* 任务函数 */

/* LV_DEMO_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LV_DEMO_TASK_PRIO   3           /* 任务优先级 */
#define LV_DEMO_STK_SIZE    1024        /* 任务堆栈大小 */
TaskHandle_t LV_DEMOTask_Handler;       /* 任务句柄 */
void lv_demo_task(void *pvParameters);  /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO       4           /* 任务优先级 */
#define LED_STK_SIZE        128         /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;           /* 任务句柄 */
void led_task(void *pvParameters);      /* 任务函数 */
/******************************************************************************************************/

/**
 * @brief       lvgl_demo入口函数
 * @param       无
 * @retval      无
 */
void lvgl_demo(void)
{
    lv_init();                                          /* lvgl系统初始化 */
    lv_port_disp_init();                                /* lvgl显示接口初始化,放在lv_init()的后面 */
    lv_port_indev_init();                               /* lvgl输入接口初始化,放在lv_init()的后面 */

    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传递给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

    vTaskStartScheduler();                              /* 开启任务调度 */
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建LVGL任务 */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    taskEXIT_CRITICAL();            /* 退出临界区 */
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
}

/**
 * @brief       LVGL运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lv_demo_task(void *pvParameters)
{
    lv_mainstart();  /* 测试的demo */
    
    while(1)
    {
        lv_timer_handler(); /* LVGL计时器 */
        vTaskDelay(5);
    }
}

/**
 * @brief       系统再运行
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
extern uint8_t a;
extern bool touchpad_is_pressed(void);
extern uint16_t g_ccr1 ;                //g_ccr150为最高速     g_ccr1 500为最低速
void led_task(void *pvParameters)
{
    
    while(1)
    {
#if DEBUG_ENABLE
       /* 查询接收PID助手的PID参数 */
        debug_receive_pid(TYPE_PID1, (float *)&g_speed_pid.Proportion,(float *)&g_speed_pid.Integral, (float *)&g_speed_pid.Derivative);

        debug_set_point_range(300, -300, 300);              /* 设置目标调节范围，速度-300~300，突变不允许超过300 */
        
        debug_cmd = debug_receive_ctrl_code();              /* 读取命令 */
        
        if (debug_cmd == BREAKED)                           /* 电机刹车 */
        {
            dcmotor_stop();                                 /* 停止电机 */
            pid_init();                                     /* pid参数重置 */
            g_run_flag = 0;                                  /* 标记电机停止 */
            MotorData.motor_pwm = 0;
            motor_pwm_set(MotorData.motor_pwm);          /* 设置电机转向、速度 */
            debug_send_motorstate(BREAKED_STATE);           /* 上传电机状态（刹车） */
            debug_send_initdata(TYPE_PID1, (float *)(&g_speed_pid.SetPoint), KP, KI, KD);
        } 
        else if (debug_cmd == RUN_CODE)                     /* 电机运行 */
        {  
            dcmotor_start();                                /* 开启电机 */
            g_speed_pid.SetPoint = 30;                      /* 设置目标速度 */
            g_run_flag = 1;                                 /* 标记电机启动 */
            debug_send_motorstate(RUN_STATE);               /* 上传电机状态（运行） */
        }  
#endif 
        

        LED0 = !LED0;
        vTaskDelay(1000);
    }
}


