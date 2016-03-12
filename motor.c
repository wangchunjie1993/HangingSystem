#include<iom128v.h>
#include"def.h"

uint ctc_cmp=0;
uint count1,count2;
int com_1=0,com_2=0;
void motor_init(void)
{ 
 TCCR2=0x00;
 TCNT2=0x64;
 OCR2=0x9c; 
 TCCR2=0x1b;
}

void motor_start()
{}

void motor_1_init()
{
 TCCR0 = 0x00; //stop
 ASSR  = 0x00; //set async mode
 TCNT0 = 0x64; //set count
 OCR0  = 0x9C;
 TCCR0 = 0x1C; //start timer
 DDRB|=(1<<4);
}

void motor_2_init()
{
 TCCR2 = 0x00; //stop
 TCNT2 = 0x64; //setup
 OCR2  = 0x9C;
 TCCR2 = 0x1B; //start
 DDRB|=(1<<7);
}

void motor_1_start(void)
{
 DDRB|=(1<<4);
}

void motor_2_start(void)
{
 DDRB|=(1<<7);
 //PORTB|=(1<<7);
}

void motor_1_stop(void)
{
 DDRB&=~(1<<4);
 TIMSK&=~(1<<1);
}

void motor_2_stop(void)
{
 DDRB&=~(1<<7);
 TIMSK&=~(1<<7);
}

void motor_stop(void)
{
 DDRB&=~(1<<7);	 
}

void forward(uint count)
{
 if(count)
 {
  ctc_cmp=count;
  TIMSK|=(1<<7);
  motor_start();
  DDRB |= 0x03;
  PORTB=(PORTB|0x02)&(~0x01);
  while(ctc_cmp);
  TIMSK&=~(1<<7);
  motor_stop();
 }
 else
 {
  motor_start();
  DDRB |= 0x03;
  PORTB=(PORTB|0x02)&(~0x01);
 }
 
}

void backward_1(uint count)
{
  com_1=count;
  TIMSK|=(1<<1);
  motor_1_start();
  
  DDRA|=0x01;		 //电机转向
  PORTA&=~(0x01);
  
}

void backward_2(uint count)
{
  com_2=count;
  TIMSK|=(1<<7);
  motor_2_start();
  
  DDRA|=0x02;	
  PORTA|=0x02;
  
}

void forward_1(uint count)
{
  com_1=count;
  TIMSK|=(1<<1);
  motor_1_start();
  
  DDRA|=0x01;
  PORTA|=0x01;
  
}

void forward_2(uint count)
{
  com_2=count;
  TIMSK|=(1<<7);
  motor_2_start();
  
  DDRA|=0x02;
  PORTA&=~(0x02);
  
}

void left(uint count)
{
 if(count)
 {
  TIMSK|=(1<<7);
  ctc_cmp=count;
  motor_start();
  DDRB|=0x03;
  PORTB|=0x03;  
  while(ctc_cmp);
  TIMSK&=~(1<<7);
  motor_stop();
 }
 else
 {
  motor_start();
  DDRB|=0x03;
  PORTB|=0x03;
 }
 
}

void right(uint count)
{
 if(count)
 {
  TIMSK|=(1<<7);
  ctc_cmp=count;
  motor_start();
  DDRB|=0x03;
  PORTB&=~0x03;
  while(ctc_cmp);
  TIMSK&=~(1<<7);
  motor_stop();
 }
 else
 {
  motor_start();
  DDRB|=0x03;
  PORTB&=~0x03;
 }
 
}

void backward(uint count)
{
 if(count)
 {
  TIMSK|=(1<<7);
  ctc_cmp=count;
  motor_start();
  DDRB|=0x03;
  PORTB=~((PORTB|0x02)&(~0x01));
  while(ctc_cmp);
  TIMSK&=~(1<<7);
  motor_stop();
 }
 else
 {
  motor_start();
  DDRB|=0x03;
  PORTB=~((PORTB|0x02)&(~0x01));
 } 
}

