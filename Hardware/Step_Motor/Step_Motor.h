#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H
#include "sys.h"
void StepMotor_DIR_ENA_Init(void);
void StepMotor_Set_Dir(uint8_t pram);
void StepMotor_Set_En(uint8_t pram);
#endif
