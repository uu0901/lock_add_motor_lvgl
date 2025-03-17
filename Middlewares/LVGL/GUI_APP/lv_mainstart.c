#include "lv_mainstart.h"
#include "lvgl.h"
#include <stdio.h>
#include "lv_cb.h"
#if (1 != LV_FONT_MONTSERRAT_10 || 1 != LV_FONT_MONTSERRAT_14 || 1 != LV_FONT_MONTSERRAT_20)
    #error Please Make Sure Enable LV_FONT_MONTSERRAT_10 & LV_FONT_MONTSERRAT_14 & LV_FONT_MONTSERRAT_20
#endif
static const lv_font_t* font;       /* 定义字体 */


/* 获取当前活动屏幕的宽高 */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())
extern lv_indev_t * indev_keypad;      // 键盘输入设备

//------------------------------------------------------------
//******************enter_event_cb          lv_mainstart
lv_obj_t *btn_enter;      /* 进入按钮部件 */
//******************enter_event_cb      
//------------------------------------------------------------

//------------------------------------------------------------
//******************keyboard_event_cb       lv_lock_sys
lv_obj_t *keyboard;             //键盘部件
lv_obj_t *textarea;             //文本框部件
extern uint8_t my_key;
//******************keyboard_event_cb
//------------------------------------------------------------

//------------------------------------------------------------
//******************sys_btn_event_cb        lv_sys
lv_obj_t *DC_Motor_label_speed;       /* 速度提示文本部件 */
lv_obj_t *DC_Motor_btn_speed_up;      /* 加速按钮部件*/
lv_obj_t *DC_Motor_btn_speed_down;    /* 减速按钮部件*/
lv_obj_t *DC_Motor_btn_stop;          /* 急停按钮部件*/
lv_obj_t *DC_Motor_btn_back;          /* 返回按钮部件*/
int32_t DC_Motor_speed_val = 0;       /* 速度值部件*/

lv_obj_t *Step_Motor_label_speed;       /* 速度提示文本部件 */
lv_obj_t *Step_Motor_btn_speed_up;      /* 加速按钮部件*/
lv_obj_t *Step_Motor_btn_speed_down;    /* 减速按钮部件*/
lv_obj_t *Step_Motor_btn_stop;          /* 急停按钮部件*/
lv_obj_t *Step_Motor_btn_back;          /* 返回按钮部件*/
int32_t Step_Motor_speed_val = 0;       /* 速度值部件*/

lv_obj_t *Bldc_Motor_label_speed;       /* 速度提示文本部件 */
lv_obj_t *Bldc_Motor_btn_speed_up;      /* 加速按钮部件*/
lv_obj_t *Bldc_Motor_btn_speed_down;    /* 减速按钮部件*/
lv_obj_t *Bldc_Motor_btn_stop;          /* 急停按钮部件*/
lv_obj_t *Bldc_Motor_btn_back;          /* 返回按钮部件*/
int32_t Bldc_Motor_speed_val = 0;       /* 速度值部件*/

lv_obj_t *Servo_label_speed;       /* 速度提示文本部件 */
lv_obj_t *Servo_btn_speed_up;      /* 加速按钮部件*/
lv_obj_t *Servo_btn_speed_down;    /* 减速按钮部件*/
lv_obj_t *Servo_btn_stop;          /* 急停按钮部件*/
lv_obj_t *Servo_btn_back;          /* 返回按钮部件*/
int32_t Servo_speed_val = 0;       /* 速度值部件*/

//******************sys_btn_event_cb        lv_sys
//------------------------------------------------------------

//------------------------------------------------------------
//******************timer_cb        lv_loading
int16_t val = 0;                            /* 当前值*/
lv_obj_t *label_per;                     /* 百分比标签部件 */
lv_obj_t *bar;                           /* 进度条部件 */
lv_obj_t *correct_bmp;                 //正确图片部件
lv_obj_t *incorrect_bmp;                 //错误图片部件
uint8_t loading_finish_flag = 0;        //加载完成标志位
uint8_t pass_flag = 0;              //通过标志位
uint8_t nopass_flag = 0;            //不通过标志位
lv_timer_t *timer;                      //定时器部件
uint8_t correct_flag = 0;               //密码正确标志位
uint8_t incorrect_flag = 0;             //密码不正确标志位
LV_IMG_DECLARE (correct);             //声明正确图片
LV_IMG_DECLARE (incorrect);             //声明错误图片
//******************timer_cb        lv_loading
//------------------------------------------------------------

//------------------------------------------------------------
//******************list_btn_event_cb        lv_menu
lv_obj_t *list;              /* 定义列表 */
lv_obj_t *list_label;        /* 定义标签 */
//------------------------------------------------------------
//******************list_btn_event_cb        lv_menu


