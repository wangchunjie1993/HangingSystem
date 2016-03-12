#ifndef _12864_CHUAN_H_
#define _12864_CHUAN_H_
#include <iom128v.h>
#include <macros.h>
#define CS PORTA |= BIT(2)//Ƭѡ�ź�
#define SID_UP PORTA|= BIT(3)//�ߵ�ƽ�����ź�
#define SID_DOWN PORTA&= ~BIT(3)//�͵�ƽ�����ź�
#define SCLK_UP PORTA|=BIT(4)//ͬ��ʱ�Ӹߵ�ƽ�ź�
#define SCLK_DOWN PORTA &= ~BIT(4)//ͬ��ʱ�ӵ͵�ƽ�ź�
#define RST_UP PORTA|=BIT(5)//��λ�ź���Ч
#define RST_DOWN PORTA &= ~BIT(5)//��λ�ź���Ч
#define PSB PORTA&=~BIT(6)//���С�����ѡ���
#define uchar unsigned char
#define uint unsigned int

void write_command(uchar command);
void write_data(uchar data);
void write_address(uchar x,uchar y);
void write_word(uchar *pos);
void init_12864(void);

#endif