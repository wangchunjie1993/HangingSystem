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

 //���� : ����Һ���Ĵ���ͨ��Э�飬��������
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


//���� : д����ָ��
void write_command(uchar command)
{
 CS;
 write_bit(0xf8);//���ڸ�ʽ��д����ָ��
 write_bit(command & 0xf0);//д��4λ
 write_bit((command<<4) & 0xf0);//д��4λ
 delay_ms(2);
}

//���� : д��������
void write_data(uchar data)
{
 CS;
 write_bit(0xfa);//д����
 write_bit(data & 0xf0);//д��4λ
 write_bit((data<<4) & 0xf0);//д��4λ
 delay_ms(2);
}

 //�趨��ʾλ��
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
 write_command(0x30);//��������:����Ϊ����ָ�
 delay_us(90);
 write_command(0x0c);//������ʾ������ʾ����λ��
 delay_us(90);
 write_command(0x01);//����
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
  write_word("������");
  delay_us(90);
  write_address(1,0);
  delay_us(90);
  write_word("����");
  delay_us(90);
  write_address(2,0);
  delay_us(90);
  write_word("����");
  delay_us(90);
  write_address(3,0);
  delay_us(90);
  write_word("�ڶ�С��A");
  delay_us(90);
 }
}*/
