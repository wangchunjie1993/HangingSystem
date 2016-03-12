#ifndef _LCD_1602_H_
#define _LCD_1602_H_
void write_command(uchar command);
void write_data(uchar data);
void write_address(uchar data);
void init_lcd(void);
void write_time_init( uchar *pos);
void write_distance_init(uchar *pos);
#endif