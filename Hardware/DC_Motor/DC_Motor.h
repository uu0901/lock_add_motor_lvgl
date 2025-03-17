#ifndef __DC_MOTOR___
#define __DC_MOTOR___
#include "sys.h"
void dcmotor_init(void);
void dcmotor_start(void);
void dcmotor_stop(void);
void dcmotor_dir(uint8_t para);
void dcmotor_speed(uint16_t para);
void motor_pwm_set(float para);
/* ��������ṹ�� */
typedef struct 
{
  uint8_t state;          /*���״̬*/
  float current;          /*�������*/
  float volatage;         /*�����ѹ*/
  float power;            /*�������*/
  float speed;            /*���ʵ���ٶ�*/
  int32_t motor_pwm;      /*���ñȽ�ֵ��С */
} Motor_TypeDef;
#endif

