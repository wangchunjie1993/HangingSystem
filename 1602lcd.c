#include<iom128v.h>
#include<macros.h>
#include"def.h"
#include"lib.h"

uchar const LCD[]={'D','i','s','t',':','T','i','m','e',':'};

uchar busytest(void)
{
 uchar temp;
 DDRC=0x00;
 RS_0;
 RW_1;
 E_1;
 delay(5);
 temp=PINC&0x80;
 E_0;
 DDRC=0xff;
 return temp;
}

void write_command(uchar com)
{
 while(busytest());
 RS_0;
 RW_0;
 E_0;
 PORTC=com;
 delay(5);
 E_1;
 delay(5);
 E_0;
}

void write_address(uchar add)
{
 write_command(add|0x80);
}

void write_data(uchar data)
{
 while(busytest());
 RS_1;
 RW_0;
 E_0;
 PORTC=data;
 delay(5);
 E_1;
 delay(5);
 E_0;
}

void lcd_init(void)
{
 uchar i;
 DDRA|=BIT(5)|BIT(6)|BIT(7);
 DDRC=0xff;
 delayms(15);
 write_command(0x38);
 delayms(5);
 write_command(0x38);
 delayms(5);
 write_command(0x38);
 delayms(5);
 write_command(0x0c);  
 delayms(5);
 write_command(0x06);  
 delayms(5);											  
 write_command(0x01);  
 delayms(5);
 
 write_command(0x04);
 delay(5);
}

void display_time(uint count)
{
 uchar num=0,i;
 uint temp=count;
 while(temp!=0)
 {
  temp/=10;
  num++;
 }
 temp=count;
 write_address(0x0f);
 for(i=0;i<num;i++)/////////////////////////////////////////
 {
  write_data(temp%10+48);
  delay(1);
  temp/=10;
 }
}

void display_distance(uint dist)
{
 uchar num=0,i;
 uint temp=dist;
 while(temp!=0)
 {
  temp/=10;
  num++;
 }
 temp=dist;
 write_address(0x4f);
 delay(1);
 write_data(temp%10+48);
 delay(1);
 temp/=10;
 write_data('.');
 delay(1);
 for(i=0;i<num-1;i++)///////////////////////////////////
 {
  write_data(temp%10+48);
  delay(1);
  temp/=10;
 }
}