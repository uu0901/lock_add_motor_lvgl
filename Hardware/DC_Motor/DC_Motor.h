#ifndef __DC_MOTOR___
#define __DC_MOTOR___
#include "sys.h"
void dcmotor_init(void);
void dcmotor_start(void);
void dcmotor_stop(void);
void dcmotor_dir(uint8_t para);
void dcmotor_speed(uint16_t para);
void motor_pwm_set(float para);
/* 电机参数结构体 */
typedef struct 
{
  uint8_t state;          /*电机状态*/
  float current;          /*电机电流*/
  float volatage;         /*电机电压*/
  float power;            /*电机功率*/
  float speed;            /*电机实际速度*/
  int32_t motor_pwm;      /*设置比较值大小 */
} Motor_TypeDef;
#endif

