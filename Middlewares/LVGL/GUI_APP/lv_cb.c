#include "lv_cb.h"
#include "lv_mainstart.h"
#include "lvgl.h"
#include "led.h"
#include "delay.h"
#include "math.h"
#include "advanced_tim1.h"
#include "dc_motor.h"
#include "pid.h"
#include "debug.h"
#include "step_motor.h" 
#include "timer4.h" 
#include "timer8.h"
#include "bldc_motor.h"
#define Password 1234


extern lv_obj_t *btn_enter;      /* 进入按钮 */
void enter_event_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* 获取触发源 */

    if(target == btn_enter)                      /* 进入按钮 */
    {
        Timer4_Init(0xffff - 1, 72 - 1);                         /* 72 000 000 / 72 =  1M hz的计数频率，计数一次为1/1M = 1us,计数1000次为1ms */ //定时器通道TIM4_CH1生成指定脉冲，PUL-接在定时器TIM4_CH1上
        StepMotor_DIR_ENA_Init();                               //初始化DIR-和ENA-输出高电平
        StepMotor_Set_En(1);                       //电机启动

//      g_ccr1 = 50;                            //限制为最高速
        
        
        TIM8_Int_Init();//arr 10000 -1 psc 1-1
        //上桥臂PC6 PC7 PC8
        TIM_CCxCmd(Advanced_TIM, TIM_Channel_1,TIM_CCx_Disable);
        TIM_CCxCmd(Advanced_TIM, TIM_Channel_2,TIM_CCx_Disable);
        TIM_CCxCmd(Advanced_TIM, TIM_Channel_3,TIM_CCx_Disable);
        //下桥臂PA7[更换PB9，因为PA7用作编码器接口了] PB0 [更换PB7，因为PB0用作屏幕背光了]PB1
        GPIO_ResetBits(GPIOB,GPIO_Pin_9);  
        GPIO_ResetBits(GPIOB,GPIO_Pin_7);  
        GPIO_ResetBits(GPIOB,GPIO_Pin_1);  
        //刹车引脚PG15
        ShutDown_Init();//CTRL_SD低电平 SD_IN输出高,H桥芯片刹车
        //霍尔传感器PB10 PB11 PB12
        Hall_Init();
        Refresh_And_CreateScren();
        lv_lock_sys(); // 重新加载主屏幕
    }

}

extern uint8_t my_key;                       //kk缓存按键值
uint8_t last_my_key = 1;                    //kk缓存按键值
extern lv_obj_t *keyboard ;
extern lv_obj_t *textarea;
uint8_t mynummap[]={0,1,2,3,4,5,6,7,8,9};   //我的按键数组
int8_t p=0;                                  //我的按键数组索引
uint8_t mynum=0;                             //我自己的键盘数组保存值
uint8_t NUM  = 0;                            //按键触摸保存值
uint8_t LAST_NUM = 255;                     //上一次的键值
uint8_t OK_FLAG = 0;                        //键盘触摸按下标志位，未用到
uint8_t OK_LEFT = 0;                        //键盘返回按下标志位，未用到，代码只做直接切换屏幕
uint32_t SUM = 0;                           //密码值


