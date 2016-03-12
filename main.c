#include<iom128v.h>
#include<macros.h>
#include<MATH.H>
#include"def.h"
#include"lib.h"
#include"motor.h"
//#include"1602lcd.h"
//#include"LCD_1602.h"
#include"12864_chuan.h"

void port_init(void);
void calc(float x1,float y1,float x2,float y2); 			//计算绳长变换量  (x1,y1)到(x2,y2)
void set_point(void); 			//设置坐标
void go(void);			  //运行
void draw_circle();		  //画圆
void trace();			  //寻线
void show_point();		  //实时显示
void draw_spiral();		  //画螺旋线

uchar mod_count=0; 	//模式计数
float x=0,y=0;	  //坐标X,Y
float x_pre=0,y_pre=0;	   //前一次坐标X,Y
uchar shiwei=0,gewei=0;	   
float dL1=0,dL2=0;	   //绳长变化量
float dx=0,dy=0;	//坐标变化量
float x_end=0,y_end=0;			//终点x,y坐标
uint i=0,j=0;
float circ_x=0,circ_y=0;		//圆心X,Y坐标
float angle=0;		//圆的参数
float t=0;		//螺旋线参数
uchar up_down=0;	//上下运动标志			
uchar miss_count=0;		//丢失黑线计数
uchar find_count=0;		//查找黑线次数计数


//extern uint ctc_cmp;
//extern uint count1,count2;

extern int com_1,com_2;

void main(void)
{
 SREG=0x80;
 motor_1_init();
 motor_2_init();
 //init_lcd();
 port_init();
 init_12864();
 
 while(1)
 {
  mode_select();
  
  switch(mod_count)
  {
   case 1:		   //自行运动
   {
   	calc(0,START_POINT,50,50);
	go();
	while(OK!=0);
	calc(50,50,0,START_POINT);
  	go();
   	break;
   }
   case 2:		   //设定坐标运动
   {
   	write_address(1,0);
	write_word("设置终点坐标：");
   	set_point();
	y=y+OFFSET;
	calc(0,START_POINT,x,y);
	go();
	while(OK!=0);
	calc(x,y,0,START_POINT);
    go();
   	break;
   }
   case 3:		   //画圆
   {
   	write_address(1,0);
	write_word("设置圆心坐标:");
	draw_circle();
	while(OK!=0);
	calc(x,y,0,START_POINT);
    go();
   	break;
   }
   case 4:		   //寻线运动
   {
   	write_address(1,0);
	write_word("设置起点坐标:");
   	trace();
	while(OK!=0);
	calc(x,y,0,START_POINT);
    go();
   	break;
   }
   case 5:		   //画螺旋线
   {
	write_address(1,0);
	write_word("设置中心坐标:");
	draw_spiral();
	while(OK!=0);
	calc(x,y,0,START_POINT);
    go();
   	break;
   }
   case 6:			//实时显示画笔坐标
   {
    write_address(1,0);
	write_word("设置终点坐标：");
	show_point();
	
	while(OK!=0);
	calc(x,y,0,START_POINT);
  	go();
	break;
   }
   default:
   break;
  }
  
 }
}

/*void main()
{

 SREG=0x80;
 motor_1_init();
 motor_2_init();
 port_init();
 
 //backward_1(6000);
 //backward_2(8000);
 
 //x=50;
 //y=50;
 //draw_circle();

 while((com_1>=0)||(com_2>=0))
 {
  if(com_1<0)
  {
   motor_1_stop();
  }
  if(com_2<0)
  {
   motor_2_stop();
  }
 }
 
 calc(0,START_POINT,50,80);
 go();
 x_pre=50;
 y_pre=80;
 for(i=0;i<30;i++)
 {
  x=x_pre;
  y=y_pre+1;
  calc(x_pre,y_pre,x,y);
  go();
  x_pre=x;
  y_pre=y;
 }
 while(1);
 
}*/

#pragma interrupt_handler timer0_comp_isr:16
void timer0_comp_isr(void)
{
 com_1--;//compare occured TCNT0=OCR0
}

#pragma interrupt_handler timer2_cmp_isr:10
void timer2_cmp_isr(void)
{
 com_2--; 
}