void lv_mainstart(void)//主界面
{
    /* 根据活动屏幕宽度选择字体 */
    if (scr_act_width() <= 320)
    {
        font = &lv_font_montserrat_10;
    }
    else if (scr_act_width() <= 480)
    {
        font = &lv_font_montserrat_14;
    }
    else
    {
        font = &lv_font_montserrat_20;
    }
    
    /*******************创建欢迎标签*******************/
    lv_obj_t * label_welcome = lv_label_create(lv_scr_act());                              
    lv_obj_set_style_text_font(label_welcome, font, LV_PART_MAIN);                   
    lv_label_set_text(label_welcome, "Welcome! Please Press The Button To Enter The System");
    lv_label_set_long_mode(label_welcome, LV_LABEL_LONG_CLIP);//长文本模式
    lv_obj_align(label_welcome, LV_ALIGN_CENTER, 0, -scr_act_height() / 3);        
    
    
    /*******************创建进入按钮*******************/
    btn_enter = lv_btn_create(lv_scr_act());                                   
    lv_obj_set_size(btn_enter, scr_act_width() / 4, scr_act_height() / 6);    
    lv_obj_set_align(btn_enter, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(btn_enter, lv_color_hex(0xde4d3e), LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn_enter, enter_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* label = lv_label_create(btn_enter);                               
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                         
    lv_label_set_text(label, "Enter");                                           
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                     


    lv_group_focus_obj(btn_enter);    // 设置按钮为当前聚焦的对象
    lv_group_t *g = lv_group_create();//定义并创建组

    lv_group_add_obj(g, btn_enter);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g);//关联按键和组

//    lv_motor_control();


}
void lv_lock_sys(void)//键盘锁界面
{

    my_key = 0;//键值赋初值0，方便切换页面通过按键写入文本框
    correct_flag = 0;//密码正确标志位赋初值0，方便加载完成进行显示
    incorrect_flag = 0;//密码不正确标志位赋初值0，方便加载完成进行显示
    /*******************创建文本框*******************/
    textarea = lv_textarea_create(lv_scr_act());                              /* 创建文本框 */
    lv_obj_set_size(textarea, scr_act_width() - 10, scr_act_height() / 2 - 10);         /* 设置大小 */
    lv_obj_align(textarea, LV_ALIGN_TOP_MID, 0, 0);                                     /* 设置位置 */

    /*******************创建键盘*******************/
    keyboard = lv_keyboard_create(lv_scr_act());                              /* 创建键盘 */
    lv_keyboard_set_textarea(keyboard, textarea);                                       /* 关联键盘和文本框 */

    //这里并没有让按键输入添加到组，因为这里没办法对键盘部件的内部键值进行索引，所以放弃了这个功能，在回调函数里通过LV_EVENT_ALL响应到按键进行手动赋值到文本

//    lv_group_t *g1 = lv_group_create();//定义并创建组
////    lv_group_set_default(g1);//设置默认组   --如果部件已加入默认组，则使用这个配置
//    lv_group_add_obj(g1, keyboard);//添加部件到组--如果部件未加入默认组，则使用这个配置
//    lv_indev_set_group(indev_keypad, g1);//关联按键和组
////    // 设置焦点到键盘部件
//    lv_group_focus_obj(keyboard);
//    lv_group_focus_next(g1);

    lv_obj_add_event_cb(keyboard, keyboard_event_cb, LV_EVENT_CLICKED, NULL);     /* 设置键盘事件回调 */
    lv_obj_add_event_cb(keyboard, keyboard_event_cb, LV_EVENT_ALL, NULL);                /* 设置键盘事件回调 */
}
void lv_loading(void)//加载界面
{
    val = 0 ;               /* 当前值赋值0，因为切换了页面，0才能正常进入回调函数进行递增*/
    loading_finish_flag = 0;      //图片正确加载标志位赋值0，因为切换了页面，0才能正常进入回调函数进行显示
    
    /*******************创建圆弧*******************/
    lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 2000, 60);
    lv_obj_set_size(spinner, 120, 120);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, -scr_act_height() / 4);         

    /*******************创建加载标签*******************/
    lv_obj_t* label_spinner = lv_label_create(lv_scr_act());                  
    lv_label_set_text(label_spinner, "Checking Password, Please Wait...");      
    lv_obj_align(label_spinner,LV_ALIGN_CENTER,0, -10);                        

    /*******************创建百分比标签*******************/
    label_per = lv_label_create(lv_scr_act());
    lv_label_set_text(label_per, "%0");
    lv_obj_set_style_text_font(label_per, font, LV_STATE_DEFAULT);
    lv_obj_align(label_per, LV_ALIGN_CENTER, 0, 70 );
    
    /*******************创建风格*******************/
    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);

    /*******************创建进度条*******************/
    bar = lv_bar_create(lv_scr_act());                         
    lv_obj_align(bar,LV_ALIGN_CENTER,0, 30);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 250, 20);                               /* 设置大小 */
    lv_obj_set_style_anim_time(bar, 100, LV_STATE_DEFAULT);  /* 设置动画时间 */
    timer = lv_timer_create(timer_cb, 100, NULL);             /* 初始化定时器 */

    /*******************创建正确图片*******************/
    correct_bmp = lv_img_create(lv_scr_act());
    lv_img_set_src(correct_bmp, &correct);
    lv_obj_add_flag(correct_bmp, LV_OBJ_FLAG_HIDDEN);
    
    /*******************创建错误图片*******************/
    incorrect_bmp = lv_img_create(lv_scr_act());
    lv_img_set_src(incorrect_bmp, &incorrect);
    lv_obj_add_flag(incorrect_bmp, LV_OBJ_FLAG_HIDDEN);
}