extern uint8_t correct_flag;               //密码正确标志位
extern uint8_t incorrect_flag;             //密码不正确标志位
uint8_t i = 0;                              //切换特殊字符标志位
uint8_t pp = 0;                             //触摸键盘数组索引
uint8_t buff[50];                           //触摸键盘数组
uint32_t BackSum = 0;                       //撤回按键的计算时候的缓存数组
int8_t uu;                                  //记录当前键值的数组的索引
uint8_t flag = 0;                           //按键撤回的标志位
uint8_t t = 0;                              //按键撤回的次数
uint8_t temp;                               //按键撤回的次数的缓存变量
uint8_t temp_2;                             //按键撤回的次数的缓存变量
void keyboard_event_cb(lv_event_t *e)
{     
    
    lv_event_code_t code = lv_event_get_code(e);                     /* 获取事件类型 */
    lv_obj_t *target = lv_event_get_current_target(e);              /* 获取触发源 */
    if(target == keyboard)//触发源为键盘，目的是不断的遍历按键my_key
    {
        if(my_key != 0)//判断下次按键按下
        {
            printf("my_key  %d\r\n",my_key);//kk是3个按键的返回值，在lv_port_indev_template中的键盘输入函数中，my_key是其中的一个缓存数组
            switch(my_key)
            {
                printf("my_key  %d\r\n",my_key);//my_key是3个按键的返回值，在lv_port_indev_template中的键盘输入函数中，my_key是其中的一个缓存数组
                case LV_KEY_NEXT:  {p++;if(p>9)p=0; printf("next\r\n"); my_key = 0;break;}//+   LV_KEY_NEXT 用于遍历数组
                case LV_KEY_PREV:   {p--;if(p<0)p=9; printf("prev\r\n"); my_key = 0;break;}//-  LV_KEY_PREV 用于遍历数组
                case LV_KEY_ENTER:  {//确认你所遍历的数组，保存键值到mynum，打印到屏幕上，清空kk用于下一次按键按下，
                                        printf("enter\r\n");  
                                        mynum =mynummap[p];
                                        printf("mynum  %d\r\n",mynum);//my_key是3个按键的返回值，在lv_port_indev_template中的键盘输入函数中，my_key是其中的一个缓存数组

                                        SUM = SUM *10 + mynum;//通过累乘实现保存输入的值
                                        printf("                    SUM= %d\r\n", SUM);
                                        switch(p)
                                        {
                                            case 0:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('0'));my_key = 0;break;}
                                            case 1:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('1'));my_key = 0;break;}
                                            case 2:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('2'));my_key = 0;break;}
                                            case 3:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('3'));my_key = 0;break;}
                                            case 4:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('4'));my_key = 0;break;}
                                            case 5:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('5'));my_key = 0;break;}
                                            case 6:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('6'));my_key = 0;break;}
                                            case 7:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('7'));my_key = 0;break;}
                                            case 8:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('8'));my_key = 0;break;}
                                            case 9:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('9'));my_key = 0;break;}
                                            default:break;
                                        }
                                    }
                default:break;
            }
        }
    }
    if(code == LV_EVENT_VALUE_CHANGED)//触发事件为键值改变
    {
        uint16_t id = lv_btnmatrix_get_selected_btn(target);                /* 获取键盘按钮索引 */
        const char *txt = lv_btnmatrix_get_btn_text(target, id);            /* 获取按钮文本，取指针内容是ASC码的字符 */
        int num = *txt - '0';                                                  //将字符转换成十进制

        //测试用
//        printf("%d\n", *txt);                                                 //直接字符对应的十进制打印
//        printf("%d\n", num);                                                  //将字符转换成十进制来打印
        //测试用
        
        printf("correct_flag = %d          incorrect_flag = %d\r\n", correct_flag, incorrect_flag);
        if(lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_TEXT_LOWER)    //当前键盘模式为小写字母时候，将切换特殊字符按键标志置0
        {
            printf("LV_KEYBOARD_MODE_TEXT_LOWER \r\n");
            i = 0;//将切换特殊字符按键标志置0
        }
        if(lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_SPECIAL)        //当前键盘模式为特殊字符的时候
        {
            printf("LV_KEYBOARD_MODE_SPECIAL \r\n");
            if(i == 0)//特殊字符按键标志置0，忽略此次键值
            {
                if(num == 1) num = 0;   //第一次默认按下，键值赋值为0，因为特殊字符切换按键按下时实际上是返回1，但是我们切换的时候并不想把返回的1存入，所以默认第一次切换特殊字符的第一个值清0
            }
            i = 1;                      //标志赋值1，存入接下来之后的键值
        }

        if( (num<=9 && num >=0) || (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) )//输出数字键盘，数字0到9和键盘撤回
        {
            if(num<=9 && num >=0) 
            {
                NUM  = num;                     //保存键盘按下值为Num
                p = NUM;                        //键盘触摸值 同步  我的按键数组索引
                printf("                        NUM = %d\r\n", NUM);
                SUM = SUM *10 + NUM;            //通过累乘实现保存输入的值
                printf("                        SUM = %d\r\n", SUM);
                if(flag == 0)//撤回标志位为0 代表正常输入
                {
                    buff[pp] = NUM;             //将键值保存到数组buff
                    pp++;                       //索引自增方便下次保存
                }
                else         //撤回标志位为1，代表上一次动作是按键撤回，进入撤回补全
                {
                    //撤回2次
                    //t = 2     temp=2       temp_2 = 2
                    //temp=1    操作数组      temp=0    操作数组    退出函数
                    
                    temp = t;                   //撤回次数赋值给temp
                    temp_2 = t;                 //撤回次数赋值给temp_2
                    
                    temp --;                    //temp自减是因为记录下撤回的次数，在这里就写回去同样的次数，当temp减到0就意味着补全了数组，撤回标志位置0，退出撤回补全
                    buff[pp - 1*temp_2] = NUM;//将键值保存到数组buff，pp-1同理上面，因为NUM存入数组后自动偏移了1方便下次写入，但这里需要减去1找到对应的元素进行写入，后面的*temp2是为了偏移写入，因为我们撤回了多少次就偏移多少次写入
                    pp++;                        //索引自增方便下次保存
                    temp_2 ++;                  //temp_2自增是因为要依次写进去数组，撤回多少次，就写回去多少次，因此在buff里面累乘了进行索引偏移
                }   
                if(temp == 0 )  {flag = 0;}     //撤回标志位清0,退出撤回补全
                //打印数组测试，uint32_t   SUM         最大10位数  保存到buff就只有0~9
                printf("                        buff[0] = %d\r\n", buff[0]);
                printf("                        buff[1] = %d\r\n", buff[1]);
                printf("                        buff[2] = %d\r\n", buff[2]);
                printf("                        buff[3] = %d\r\n", buff[3]);
                printf("                        buff[4] = %d\r\n", buff[4]);
                printf("                        buff[5] = %d\r\n", buff[5]);
                printf("                        buff[6] = %d\r\n", buff[6]);
                printf("                        buff[7] = %d\r\n", buff[7]);
                printf("                        buff[8] = %d\r\n", buff[8]);
                printf("                        buff[9] = %d\r\n", buff[9]);

                if(LAST_NUM != NUM || LAST_NUM ==NUM )//当前值和上一次值不一样，说明键盘按下，将最后一次按键按下的索引赋值给uu，uu是在键盘撤销里做动作
                {
                    //测试
                    static uint8_t a = 0;
                    a++;
                    printf("a=%d\r\n\r\n",a);
                    //测试
                    uu = pp - 1;//这里减1是因为这段函数到这里时，pp已经自增了1，自增是为了方便存放数组，但是这里是为了传递数组给键盘撤销做动作，所以要索引回去，因此要减1
                }
                
                LAST_NUM = NUM;//保存键值，记录旧值
            }
            if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)       /* 判断是不是<x图标被按下 */
            {
                flag = 1;//撤回标志位置1
                t ++;      //记录撤回次数
                printf("BACKSPACE\r\n");
                if(uu >= 0)//数组索引限位，只有索引在0以上，才能做SUM减法
                {     
                    printf("                    uu= %d\r\n", uu);                    
                    printf("                        buff[uu] = %d\r\n", buff[uu]);
                    BackSum =  SUM - buff[uu];//11223344--->11223340
                    BackSum =  BackSum/10;//1122334
                    SUM = BackSum;//1122334
                }
                if(SUM == 0)                            //将SUM减到0后
                {
                    uu = -1;                            //索引限位为-1，目的是当SUM减到0后再自减，不会继续做SUM减法，因为不限位，还是会做上面的减法导致出现问题
                    memset(buff, 0, sizeof(buff));     //清空数组
                    pp = 0;                             //将键盘保存索引赋值为0，确保清空后。键盘键值保存是从buff[0]到buff[9]
                }
                uu --;                                  //每按下一次撤销，做完减法后就对索引减减，方便下次做减法
                printf("                    SUM= %d\r\n", SUM);                
                printf("                    uu= %d\r\n", uu);
            }

        }
 
        if(strcmp(txt, LV_SYMBOL_KEYBOARD) == 0)                            /* 判断是不是键盘图标被按下 */
        {
            printf("LV_SYMBOL_KEYBOARD \r\n");
            if(lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_NUMBER)     /* 获取当前键盘模式，判断是否为数字模式 */
            {
                lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_TEXT_LOWER);  /* 如果是数字模式，则切换为小写字母模式 */
                printf("Switch to the LV_KEYBOARD_MODE_TEXT_LOWER\r\n");
                i = 0;
            }
            else
            {
                lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_NUMBER);      /* 不是数字模式，则切换为数字模式 */   
                printf("Switch to the LV_KEYBOARD_MODE_NUMBER\r\n");
            }
        }
        
        if(strcmp(txt, LV_SYMBOL_OK) == 0)           /* 判断是不是√图标被按下 */
        {
            printf("LV_SYMBOL_OK\r\n");
            printf("                    SUM= %d\r\n", SUM);
            OK_FLAG = 1;//未使用
            if(SUM == Password)
            {
                correct_flag = 1;           //密码正确标志位置1
                incorrect_flag = 0;         //密码不正确标志位置0
                printf("Success!     correct_flag = %d", correct_flag);
                SUM = 0;                    //密码清0
                p = 0;                      //我的按键数组索引赋值为0
            }
            else
            {
                incorrect_flag = 1;//密码不正确标志位置1
                correct_flag = 0;//密码正确标志位置0
                printf("Fail!    incorrect_flag = %d\r\n", incorrect_flag);
                SUM = 0;//密码清0
                p = 0;//我的按键数组索引赋值为0
            }
            Refresh_And_CreateScren();
            lv_loading(); // 重新加载主屏幕
        }
        if(strcmp(txt, LV_SYMBOL_LEFT) == 0)         /* 判断是不是<图标被按下 */
        {
            printf("LV_SYMBOL_LEFT\r\n");   
            OK_LEFT = 1;//未使用
            Refresh_And_CreateScren();
            lv_mainstart(); // 重新加载主屏幕
        }

    }
}  