void port_init(void)	 
{
 DDRD&=0xf0;			 //按键端口
 PORTD|=0x0f;
 
 DDRB&=0x00;
 
 		
 	 
 PORTA&= 0x03;			 //12864
 DDRA|= 0xfc;
 
 DDRA|=0x80;
 PORTA|=0x80;
 
 DDRF&=0xf0;		   //光电开关端口
 PORTF&=0xf0;
 
}

void calc(float x1,float y1,float x2,float y2) 			//计算绳长变换量  (x1,y1)到(x2,y2)
{
 float l1,l2;
 float dL1_1,dL1_2,dL2_1,dL2_2;
 
 
 
 l1=sqrtf((x1+15)*(x1+15)+(b-y1)*(b-y1));
 l2=sqrtf((a-x1)*(a-x1)+(b-y1)*(b-y1));
 
 dL1_1=l1-L1;
 dL2_1=l2-L2;

 l1=sqrtf((x2+15)*(x2+15)+(b-y2)*(b-y2));
 l2=sqrtf((a-x2)*(a-x2)+(b-y2)*(b-y2)); 
 
 dL1_2=l1-L1;
 dL2_2=l2-L2;
 
 dL1=dL1_2-dL1_1;
 dL2=dL2_2-dL2_1;
 
}

void mode_select(void)				//模式选择
{
  write_command(0x01);
  write_address(0,0);
  write_word("请选择工作模式:");
  delay_ms(1);
  mod_count=0;
  while(OK!=0)  			
  {
   if(MOD==0)
   {
   	mod_count++;
	if(mod_count>6)
	{
	 mod_count=1;
	}
   }
   delayms(200);
   /*write_address(0x00);
   write_data('M');
   write_data('O');
   write_data('D');
   write_data(mod_count+48);   */
   switch(mod_count)
   {
   	case 1:
	{
	 write_command(0x01);
	 write_address(0,0);
	 write_word("自行设定轨迹");
	 break;
	}
	case 2:
	{
	 write_command(0x01);
	 write_address(0,0);
	 write_word("设定坐标运动");
	 break;
	}
	case 3:
	{
	 write_command(0x01);
	 write_address(0,0);
	 write_word("画圆");
	 break;
	}
	case 4:
	{
	 write_command(0x01);
	 write_address(0,0);
	 write_word("寻线运动");
	 break;
	}
	case 5:
	{
	 write_command(0x01);
	 write_address(0,0);
	 write_word("画螺旋线");
	 break;
	}
	case 6:
	{
	 write_command(0x01);
	 write_address(0,0);
	 write_word("实时显示画笔坐标");
	 break;
	}
	default:
	{
	 break;
	}
   }
  }
  delayms(100);
  while(OK==0);
}

