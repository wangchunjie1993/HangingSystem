#include<iom128v.h>
#include"def.h"

#ifndef __1602LCD_H__
#define __1602LCD_H__

uchar busytest(void);
void write_command(uchar com);
void write_address(uchar add);
void write_data(uchar data);
void lcd_init(void);
void display_time(uint count);
void display_distance(uint dist);


#endif
