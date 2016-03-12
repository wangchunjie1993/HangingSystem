#include"def.h"

#ifndef __MOTOR_H__
#define __MOTOR_H__

void motor_init(void);
void motor_start(void);
void motor_stop(void);
void forward(uint count);
void left(uint count);
void right(uint count);
void backward(uint count);

void forward_1(uint count);
void forward_2(uint count);
void backward_1(uint count);
void backward_2(uint count);

#endif