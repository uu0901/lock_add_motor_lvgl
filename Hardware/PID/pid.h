/**
 ****************************************************************************************************
 * @file        pid.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       PID算法代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com/forum.php
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 *
 ****************************************************************************************************
 */
#ifndef __PID_H
#define __PID_H

#include "sys.h"

/******************************************************************************************/

#define  INCR_LOCT_SELECT  0        /* 0:位置式PID ，1：增量式PID */

#if INCR_LOCT_SELECT

/* 增量式PID参数 */
#define  KP      8.50f               /* P参数*/
#define  KI      5.00f               /* I参数*/
#define  KD      0.10f               /* D参数*/
#define  SMAPLSE_PID_SPEED  50       /*采样周期 单位ms*/
#else

/* 位置式PID参数 */
#define  KP      10.00f               /* P参数*/
#define  KI      8.50f               /* I参数*/
#define  KD      0.5f                /* D参数*/
#define  SMAPLSE_PID_SPEED  50       /*采样周期 单位ms*/
#endif

/*PID结构体*/
typedef struct
{
    __IO float  SetPoint;            /* 目标值 */
    __IO float  ActualValue;         /* 期望输出值 */
    __IO float  SumError;            /* 累计偏差 */
    __IO float  Proportion;          /* 比例常数 P */
    __IO float  Integral;            /* 积分常数 I */
    __IO float  Derivative;          /* 微分常数 D */
    __IO float  Error;               /* Error[1] */
    __IO float  LastError;           /* Error[-1] */
    __IO float  PrevError;           /* Error[-2] */
} PID_TypeDef;

extern PID_TypeDef  g_speed_pid;     /*速度环PID参数结构体*/

/******************************************************************************************/

void pid_init(void);                 /* pid初始化 */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value);      /* pid闭环控制 */

#endif