extern lv_obj_t *DC_Motor_label_speed;       /* 速度提示文本 */
extern lv_obj_t *DC_Motor_btn_speed_up;      /* 加速按钮 */
extern lv_obj_t *DC_Motor_btn_speed_down;    /* 减速按钮 */
extern lv_obj_t *DC_Motor_btn_stop;          /* 急停按钮 */
extern lv_obj_t *DC_Motor_btn_back;          /* 返回按钮 */
extern int32_t DC_Motor_speed_val;       /* 速度值 */

extern lv_obj_t *Step_Motor_label_speed;       /* 速度提示文本部件 */
extern lv_obj_t *Step_Motor_btn_speed_up;      /* 加速按钮部件*/
extern lv_obj_t *Step_Motor_btn_speed_down;    /* 减速按钮部件*/
extern lv_obj_t *Step_Motor_btn_stop;          /* 急停按钮部件*/
extern lv_obj_t *Step_Motor_btn_back;          /* 返回按钮部件*/
extern int32_t Step_Motor_speed_val;       /* 速度值部件*/

extern lv_obj_t *Bldc_Motor_label_speed;       /* 速度提示文本部件 */
extern lv_obj_t *Bldc_Motor_btn_speed_up;      /* 加速按钮部件*/
extern lv_obj_t *Bldc_Motor_btn_speed_down;    /* 减速按钮部件*/
extern lv_obj_t *Bldc_Motor_btn_stop;          /* 急停按钮部件*/
extern lv_obj_t *Bldc_Motor_btn_back;          /* 返回按钮部件*/
extern int32_t Bldc_Motor_speed_val;       /* 速度值部件*/

