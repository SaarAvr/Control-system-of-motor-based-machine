#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern int wait_command;
extern int slow_flag;
extern int l;
extern int step_counter_send[2];
extern float delta_phi;

extern void clockwise_turn(void);
extern void motor_control();
extern void calibrate();
extern void motor_clockwise();
extern void motor_counterwise();
extern void script_mode();
extern void painter();

extern float pointer_degree;

#endif