void set_point(void) 			//设置坐标
{
    x=y=0;
	shiwei=gewei=0;
	
   	/*write_address(0x40);				//设置X坐标
	write_data('X');
	write_data('=');
	write_data('0');
	write_data('0');*/
	
	write_address(2,0);
	write_word("X=");
	
	while(OK!=0)		//十位
	{
	 if(SHU==0)
	 shiwei++;
	 delayms(200);
	 if(shiwei>8)
	 shiwei=0;
	 /*write_address(0x42);
	 write_data(shiwei+48);*/
	 
	 write_address(2,1);
	 write_data(shiwei+48);
	 write_address(2,2);
	 write_data('0');
	}
	delayms(500);
	
	while(OK!=0)	  //个位
	{
	 if(SHU==0)
	 gewei++;
	 delayms(200);
	 if(gewei>9)
	 gewei=0;
	 /*write_address(0x43);
	 write_data(gewei+48);*/
	 
	 write_address(2,2);
	 write_data(gewei+48);
	}
	delayms(500);
	x=shiwei*10+gewei;	  
	
	/*write_address(0x46);   				//设置Y坐标
	write_data('Y');
	write_data('=');
	write_data('0');
	write_data('0');
	write_data('0');*/
	
	write_address(3,0);
	write_word("Y=");
	
	shiwei=gewei=0;
	
	while(OK!=0)		//十位
	{
	 if(SHU==0)
	 shiwei++;
	 delayms(200);
	 if(shiwei<10)
	 {
	  /*write_address(0x48);
	  write_data('0');
	  write_data(shiwei+48);
	  write_data('0');*/
	  write_address(3,1);
	  write_data('0');
	  write_address(3,2);
	  write_data(shiwei+48);
	  write_address(3,3);
	  write_data('0');
	  
	 }
	 else if(shiwei==10)
	 {
	  /*write_address(0x48);
	  write_data('1');
	  write_data('0');
	  write_data('0');*/
	  write_address(3,1);
	  write_data('1');
	  write_address(3,2);
	  write_data('0');
	  write_address(3,3);
	  write_data('0');
	 }
	 else 
	 {
	  shiwei=0;
	  /*write_address(0x48);
	  write_data('0');
	  write_data('0');
	  write_data('0');*/
	  write_address(3,1);
	  write_data('0');
	  write_address(3,2);
	  write_data('0');
	  write_address(3,3);
	  write_data('0');
	 }
	}
	delayms(500);
	
	while(OK!=0)	  	  //个位
	{
	 if(SHU==0)
	 gewei++;
	 delayms(200);
	 if(gewei>9)
	 gewei=0;
	 /*write_address(0x4a);
	 write_data(gewei+48);*/
	 write_address(3,3);
	 write_data(gewei+48);
	}
	
	y=shiwei*10+gewei;
	delayms(500);
}

void go(void)			  //运行
{
 if(dL1>0)
 forward_1(PER_STEP_1*dL1);
 else 
 backward_1((-PER_STEP_1)*dL1);
 
 if(dL2>0)
 forward_2(PER_STEP_2*dL2);
 else
 backward_2((-PER_STEP_2)*dL2);
 while((com_1>=0)||(com_2>=0))
 {
  if(com_1<0)
  {
   motor_1_stop();
  }
  if(com_2<0)
  {
   motor_2_stop();
  }
 }
 motor_1_stop();
 motor_2_stop();
}

void draw_circle()
{
 	angle=-_PI/2;
   	set_point();
	circ_x=x;
	circ_y=y+OFFSET;
	calc(0,START_POINT,circ_x,circ_y-R);
	go();
	x_pre=circ_x;
	y_pre=circ_y-R;
	for(i=0;i<120;i++)
	{
	 angle+=_PI/60;
	 x=circ_x+R*cosf(angle);
	 y=circ_y+R*sinf(angle);
	 calc(x_pre,y_pre,x,y);
	 go();
	 x_pre=x;
	 y_pre=y;
	}
}

