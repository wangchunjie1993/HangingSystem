#include<iom128v.h>
#include"def.h"

#ifndef __USART_H__
#define __USART_H__

void usart0_init(uint baud);
void usart0_transmit(uchar data);

#pragma interrupt_handler usart0_receive_isr:19
void usart0_receive_isr();

#endif