void lv_menu(void)//菜单选择界面
{
    lv_group_t *g3 = lv_group_create();//定义并创建组

    /* 创建左侧矩形背景 */
    lv_obj_t* obj_left = lv_obj_create(lv_scr_act());                               /* 创建一个基础对象 */
    lv_obj_set_width(obj_left, scr_act_width() * 0.7);                              /* 设置宽度 */
    lv_obj_set_height(obj_left, scr_act_height() * 0.9);                            /* 设置高度 */
    lv_obj_align(obj_left, LV_ALIGN_LEFT_MID, 5, 0);                                /* 设置位置 */
    lv_obj_update_layout(obj_left);                                                 /* 手动更新物体的参数 */

    /* 创建右侧矩形背景 */
    lv_obj_t* obj_right = lv_obj_create(lv_scr_act());                              /* 创建一个基础对象 */
    lv_obj_set_width(obj_right, scr_act_width() - lv_obj_get_width(obj_left) - 15); /* 设置宽度 */
    lv_obj_set_height(obj_right, lv_obj_get_height(obj_left));                      /* 设置高度 */
    lv_obj_align_to(obj_right, obj_left, LV_ALIGN_OUT_RIGHT_MID, 5, 0);             /* 设置位置 */
    lv_obj_update_layout(obj_right);                                                /* 手动更新物体的参数 */

    /* 显示当前选项的文本内容 */
    list_label = lv_label_create(obj_right);                                        /* 创建标签 */
    lv_obj_set_width(list_label, lv_obj_get_width(obj_right) - 13);                 /* 设置标签的宽度 */
    lv_obj_align(list_label, LV_ALIGN_TOP_MID, 0, 5);                               /* 设置标签位置 */
    lv_obj_update_layout(list_label);                                               /* 手动更新标签的参数 */
    lv_obj_set_style_text_align(list_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);    /* 设置标签文本对齐方式 */
    lv_label_set_text(list_label, "u can do that");                                           /* 设置标签文本 */
    lv_obj_set_style_text_font(list_label, font, LV_PART_MAIN);                     /* 设置标签文本字体 */

    /* 创建列表 */
    list = lv_list_create(obj_left);                                                /* 创建列表 */
    lv_obj_set_width(list, lv_obj_get_width(obj_left) * 0.8);                       /* 设置列表宽度 */
    lv_obj_set_height(list, lv_obj_get_height(obj_left) * 0.9);                     /* 设置列表高度 */
    lv_obj_center(list);                                                            /* 设置列表的位置 */
    lv_obj_set_style_text_font(list, font, LV_PART_MAIN);                           /* 设置字体 */

    /* 为列表添加按钮 */
    lv_obj_t* btn;
    
    lv_list_add_text(list, "u can do that");                                                 /* 添加列表文本 */
    
    btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "MotorControl" );                             /* 添加按钮 */
    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
    lv_group_add_obj(g3, btn);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g3);//关联按键和组
//    btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Open");                       /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_SAVE, "Save");                            /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_CLOSE, "Delete");                         /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Edit");                            /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    
//    
//    lv_list_add_text(list, "Connectivity");                                         /* 添加列表文本 */
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_BLUETOOTH, "Bluetooth");                  /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_GPS, "Navigation");                       /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_USB, "USB");                              /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_BATTERY_FULL, "Battery");                 /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
//    lv_list_add_text(list, "Exit");                                                 /* 添加列表文本 */
//    
//    
//    btn = lv_list_add_btn(list, LV_SYMBOL_OK, "Apply");                             /* 添加按钮 */
//    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
//    
    btn = lv_list_add_btn(list, LV_SYMBOL_CLOSE, "back");                          /* 添加按钮 */
    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 添加按钮回调 */
    lv_group_add_obj(g3, btn);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g3);//关联按



}
void lv_motor_control(void)//系统界面
{
    lv_group_t *g2 = lv_group_create();//定义并创建组
    
    lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, scr_act_height()/6);        /* 创建选项卡 */
    lv_obj_set_style_text_font(tabview, font, LV_STATE_DEFAULT);                                /* 设置字体 */

    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "DC_Motor");                                    /* 添加选项卡1 */
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Step_Motor");                                   /* 添加选项卡2 */
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Bldc_Motor");                                    /* 添加选项卡3 */
    lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "Servo");                                       /* 添加选项卡4 */
    
    