extern lv_obj_t *Servo_label_speed;       /* 速度提示文本部件 */
extern lv_obj_t *Servo_btn_speed_up;      /* 加速按钮部件*/
extern lv_obj_t *Servo_btn_speed_down;    /* 减速按钮部件*/
extern lv_obj_t *Servo_btn_stop;          /* 急停按钮部件*/
extern lv_obj_t *Servo_btn_back;          /* 返回按钮部件*/
extern int32_t Servo_speed_val;       /* 速度值部件*/

extern uint8_t  g_run_flag;  /*电机参数变量*/
extern Motor_TypeDef MotorData;  /*电机参数变量*/

uint16_t g_ccr1 = 0;                //g_ccr150为最高速     g_ccr1 500为最低速
uint8_t a = 0;                        //赋初值标志位
uint8_t changedir = 0;              //允许改变方向
uint16_t dir = 0;                   //初始化为正转
uint8_t Servo_val = 1;

_bldc_obj g_bldc_motor1 = {STOP,0,0,CCW,0,0,0,0,0,0};   /* 电机结构体初始值 */
int16_t pwm_duty_temp = 0;
void sys_btn_event_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* 获取触发源 */
    //直流有刷电机速度环
    if(target == DC_Motor_btn_speed_up)                      /* 加速按钮 */
    {
//            DC_Motor_speed_val += 30;
            g_run_flag = 1;                 /* 标记电机启动 */
            g_speed_pid.SetPoint += 30;
            if( g_speed_pid.SetPoint == 0)
            {
                 dcmotor_stop();            /* 停止则立刻响应 */
                 MotorData.motor_pwm = 0;
                 motor_pwm_set(MotorData.motor_pwm);  /* 设置电机方向、转速 */ 
            }
            else
            {
                 dcmotor_start();                    /* 开启电机 */
                 if(g_speed_pid.SetPoint >= 300)     /* 限速 */
                 {
                     g_speed_pid.SetPoint = 300;
                 }
#if DEBUG_ENABLE  
                debug_send_motorstate(RUN_STATE);      /* 上传电机状态（运行） */                 
#endif                 
            }     
            
        LED1 = !LED1;
    }
    else if(target == DC_Motor_btn_speed_down )               /* 减速按钮 */
    {
//        DC_Motor_speed_val -= 30;
            g_run_flag = 1;                 /* 标记电机启动 */
            g_speed_pid.SetPoint -= 30;
            if( g_speed_pid.SetPoint == 0)
            {
                 dcmotor_stop();            /* 停止则立刻响应 */
                 MotorData.motor_pwm = 0;
                 motor_pwm_set(MotorData.motor_pwm);  /* 设置电机方向、转速 */ 
            }
            else
            {
                 dcmotor_start();                    /* 开启电机 */
                 if(g_speed_pid.SetPoint <= -300)     /* 限速 */
                 {
                     g_speed_pid.SetPoint = -300;
                 }
#if DEBUG_ENABLE  
                debug_send_motorstate(RUN_STATE);      /* 上传电机状态（运行） */   
 #endif 
             }                 
           
        LED1 = !LED1;
    }
    else if(target == DC_Motor_btn_stop)                     /* 急停按钮 */
    {
//        DC_Motor_speed_val = 0;
            dcmotor_stop();                                 /* 停止电机 */
            pid_init();                                     /* pid参数重置 */
            g_run_flag = 0;                                  /* 标记电机停止 */
            MotorData.motor_pwm = 0;
            motor_pwm_set(MotorData.motor_pwm);          /* 设置电机转向、速度 */
#if DEBUG_ENABLE               
            debug_send_motorstate(BREAKED_STATE);           /* 上传电机状态（刹车） */
            debug_send_initdata(TYPE_PID1, (float *)(&g_speed_pid.SetPoint), KP, KI, KD);
#endif                        
        LED1 = !LED1;
    }
    else if(target == DC_Motor_btn_back)                     /* 返回按钮 */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // 重新加载主屏幕
        i = 0;
        return;
    }
    //Step_Motor
    if(target == Step_Motor_btn_speed_up)                      /* 加速按钮 */
    {
        changedir = 0;                              //允许电机改变方向  changedir = 0允许改变方向       changedir = 1   不允许改变方向
        StepMotor_Set_En(1);                       //电机启动
        if(a == 0)  g_ccr1 = 500;                   //给g_ccr1初值 默认最低速
        
        
        g_ccr1 -= 50;                                /* 加速 */     
        a = 1;                                       //停止赋初值
        
        if(g_ccr1 == 0)                             //到达最高速再按下，限制为最高速
        { 
            g_ccr1 = 50;                            //限制为最高速
        }
        printf("g_ccr=%d\r\n", g_ccr1);
        LED1 = !LED1;
    }
    else if(target == Step_Motor_btn_speed_down && changedir == 0)               /* 减速按钮 */
    {
//        if(changedir == 0) return;
        StepMotor_Set_En(1);                        //电机启动
        if(a == 0)  g_ccr1 = 500;                   //给g_ccr1初值 默认最低速

        g_ccr1 += 50;                               /* 减速 */
        a = 1;                                      //停止赋初值
        
        if(g_ccr1 == 550)                            //到达最低速再按下，改变旋转方向，且只改变一次
        { 
            g_ccr1 = 500;                           //限制为最低速
            dir = !dir;                             //电机方向取反
            StepMotor_Set_Dir(dir);                //改变电机方向
            changedir = 1;                          //电机改变方向标志位，目的是只改变一次方向
        }
        printf("g_ccr=%d\r\n", g_ccr1);
        LED1 = !LED1;
    }
    else if(target == Step_Motor_btn_stop)                     /* 急停按钮 */
    {
        StepMotor_Set_En(0);
        a = 0;                                               //赋初值标志位
        LED1 = !LED1;
    }
    else if(target == Step_Motor_btn_back)                     /* 返回按钮 */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // 重新加载主屏幕
        i = 0;
        return;
    }
    //Bldc_Motor
    else if(target == Bldc_Motor_btn_speed_up)                      /* 加速按钮 */
    {
        Motor_Start();                       /* 运行电机 */
        g_bldc_motor1.run_flag = RUN;       /* 标记运行 */
        pwm_duty_temp += 500;
        if(pwm_duty_temp >= MAX_PWM_DUTY)//>9600/2 
        {
            pwm_duty_temp =  MAX_PWM_DUTY;//=9600/2
        }
        if(pwm_duty_temp >= 0)//>=0 正转,此处要加等号，不然=0时电机的状态无法判断
        {
            g_bldc_motor1.dir = CW;//设置正转
            g_bldc_motor1.pwm_duty = pwm_duty_temp;//CCR赋值
        } 
        else//<0 反转
        {
            g_bldc_motor1.dir = CCW;//设置反转
            g_bldc_motor1.pwm_duty = -pwm_duty_temp;//CCR取反赋值
        }

        LED1 = !LED1;
    }
    else if(target == Bldc_Motor_btn_speed_down)               /* 减速按钮 */
    {
        Motor_Start();                       /* 运行电机 */
        g_bldc_motor1.run_flag = RUN;       /* 标记运行 */
        pwm_duty_temp -= 500;
        if(pwm_duty_temp <= -MAX_PWM_DUTY)//<=-9600/2
        {
            pwm_duty_temp =  -MAX_PWM_DUTY;//=-9600/2
        }
        if(pwm_duty_temp <= 0)//<=0反转，此处要加等号，不然=0时电机的状态无法判断
        {
            g_bldc_motor1.dir = CCW;//设置反转
            g_bldc_motor1.pwm_duty = -pwm_duty_temp;//CCR取反赋值
        } 
        else //>0正转
        {
            g_bldc_motor1.dir = CW;//设置正转
            g_bldc_motor1.pwm_duty = pwm_duty_temp;//CCR赋值
        }


        LED1 = !LED1;
    }
    else if(target == Bldc_Motor_btn_stop)                     /* 急停按钮 */
    {
        Motor_Stop();                       /* 停止电机 */
        g_bldc_motor1.run_flag = STOP;   /* 标记停机 */
        g_bldc_motor1.pwm_duty = 0;      //占空比清0
        pwm_duty_temp = 0;                /* 数据清0 */
        LED1 = !LED1;
    }
    else if(target == Bldc_Motor_btn_back)                     /* 返回按钮 */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // 重新加载主屏幕
        i = 0;
        return;
    }
    //Servo
    else if(target == Servo_btn_speed_up)                      /* 加速按钮 */
    {
        Servo_val++;
        if(Servo_val == 6)
        {
            Servo_val = 1;
        }

        LED1 = !LED1;
    }
    else if(target == Servo_btn_speed_down)               /* 减速按钮 */
    {
        Servo_val--;
        if(Servo_val == 0)
        {
            Servo_val = 5;
        }

        LED1 = !LED1;
    }
    else if(target == Servo_btn_stop)                     /* 急停按钮 */
    {
        Servo_speed_val = 0;
        LED1 = !LED1;
    }
    else if(target == Servo_btn_back)                     /* 返回按钮 */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // 重新加载主屏幕
        i = 0;
        return;
    }
        TIM_SetCompare2(TIM5, 5*Servo_val);                       // 5*1 5*2 5*3 5*4 5*5 ---->  5*1 5*2 5*3 5*4 5*5      
        lv_label_set_text_fmt(DC_Motor_label_speed, "DC_Motor_Speed : %3d RPM", (int16_t)g_speed_pid.SetPoint);    /* 更新速度值 */
        lv_label_set_text_fmt(Step_Motor_label_speed, "Step_Motor_g_ccr1: %d", g_ccr1);    /* 更新速度值 */
        lv_label_set_text_fmt(Bldc_Motor_label_speed, "Bldc_Motor_PWM_Duty:%.1f%%", (float)((g_bldc_motor1.pwm_duty/MAX_PWM_DUTY)*100));    /* 更新速度值 */
        lv_label_set_text_fmt(Servo_label_speed, "Servo_Speed : %d", Servo_val);    /* 更新速度值 */
}
extern int16_t val;                         /* 当前值 */
extern lv_obj_t *label_per;                     /* 百分比标签 */
extern lv_obj_t *bar;                           /* 进度条 */
extern lv_obj_t *correct_bmp;
extern lv_obj_t *incorrect_bmp;
extern uint8_t loading_finish_flag;        //加载完成标志位
extern uint8_t pass_flag ;              //通过标志位
extern uint8_t nopass_flag;            //不通过标志位
extern lv_timer_t *timer;

