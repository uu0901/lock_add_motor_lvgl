#ifndef __BLDC_MOTOR___
#define __BLDC_MOTOR___
#include "sys.h"

#define CCW                         (1)
#define CW                          (2)
#define RUN                         (1)
#define STOP                        (0)
#define MAX_PWM_DUTY    (((10000) - 1)*0.96) /* ռ�ձ������ֵ 9600-1��9599.04��*/
typedef struct {
    __IO uint8_t    run_flag;       /* ���б�־ */
    __IO uint8_t    locked_rotor;   /* ��ת��� */
    __IO uint8_t    step_sta;       /* ���λ���״̬ */
    __IO uint8_t    hall_single_sta;/* ��������״̬ */
    __IO uint8_t    hall_sta_edge;  /* ��������״̬���� */
    __IO uint8_t    step_last;      /* �ϴλ���״̬ */
    __IO uint8_t    dir;            /* �����ת���� */
    __IO int32_t    pos;            /* ���λ�� */
    __IO int32_t    speed;          /* ����ٶ� */
    __IO int16_t    current;        /* ����ٶ� */
    __IO uint16_t   pwm_duty;       /* ���ռ�ձ� */
    __IO uint32_t   hall_keep_t;    /* ��������ʱ�� */
    __IO uint32_t   hall_pul_num;   /* ���������������� */
    __IO uint32_t   lock_time;      /* �����תʱ�� */
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