//选项卡1页面*********************************直流有刷电机*********************************
//    lv_obj_t *tab1_label1 = lv_label_create(tab1);                                                   /* 创建标签（在选项卡1内） */
//    lv_label_set_text(tab1_label1, "Tonight's meeting cancelled.");                                  /* 设置文本内容 */
//    lv_obj_center(tab1_label1);                                                                      /* 设置位置 */

    /*******************创建直流有刷电机速度标签*******************/
    DC_Motor_label_speed = lv_label_create(tab1);                                    /* 创建速度显示标签 */
    lv_obj_set_style_text_font(DC_Motor_label_speed, font, LV_PART_MAIN);                    /* 设置字体 */
    lv_label_set_text(DC_Motor_label_speed, "DC_Motor_Speed : 0 RPM");                                /* 设置文本 */
    lv_obj_align(DC_Motor_label_speed, LV_ALIGN_CENTER, 0, -scr_act_height() / 4);           /* 设置标签位置 */
    
    /*******************创建直流有刷电机加速按钮*******************/
    DC_Motor_btn_speed_up = lv_btn_create(tab1);                                    
    lv_obj_set_size(DC_Motor_btn_speed_up, scr_act_width() / 4, scr_act_height() / 6);    
    lv_obj_align(DC_Motor_btn_speed_up, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);        
    lv_obj_add_event_cb(DC_Motor_btn_speed_up, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* label = lv_label_create(DC_Motor_btn_speed_up);                               
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                        
    lv_label_set_text(label, "Speed +");                                            
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                       

    lv_group_add_obj(g2, DC_Motor_btn_speed_up);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组

    /*******************创建直流有刷电机减速按钮*******************/
    DC_Motor_btn_speed_down = lv_btn_create(tab1);                                 
    lv_obj_set_size(DC_Motor_btn_speed_down, scr_act_width() / 4, scr_act_height() / 6);   
    lv_obj_align(DC_Motor_btn_speed_down, LV_ALIGN_CENTER, 0, 0);                            
    lv_obj_add_event_cb(DC_Motor_btn_speed_down, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);   

    lv_obj_t* label2 = lv_label_create(DC_Motor_btn_speed_down);                          
    lv_obj_set_style_text_font(label2, font, LV_PART_MAIN);                      
    lv_label_set_text(label2, "Speed -");                                            
    lv_obj_set_align(label2,LV_ALIGN_CENTER);                                        

    lv_group_add_obj(g2, DC_Motor_btn_speed_down);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
    
    /*******************创建直流有刷电机停止按钮*******************/
    DC_Motor_btn_stop = lv_btn_create(tab1);                                      
    lv_obj_set_size(DC_Motor_btn_stop, scr_act_width() / 4, scr_act_height() / 6);        
    lv_obj_align(DC_Motor_btn_stop, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                
    lv_obj_set_style_bg_color(DC_Motor_btn_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  
    lv_obj_set_style_bg_color(DC_Motor_btn_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);  
    lv_obj_add_event_cb(DC_Motor_btn_stop, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* label3 = lv_label_create(DC_Motor_btn_stop);                                   
    lv_obj_set_style_text_font(label3, font, LV_PART_MAIN);                          
    lv_label_set_text(label3, "Stop");                                              
    lv_obj_set_align(label3,LV_ALIGN_CENTER);                                       
    lv_group_add_obj(g2, DC_Motor_btn_stop);//添加部件到组--如果部件未加入默认组，则使用这个
    lv_indev_set_group(indev_keypad, g2);//关联按键和组


    /*******************创建直流有刷电机返回按钮*******************/
    DC_Motor_btn_back = lv_btn_create(tab1);                                         
    lv_obj_set_size(DC_Motor_btn_back, scr_act_width() / 4, scr_act_height() / 6);       

    // 设置按钮为圆形（椭圆形）
    lv_obj_set_style_radius(DC_Motor_btn_back, 999, LV_PART_MAIN);                           /* 设置圆角大小，999 表示接近圆形 */
 
    lv_obj_align(DC_Motor_btn_back, LV_ALIGN_CENTER, scr_act_width() / 3, scr_act_height() / 3);
    lv_obj_set_style_bg_color(DC_Motor_btn_back, lv_color_hex(0xa5c2d7), LV_STATE_DEFAULT);  /* 设置按钮背景颜色（默认） shift + C 复制RGB颜色*/
    lv_obj_set_style_bg_color(DC_Motor_btn_back, lv_color_hex(0x408080), LV_STATE_PRESSED);  /* 设置按钮背景颜色（按下） */
    lv_obj_add_event_cb(DC_Motor_btn_back, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* label4 = lv_label_create(DC_Motor_btn_back);                                   
    lv_obj_set_style_text_font(label4, font, LV_PART_MAIN);                         
    lv_label_set_text(label4, "back");                                               
    lv_obj_set_align(label4,LV_ALIGN_CENTER);                                       

    lv_group_add_obj(g2, DC_Motor_btn_back);
    lv_indev_set_group(indev_keypad, g2);
    
//选项卡1页面*********************************直流有刷电机*********************************
    
    
    
//选项卡2页面*********************************步进电机*********************************
//    lv_obj_t *tab2_label = lv_label_create(tab2);                                                   /* 创建标签（在选项卡2内） */
//    lv_label_set_text(tab2_label, "Step_Motor   Test");   /* 设置文本内容 */
//    lv_obj_center(tab2_label);                                                                      /* 设置位置 */

    /*******************创建步进电机速度标签*******************/
    Step_Motor_label_speed = lv_label_create(tab2);                                    /* 创建速度显示标签 */
    lv_obj_set_style_text_font(Step_Motor_label_speed, font, LV_PART_MAIN);                    /* 设置字体 */
    lv_label_set_text(Step_Motor_label_speed, "Step_Motor_Speed : 0 RPM");                                /* 设置文本 */
    lv_obj_align(Step_Motor_label_speed, LV_ALIGN_CENTER, 0, -scr_act_height() / 4);           /* 设置标签位置 */
    
    /*******************创建步进电机加速按钮*******************/
    Step_Motor_btn_speed_up = lv_btn_create(tab2);                                    
    lv_obj_set_size(Step_Motor_btn_speed_up, scr_act_width() / 4, scr_act_height() / 6);    
    lv_obj_align(Step_Motor_btn_speed_up, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);        
    lv_obj_add_event_cb(Step_Motor_btn_speed_up, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* label21 = lv_label_create(Step_Motor_btn_speed_up);                               
    lv_obj_set_style_text_font(label21, font, LV_PART_MAIN);                        
    lv_label_set_text(label21, "Speed +");                                            
    lv_obj_set_align(label21,LV_ALIGN_CENTER);                                       

    lv_group_add_obj(g2, Step_Motor_btn_speed_up);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
    
    /*******************创建步进电机减速按钮*******************/
    Step_Motor_btn_speed_down = lv_btn_create(tab2);                                 
    lv_obj_set_size(Step_Motor_btn_speed_down, scr_act_width() / 4, scr_act_height() / 6);   
    lv_obj_align(Step_Motor_btn_speed_down, LV_ALIGN_CENTER, 0, 0);                            
    lv_obj_add_event_cb(Step_Motor_btn_speed_down, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);   

    lv_obj_t* label22 = lv_label_create(Step_Motor_btn_speed_down);                          
    lv_obj_set_style_text_font(label22, font, LV_PART_MAIN);                      
    lv_label_set_text(label22, "Speed -");                                            
    lv_obj_set_align(label22,LV_ALIGN_CENTER);                                        

    lv_group_add_obj(g2, Step_Motor_btn_speed_down);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
    
    /*******************创建步进电机停止按钮*******************/
    Step_Motor_btn_stop = lv_btn_create(tab2);                                      
    lv_obj_set_size(Step_Motor_btn_stop, scr_act_width() / 4, scr_act_height() / 6);        
    lv_obj_align(Step_Motor_btn_stop, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                
    lv_obj_set_style_bg_color(Step_Motor_btn_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  
    lv_obj_set_style_bg_color(Step_Motor_btn_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);  
    lv_obj_add_event_cb(Step_Motor_btn_stop, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* label23 = lv_label_create(Step_Motor_btn_stop);                                   
    lv_obj_set_style_text_font(label23, font, LV_PART_MAIN);                          
    lv_label_set_text(label23, "Stop");                                              
    lv_obj_set_align(label23,LV_ALIGN_CENTER);                                       
    lv_group_add_obj(g2, Step_Motor_btn_stop);//添加部件到组--如果部件未加入默认组，则使用这个
    lv_indev_set_group(indev_keypad, g2);//关联按键和组


    /*******************创建步进电机返回按钮*******************/
    Step_Motor_btn_back = lv_btn_create(tab2);                                         
    lv_obj_set_size(Step_Motor_btn_back, scr_act_width() / 4, scr_act_height() / 6);       

    // 设置按钮为圆形（椭圆形）
    lv_obj_set_style_radius(Step_Motor_btn_back, 999, LV_PART_MAIN);                           /* 设置圆角大小，999 表示接近圆形 */
 
    lv_obj_align(Step_Motor_btn_back, LV_ALIGN_CENTER, scr_act_width() / 3, scr_act_height() / 3);
    lv_obj_set_style_bg_color(Step_Motor_btn_back, lv_color_hex(0xa5c2d7), LV_STATE_DEFAULT);  /* 设置按钮背景颜色（默认） shift + C 复制RGB颜色*/
    lv_obj_set_style_bg_color(Step_Motor_btn_back, lv_color_hex(0x408080), LV_STATE_PRESSED);  /* 设置按钮背景颜色（按下） */
    lv_obj_add_event_cb(Step_Motor_btn_back, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* labe24 = lv_label_create(Step_Motor_btn_back);                                   
    lv_obj_set_style_text_font(labe24, font, LV_PART_MAIN);                         
    lv_label_set_text(labe24, "back");                                               
    lv_obj_set_align(labe24,LV_ALIGN_CENTER);                                       

    lv_group_add_obj(g2, Step_Motor_btn_back);
    lv_indev_set_group(indev_keypad, g2);
//选项卡2页面*********************************步进电机*********************************
    








//选项卡3页面*********************************直流无刷电机*********************************
//    lv_obj_t *tab3_label = lv_label_create(tab3);                                                   /* 创建标签（在选项卡3内） */
//    lv_label_set_text(tab3_label, "Bldc_Motor    Test");                                                          /* 设置文本内容 */
//    lv_obj_center(tab3_label);                                                                      /* 设置位置 */
    /*******************创建直流无刷电机速度标签*******************/
    Bldc_Motor_label_speed = lv_label_create(tab3);                                    /* 创建速度显示标签 */
    lv_obj_set_style_text_font(Bldc_Motor_label_speed, font, LV_PART_MAIN);                    /* 设置字体 */
    lv_label_set_text(Bldc_Motor_label_speed, "Bldc_Motor_Speed : 0 RPM");                                /* 设置文本 */
    lv_obj_align(Bldc_Motor_label_speed, LV_ALIGN_CENTER, 0, -scr_act_height() / 4);           /* 设置标签位置 */
    /*******************创建直流无刷电机加速按钮*******************/
    Bldc_Motor_btn_speed_up = lv_btn_create(tab3);                                    
    lv_obj_set_size(Bldc_Motor_btn_speed_up, scr_act_width() / 4, scr_act_height() / 6);    
    lv_obj_align(Bldc_Motor_btn_speed_up, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);        
    lv_obj_add_event_cb(Bldc_Motor_btn_speed_up, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* label31 = lv_label_create(Bldc_Motor_btn_speed_up);                               
    lv_obj_set_style_text_font(label31, font, LV_PART_MAIN);                        
    lv_label_set_text(label31, "Speed +");                                            
    lv_obj_set_align(label31,LV_ALIGN_CENTER);                                       

    lv_group_add_obj(g2, Bldc_Motor_btn_speed_up);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
    /*******************创建直流无刷电机减速按钮*******************/
    Bldc_Motor_btn_speed_down = lv_btn_create(tab3);                                 
    lv_obj_set_size(Bldc_Motor_btn_speed_down, scr_act_width() / 4, scr_act_height() / 6);   
    lv_obj_align(Bldc_Motor_btn_speed_down, LV_ALIGN_CENTER, 0, 0);                            
    lv_obj_add_event_cb(Bldc_Motor_btn_speed_down, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);   

    lv_obj_t* label32 = lv_label_create(Bldc_Motor_btn_speed_down);                          
    lv_obj_set_style_text_font(label32, font, LV_PART_MAIN);                      
    lv_label_set_text(label32, "Speed -");                                            
    lv_obj_set_align(label32,LV_ALIGN_CENTER);    
    
    lv_group_add_obj(g2, Bldc_Motor_btn_speed_down);
    lv_indev_set_group(indev_keypad, g2);
    /*******************创建直流无刷电机停止按钮*******************/
    Bldc_Motor_btn_stop = lv_btn_create(tab3);                                      
    lv_obj_set_size(Bldc_Motor_btn_stop, scr_act_width() / 4, scr_act_height() / 6);        
    lv_obj_align(Bldc_Motor_btn_stop, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                
    lv_obj_set_style_bg_color(Bldc_Motor_btn_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  
    lv_obj_set_style_bg_color(Bldc_Motor_btn_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);  
    lv_obj_add_event_cb(Bldc_Motor_btn_stop, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* label33 = lv_label_create(Bldc_Motor_btn_stop);                                   
    lv_obj_set_style_text_font(label23, font, LV_PART_MAIN);                          
    lv_label_set_text(label33, "Stop");                                              
    lv_obj_set_align(label33,LV_ALIGN_CENTER);                                       
    lv_group_add_obj(g2, Bldc_Motor_btn_stop);//添加部件到组--如果部件未加入默认组，则使用这个
    lv_indev_set_group(indev_keypad, g2);//关联按键和组


    /*******************创建直流无刷电机返回按钮*******************/
    Bldc_Motor_btn_back = lv_btn_create(tab3);                                         
    lv_obj_set_size(Bldc_Motor_btn_back, scr_act_width() / 4, scr_act_height() / 6);       

    // 设置按钮为圆形（椭圆形）
    lv_obj_set_style_radius(Bldc_Motor_btn_back, 999, LV_PART_MAIN);                           /* 设置圆角大小，999 表示接近圆形 */
 
    lv_obj_align(Bldc_Motor_btn_back, LV_ALIGN_CENTER, scr_act_width() / 3, scr_act_height() / 3);
    lv_obj_set_style_bg_color(Bldc_Motor_btn_back, lv_color_hex(0xa5c2d7), LV_STATE_DEFAULT);  /* 设置按钮背景颜色（默认） shift + C 复制RGB颜色*/
    lv_obj_set_style_bg_color(Bldc_Motor_btn_back, lv_color_hex(0x408080), LV_STATE_PRESSED);  /* 设置按钮背景颜色（按下） */
    lv_obj_add_event_cb(Bldc_Motor_btn_back, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* labe34 = lv_label_create(Bldc_Motor_btn_back);                                   
    lv_obj_set_style_text_font(labe34, font, LV_PART_MAIN);                         
    lv_label_set_text(labe34, "back");                                               
    lv_obj_set_align(labe34,LV_ALIGN_CENTER);                                       


    lv_group_add_obj(g2, Bldc_Motor_btn_back);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
//选项卡3页面*********************************直流无刷电机*********************************



//选项卡4页面*********************************舵机*********************************
//    lv_obj_t *tab4_label = lv_label_create(tab4);                                                   /* 创建标签（在选项卡3内） */
//    lv_label_set_text(tab4_label, "Servo    Test");                                                          /* 设置文本内容 */
//    lv_obj_center(tab4_label);                                                                      /* 设置位置 */
    /*******************创建舵机速度标签*******************/
    Servo_label_speed = lv_label_create(tab4);                                    /* 创建速度显示标签 */
    lv_obj_set_style_text_font(Servo_label_speed, font, LV_PART_MAIN);                    /* 设置字体 */
    lv_label_set_text(Servo_label_speed, "Servor_Speed : 0 RPM");                                /* 设置文本 */
    lv_obj_align(Servo_label_speed, LV_ALIGN_CENTER, 0, -scr_act_height() / 4);           /* 设置标签位置 */
     /*******************创建舵机加速按钮*******************/
    Servo_btn_speed_up = lv_btn_create(tab4);                                    
    lv_obj_set_size(Servo_btn_speed_up, scr_act_width() / 4, scr_act_height() / 6);    
    lv_obj_align(Servo_btn_speed_up, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);        
    lv_obj_add_event_cb(Servo_btn_speed_up, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* label41 = lv_label_create(Servo_btn_speed_up);                               
    lv_obj_set_style_text_font(label41, font, LV_PART_MAIN);                        
    lv_label_set_text(label41, "Speed +");                                            
    lv_obj_set_align(label41,LV_ALIGN_CENTER);                                       

    lv_group_add_obj(g2, Servo_btn_speed_up);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
     /*******************创建舵机减速按钮*******************/
    Servo_btn_speed_down = lv_btn_create(tab4);                                 
    lv_obj_set_size(Servo_btn_speed_down, scr_act_width() / 4, scr_act_height() / 6);   
    lv_obj_align(Servo_btn_speed_down, LV_ALIGN_CENTER, 0, 0);                            
    lv_obj_add_event_cb(Servo_btn_speed_down, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);   

    lv_obj_t* label42 = lv_label_create(Servo_btn_speed_down);                          
    lv_obj_set_style_text_font(label42, font, LV_PART_MAIN);                      
    lv_label_set_text(label42, "Speed -");                                            
    lv_obj_set_align(label42,LV_ALIGN_CENTER);                                        

    lv_group_add_obj(g2, Servo_btn_speed_down);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
    /*******************创建舵机停止按钮*******************/
    Servo_btn_stop = lv_btn_create(tab4);                                      
    lv_obj_set_size(Servo_btn_stop, scr_act_width() / 4, scr_act_height() / 6);        
    lv_obj_align(Servo_btn_stop, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                
    lv_obj_set_style_bg_color(Servo_btn_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  
    lv_obj_set_style_bg_color(Servo_btn_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);  
    lv_obj_add_event_cb(Servo_btn_stop, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* label43 = lv_label_create(Servo_btn_stop);                                   
    lv_obj_set_style_text_font(label43, font, LV_PART_MAIN);                          
    lv_label_set_text(label43, "Stop");                                              
    lv_obj_set_align(label43,LV_ALIGN_CENTER);                                       
    lv_group_add_obj(g2, Servo_btn_stop);//添加部件到组--如果部件未加入默认组，则使用这个
    lv_indev_set_group(indev_keypad, g2);//关联按键和组


    /*******************创建舵机返回按钮*******************/
    Servo_btn_back = lv_btn_create(tab4);                                         
    lv_obj_set_size(Servo_btn_back, scr_act_width() / 4, scr_act_height() / 6);       
    // 设置按钮为圆形（椭圆形）
    lv_obj_set_style_radius(Servo_btn_back, 999, LV_PART_MAIN);                           /* 设置圆角大小，999 表示接近圆形 */
 
    lv_obj_align(Servo_btn_back, LV_ALIGN_CENTER, scr_act_width() / 3, scr_act_height() / 3);
    lv_obj_set_style_bg_color(Servo_btn_back, lv_color_hex(0xa5c2d7), LV_STATE_DEFAULT);  /* 设置按钮背景颜色（默认） shift + C 复制RGB颜色*/
    lv_obj_set_style_bg_color(Servo_btn_back, lv_color_hex(0x408080), LV_STATE_PRESSED);  /* 设置按钮背景颜色（按下） */
    lv_obj_add_event_cb(Servo_btn_back, sys_btn_event_cb, LV_EVENT_CLICKED, NULL);        

    lv_obj_t* labe44 = lv_label_create(Servo_btn_back);                                   
    lv_obj_set_style_text_font(labe44, font, LV_PART_MAIN);                         
    lv_label_set_text(labe44, "back");                                               
    lv_obj_set_align(labe44,LV_ALIGN_CENTER);                                       


    lv_group_add_obj(g2, Servo_btn_back);//添加部件到组--如果部件未加入默认组，则使用这个配置
    lv_indev_set_group(indev_keypad, g2);//关联按键和组
//选项卡4页面*********************************舵机*********************************

    /*************************   第二部分：选项卡（界面优化）   ************************/

    /* 1、按钮 */
    lv_obj_t *btn = lv_tabview_get_tab_btns(tabview);                                           /* 获取按钮部分 */

//    lv_group_add_obj(g2, btn);
//    lv_indev_set_group(indev_keypad, g2);
    /* 未选中的按钮 */
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xb7472a), LV_PART_ITEMS|LV_STATE_DEFAULT);     /* 设置按钮背景颜色：橙色 */
    lv_obj_set_style_bg_opa(btn, 200, LV_PART_ITEMS|LV_STATE_DEFAULT);                          /* 设置按钮背景透明度 */
    lv_obj_set_style_text_color(btn, lv_color_hex(0xf3f3f3), LV_PART_ITEMS|LV_STATE_DEFAULT);   /* 设置按钮文本颜色：白色 */

    /* 选中的按钮 */
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xe1e1e1), LV_PART_ITEMS|LV_STATE_CHECKED);     /* 设置按钮背景颜色：灰色 */
    lv_obj_set_style_bg_opa(btn, 200, LV_PART_ITEMS|LV_STATE_CHECKED);                          /* 设置按钮背景透明度 */
    lv_obj_set_style_text_color(btn, lv_color_hex(0xb7472a), LV_PART_ITEMS|LV_STATE_CHECKED);   /* 设置按钮文本颜色：橙色 */
    lv_obj_set_style_border_width(btn, 0, LV_PART_ITEMS| LV_STATE_CHECKED);                     /* 设置按钮边框宽度为0 */

    /* 2、主体 */
    lv_obj_t *obj = lv_tabview_get_content(tabview);                                            /* 获取主体部分 */
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_STATE_DEFAULT);                   /* 设置背景颜色：白色 */
    lv_obj_set_style_bg_opa(obj, 255, LV_STATE_DEFAULT);                                        /* 设置背景透明度 */
    
    
}

void Refresh_And_CreateScren(void)//刷新页面函数，调用他，再调用一个界面函数即可刷屏
{

        lv_obj_clean(lv_scr_act());             //销毁当前屏幕上的所有对象

        lv_scr_load(lv_scr_act());              // 卸载当前屏幕
 
        lv_obj_t *new_screen = lv_scr_act();   // 创建一个新的屏幕

        lv_scr_load(new_screen);                //加载新的屏幕

        lv_obj_clean(lv_scr_act());             //销毁当前屏幕上的所有对象

        if (scr_act_width() <= 320)              /* 根据活动屏幕宽度选择字体，由于我们只是由主界面开始，之后都是通过此函数来进行切屏，因此刷新函数这里默认带了选择字体，之后的界面都不需要调用选择字体，只有主界面一开始需要调用一次*/
        {
            font = &lv_font_montserrat_10;
        }
        else if (scr_act_width() <= 480)
        {
            font = &lv_font_montserrat_14;
        }
        else
        {
            font = &lv_font_montserrat_20;
        }
        
        //                                     // 清除屏幕后重新绘制或加载新的内容
}