void timer_cb(lv_timer_t *timer)
{
    if(val < 100)                                                           /* 当前值小于100 */
    {
        val ++;
        lv_bar_set_value(bar, val, LV_ANIM_ON);                             /* 设置当前值 */
        lv_label_set_text_fmt(label_per, "%d %%", lv_bar_get_value(bar));   /* 获取当前值，更新显示 */
    }
    else                                                                    /* 当前值大于等于100 */
    {
        if(loading_finish_flag == 0)//进度加载完成，进行校验
        {
            if(correct_flag == 1)//密码正确
            {
                lv_label_set_text(label_per, "finished!");                          /* 加载完成 */
                //显示正确图标
                lv_obj_clear_flag(correct_bmp, LV_OBJ_FLAG_HIDDEN);
                lv_obj_align(correct_bmp,LV_ALIGN_CENTER,180, 30);
                //校验完成
                loading_finish_flag = 1;

            }
            else if(incorrect_flag == 1)//密码错误
            {
                lv_label_set_text(label_per, "finished!");                          /* 加载完成 */
                //显示不正确图标
                lv_obj_clear_flag(incorrect_bmp, LV_OBJ_FLAG_HIDDEN);
                lv_obj_align(incorrect_bmp,LV_ALIGN_CENTER,180, 30);
                //校验完成
                loading_finish_flag = 1;
  
            }

        }
       else if(loading_finish_flag == 1)//校验完成，退出切换屏幕
       {
            if(correct_flag == 1)//切换系统页面
            {
                delay_ms(1000);
                Refresh_And_CreateScren();
                lv_menu();
                loading_finish_flag = 2;
                lv_timer_del(timer);
                correct_flag = 0;               //清除标志位，退出
            }  
            else if(incorrect_flag == 1)//切换键盘页面
            {
                delay_ms(1000);
                Refresh_And_CreateScren();
                lv_lock_sys();
                loading_finish_flag = 2;
                lv_timer_del(timer);
                incorrect_flag = 0;             //清除标志位，退出
            }                    
       }
    }
}
extern lv_obj_t *list;              /* 定义列表 */
extern lv_obj_t *list_label;        /* 定义标签 */
void list_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *list_btn = lv_event_get_target(e);                                   /* 获取触发源 */
//    lv_obj_add_state(list_btn, LV_STATE_FOCUS_KEY);                                /* 添加状态（聚焦） */
    lv_label_set_text(list_label, lv_list_get_btn_text(list, list_btn));           /* 获取按钮文本并显示 */

    const char *txt = lv_list_get_btn_text(list, list_btn);
    printf("%s\r\n", txt);//如果你有一个指向字符串的指针，你可以使用printf函数配合格式化字符串"%s"来打印这个字符串。
    //比较两个字符串是否相等不能简单地使用 == 操作符，
    //因为这只会比较两个指针是否指向同一个内存位置，而不是比较字符串的内容。要比较字符串的内容，你需要使用标准库函数 strcmp 或者逐字符比较。
    if (strcmp(txt, "MotorControl") == 0)
    {
        Refresh_And_CreateScren();
        lv_motor_control();
    }
    else if (strcmp(txt, "back") == 0)
    {
        Refresh_And_CreateScren();
        lv_lock_sys();
    }

}

