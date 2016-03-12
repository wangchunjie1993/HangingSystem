//ICC-AVR application builder : 2013/5/1 23:36:04
// Target : M128
// Crystal: 16.000Mhz

#include <iom128v.h>
#include <macros.h>
#include"def.h"
//#include<intrins.h>
//const uchar code_1[]={0,1,2,3,4,5,6,7,8,9};
//const uchar code_2[]="distance:";
uchar pos=0;
uchar count=0;
void delay_us(uint us)
{
  uint x;
  for(x=us;x>0;x--)
	  {
	    _NOP();
	  }
  
}
void delay_ms(uint ms)
{
 uint x,y;
  for(x=ms;x>0;x--)
      for(y=1141;y>0;y--);
}

void write_command(uchar command)
{
  //uchar temp;
  //temp=command;
  //EN_0;//禁止
  //delay_us(20);
  RS_C;
  EN_0;
  //delay_us(2);
  
  PORTF&=0x0f;//清高四位
  PORTF=PORTF|(command&0xf0);//写高四位
  EN_1;
  EN_0;
  //delay_us(2);
  
  command=command<<4;
  PORTF&=0x0f;//清高四位
  PORTF=PORTF|(command&0xf0);//写低四位
  EN_1;
  EN_0;
  delay_ms(3);
}


void write_data(uchar data)
{
  //uchar temp;
  //temp=data;
  //EN_0;//禁止
  //delay_us(20);
  RS_D;
  //delay_us(20);
  EN_0;
  delay_us(2);
  EN_1;
  
  PORTF&=0x0f;//清高四位
  PORTF=PORTF|(data&0xf0);//写高四位
  //EN_1;
  //delay_us(2);
  EN_0;
  //delay_us(2);
  
  data=(data<<4);
  PORTF&=0x0f;//清高四位
  PORTF=PORTF|(data&0xf0);//写低四位
  EN_1;
  EN_0;
  delay_ms(3);
}

void write_address(uchar add)
{
 write_command(add|0x80);
 delay_ms(3);
}

void init_lcd(void)
{
  DDRF  = 0xFF;
  PORTF = 0x00;
  EN_0;
  delay_ms(15);//上电稳定
  write_command(0x28);//设定4位数据接口、两行显示、5X7点阵字符
  delay_us(400);
  write_command(0x28);//设定4位数据接口、两行显示、5X7点阵字符
  delay_us(400);
  write_command(0x28);
  delay_us(400);
  write_command(0x0c);//开显示，关光标，不闪烁
  delay_us(400);
  write_command(0x06);//显示模式，屏幕不动，字符后移
  delay_us(400);
  write_command(0x01);//清屏
  delay_us(200); 
  delay_ms(1);
  //write_time_init("TimA:");
   delay_ms(1);
  //write_distance_init("Dis:");
  delay_ms(50);
}

void write_time_init( uchar *pos)
{
  write_command(0x80);
  delay_ms(20);
  while(*pos)
  {
    write_data(*pos);
	delay_ms(1);
	pos++;
  }
}
void write_distance_init(uchar *pos)
{
   write_command(0xc0);
   delay_ms(50);
    while(*pos)
  {
    write_data(*pos);
	delay_ms(2);
	pos++;
  }
}
