#ifndef __BLDC_MOTOR___
#define __BLDC_MOTOR___
#include "sys.h"

#define CCW                         (1)
#define CW                          (2)
#define RUN                         (1)
#define STOP                        (0)
#define MAX_PWM_DUTY    (((10000) - 1)*0.96) /* 占空比最大限值 9600-1（9599.04）*/
typedef struct {
    __IO uint8_t    run_flag;       /* 运行标志 */
    __IO uint8_t    locked_rotor;   /* 堵转标记 */
    __IO uint8_t    step_sta;       /* 本次霍尔状态 */
    __IO uint8_t    hall_single_sta;/* 单个霍尔状态 */
    __IO uint8_t    hall_sta_edge;  /* 单个霍尔状态跳变 */
    __IO uint8_t    step_last;      /* 上次霍尔状态 */
    __IO uint8_t    dir;            /* 电机旋转方向 */
    __IO int32_t    pos;            /* 电机位置 */
    __IO int32_t    speed;          /* 电机速度 */
    __IO int16_t    current;        /* 电机速度 */
    __IO uint16_t   pwm_duty;       /* 电机占空比 */
    __IO uint32_t   hall_keep_t;    /* 霍尔保持时间 */
    __IO uint32_t   hall_pul_num;   /* 霍尔传感器脉冲数 */
    __IO uint32_t   lock_time;      /* 电机堵转时间 */
    __IO uint32_t   no_single;
    __IO uint32_t   count_j;
} _bldc_obj;
extern _bldc_obj g_bldc_motor1;
        
void ShutDown_Init(void);
void Hall_Init(void);
uint32_t Get_HallSensor(void);
void Motor_Start(void);
void Motor_Stop(void);

#endif
