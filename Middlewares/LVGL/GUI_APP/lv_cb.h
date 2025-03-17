#ifndef __LV_CB_H
#define __LV_CB_H
#include "sys.h"
#include "lvgl.h"
void enter_event_cb(lv_event_t * e);
void keyboard_event_cb(lv_event_t *e);
void sys_btn_event_cb(lv_event_t * e);
void timer_cb(lv_timer_t *timer);
void list_btn_event_cb(lv_event_t *e);
#endif
