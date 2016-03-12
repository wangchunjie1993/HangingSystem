//ICC-AVR application builder : 2013/7/4 18:16:05
// Target : M128
// Crystal: 8.0000Mhz

#include"12864_chuan.h"
void delay_us(uchar us)
{
 uint i;
 for(i=us;i>0;i--)
   _NOP();
}

void delay_ms(uint ms)
{
 uint i,j;
 for(i=ms;i>0;i--)
     for(j=1141;j>0;j--);
} 
//void port_init(void)
//{
 //PORTA = 0x00;
 //DDRA  = 0xff;
 /*PORTB = 0x00;
 DDRB  = 0x00;
 PORTC = 0x00; //m103 output only
 DDRC  = 0x00;
 PORTD = 0x00;
 DDRD  = 0x00;
 PORTE = 0x00;
 DDRE  = 0x00;
 PORTF = 0x00;
 DDRF  = 0x00;
 PORTG = 0x00;
 DDRG  = 0x00;*/
//}

 //功能 : 按照液晶的串口通信协议，发送数据
void write_bit(uchar bits)
{
 uchar i;
 for(i=0;i<8;i++)
 {
  if((bits<<i) & 0x80)
  {
   SID_UP;
  }
  else
  {
   SID_DOWN;
  }
  SCLK_DOWN;
  SCLK_UP;
 }
}


//功能 : 写串口指令
void write_command(uchar command)
{
 CS;
 write_bit(0xf8);//串口格式，写控制指令
 write_bit(command & 0xf0);//写高4位
 write_bit((command<<4) & 0xf0);//写低4位
 delay_ms(2);
}

//功能 : 写串口数据
void write_data(uchar data)
{
 CS;
 write_bit(0xfa);//写数据
 write_bit(data & 0xf0);//写高4位
 write_bit((data<<4) & 0xf0);//写低4位
 delay_ms(2);
}

 //设定显示位置
void write_address(uchar x,uchar y)
{
 uchar add;
 switch(x)
 {
  case 0:add=0x80+y;break;
  case 1:add=0x90+y;break;
  case 2:add=0x88+y;break;
  case 3:add=0x98+y;break;
 }
 write_command(add);
 delay_us(90);
}

void write_word(uchar *pos)
{
  while(*pos)
  {
   write_data(*pos);
   pos++;
   delay_us(90);
  }
}
void init_12864(void)
{
 RST_DOWN;
 //delay(10);
 RST_UP;
 //delay(50);
 write_command(0x30);//功能设置:设置为基本指令集
 delay_us(90);
 write_command(0x0c);//整体显示，不显示光标和位置
 delay_us(90);
 write_command(0x01);//清屏
 delay_ms(8);
 PSB;
}

//call this routine to initialize all peripherals
void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 XDIV  = 0x00; //xtal divider
 XMCRA = 0x00; //external memory
 port_init();
 init_12864();
 MCUCR = 0x00;
 EICRA = 0x00; //extended ext ints
 EICRB = 0x00; //extended ext ints
 EIMSK = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 ETIMSK = 0x00; //extended timer interrupt sources
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}
/*void main()
{
 init_devices();
 while(1)
 {
  write_address(0,0);
  delay_us(90);
  write_word("王春杰");
  delay_us(90);
  write_address(1,0);
  delay_us(90);
  write_word("王杰");
  delay_us(90);
  write_address(2,0);
  delay_us(90);
  write_word("王凯");
  delay_us(90);
  write_address(3,0);
  delay_us(90);
  write_word("第二小组A");
  delay_us(90);
 }
}*/