void trace()
{
 set_point();
 y=y+OFFSET;
 calc(0,START_POINT,x,y);
 go();

 up_down=0;
 while(1)
 {
  if((NO_1)&&(!NO_2)&&(!NO_3)&&(!NO_4))   			//黑线在左
  //if(NO_1!=0)
  {
   miss_count=0;
   find_count=0;
   calc(x,y,x-1,y);
   x=x-1;
   go();
  }
  else if((!NO_1)&&(!NO_2)&&(!NO_3)&&(NO_4))				//黑线在右
  //else if(NO_4!=0)
  {
   miss_count=0;
   find_count=0;
   calc(x,y,x+1,y);
   x=x+1;
   go();
  }
  //else if(((NO_1==0)&&(NO_2!=0)&&(NO_3==0)&&(NO_4==0))||((NO_1==0)&&(NO_2==0)&&(NO_3!=0)&&(NO_4==0)))		//黑线在中
  else if((!NO_1)&&(NO_2)&&(!NO_3)&&(!NO_4))						//黑线在中左
  {
   miss_count=0;
   find_count=0;
   if(up_down==0)
   {
   	calc(x,y,x-1,y+1);
	x=x-1;
	y=y+1;
   	
   }
   else
   {
   	calc(x,y,x-1,y-1);
	x=x-1;
	y=y-1;
	
   }
   go();
  }
  else if((!NO_1)&&(!NO_2)&&(NO_3)&&(!NO_4))						//黑线在中右
  {
   miss_count=0;
   find_count=0;
   if(up_down==0)
   {
   	calc(x,y,x+1,y+1);
	x=x+1;
	y=y+1;
   	
   }
   else
   {
   	calc(x,y,x+1,y-1);
	x=x+1;
	y=y-1;
	
   }
   go();
  }
  else if((!NO_1)&&(!NO_2)&&(!NO_3)&&(!NO_4)) 				//丢失黑线
  {
   miss_count++;
   
   if(miss_count>2)
   {
   	up_down=(up_down+1)%2;
	miss_count=0;
	
	find_count++;
	if(find_count>4)
	{
	 break;
	}
	
	if(up_down==0)	 	   //回到上一个位置
	{
	 calc(x,y,x,y+2);
	 y=y+2;
	}
	else
	{
	 calc(x,y,x,y-2);
	 y=y-2;
	}
	go();
   }
   
   if(up_down==0)
   {
   	calc(x,y,x,y+1);
	y=y+1;
   }
   else
   {
   	calc(x,y,x,y-1);
	y=y-1;
   }
   go();
  }
  else if((NO_1)&&(NO_2)&&(!NO_3)&&(!NO_4))					//只有左边两个检测到黑线
  {
   miss_count=0;
   find_count=0;
   calc(x,y,x-1,y);
   x=x-1;
   go();
  }
  else if((!NO_1)&&(!NO_2)&&(NO_3)&&(NO_4))					//只有右边两个检测到黑线
  {
	miss_count=0;
    find_count=0;
    calc(x,y,x+1,y);
    x=x+1;
    go();
  }
  else if((!NO_1)&&(NO_2)&&(NO_3)&&(!NO_4))					//只有中间两个检测到黑线 
  {
	miss_count=0;
    find_count=0;
    calc(x,y,x,y+1);
    y=y+1;
    go();
  }
  else
  {
	miss_count=0;
	find_count=0;
	calc(x,y,x+1,y);
	x=x+1;
	go();
  }
 }
}

void show_point()
{
	set_point();
	x_end=x;
	y_end=y;
	write_command(0x01);
	
	write_address(0,0);
	write_word("终点坐标：");
	
	write_address(1,0);
	write_word("X=");
	write_data((int)x/10+48);
	write_data((int)x%10+48);
	
	write_address(1,3);
	write_word("Y=");
	write_data((int)y/100+48);
	write_data((int)y%100/10+48);
	write_data((int)y%10+48);
	
	write_address(2,0);
	write_word("实时坐标：");
	
	write_address(3,0);
	write_word("X=00");
	
	write_address(3,3);
	write_word("Y=000");
	
	dx=x/100;
	dy=(y+OFFSET-START_POINT)/100;
	
	x_pre=0;
	y_pre=START_POINT;
	
	
	for(i=0;i<100;i++)
	{
	 x=x_pre+dx;
	 y=y_pre+dy;
	 calc(x_pre,y_pre,x,y);
	 go();
	 
	 write_address(3,1);
	 write_data((int)x/10+48);
	 write_data((int)x%10+48);
	 
	 write_address(3,4);
	 write_data((int)(y-OFFSET)/100+48);
	 write_data((int)(y-OFFSET)%100/10+48);
	 write_data((int)(y-OFFSET)%10+48);
	 
	 x_pre=x;
	 y_pre=y;
	}
	
	write_address(3,1);
	write_data((int)x_end/10+48);
	write_data((int)x_end%10+48);
	 
	write_address(3,4);
	write_data((int)(y_end)/100+48);
	write_data((int)(y_end)%100/10+48);
	write_data((int)(y_end)%10+48);
}

void draw_spiral()
{
	t=0;
	angle=0;
	set_point();
	circ_x=x;
	circ_y=y+OFFSET;
	calc(0,START_POINT,circ_x,circ_y);
	go();
	x_pre=circ_x;
	y_pre=circ_y;
	for(i=0;i<4;i++)
	{
	 for(j=0;j<72;j++)
	 {
		t+=_PI/36;
		angle+=_PI/36;
		x=circ_x+t*cosf(angle);
		y=circ_y+t*sinf(angle);
		calc(x_pre,y_pre,x,y);
		go();
		x_pre=x;
		y_pre=y;
	 }
	 angle=0;
	}
}