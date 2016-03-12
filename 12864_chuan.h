#ifndef _12864_CHUAN_H_
#define _12864_CHUAN_H_
#include <iom128v.h>
#include <macros.h>
#define CS PORTA |= BIT(2)//片选信号
#define SID_UP PORTA|= BIT(3)//高电平数据信号
#define SID_DOWN PORTA&= ~BIT(3)//低电平数据信号
#define SCLK_UP PORTA|=BIT(4)//同步时钟高电平信号
#define SCLK_DOWN PORTA &= ~BIT(4)//同步时钟低电平信号
#define RST_UP PORTA|=BIT(5)//复位信号无效
#define RST_DOWN PORTA &= ~BIT(5)//复位信号有效
#define PSB PORTA&=~BIT(6)//并行、串行选择端
#define uchar unsigned char
#define uint unsigned int

void write_command(uchar command);
void write_data(uchar data);
void write_address(uchar x,uchar y);
void write_word(uchar *pos);
void init_12864(void);

#endif