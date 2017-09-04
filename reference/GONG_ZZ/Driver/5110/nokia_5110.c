/*********************************************************************************************************************
** 文件名称：nokia_5110_lcd  driver                                                                                 **
** 作    者：zz                                                                                                     **
** 时    间: 2014.06.30                                                                                             **
** 简    介：在stm32/ucos2 上实现的nokia5110 driver                                                                 **
*********************************************************************************************************************/   
#include "includes.h"
#include "nokia_5110.h"

void nokia_delay_1us(void)                 //1us延时函数
{
   unsigned int i;
  for(i=0;i<1000;i++);					 //zz修改，此处非1us延时
}
void nokia_delay_1ms(void)                 //1ms延时函数
{
   unsigned int i;
   for (i=0;i<1140;i++);
}  
void nokia_delay_nms(unsigned int n)       //N ms延时函数
{
   unsigned int i=0;
   for (i=0;i<n;i++)
   nokia_delay_1ms();
}

#define   nokia_LCD_RST_L    GPIO_ResetBits(GPIOD, GPIO_Pin_0)
#define   nokia_LCD_RST_H    GPIO_SetBits(GPIOD, GPIO_Pin_0)
#define   nokia_LCD_CE_L     GPIO_ResetBits(GPIOD, GPIO_Pin_1)
#define   nokia_LCD_CE_H     GPIO_SetBits(GPIOD, GPIO_Pin_1)
#define   nokia_LCD_DC_L     GPIO_ResetBits(GPIOD, GPIO_Pin_2)
#define   nokia_LCD_DC_H     GPIO_SetBits(GPIOD, GPIO_Pin_2)		   
#define   nokia_SDIN_L       GPIO_ResetBits(GPIOD, GPIO_Pin_3)
#define   nokia_SDIN_H       GPIO_SetBits(GPIOD, GPIO_Pin_3)
#define   nokia_SCLK_L       GPIO_ResetBits(GPIOD, GPIO_Pin_4)
#define   nokia_SCLK_H       GPIO_SetBits(GPIOD, GPIO_Pin_4)

/*************************************************************************
LCD_init          : 3310LCD初始化
编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
*************************************************************************/
void nokia_LCD_init(void)
{
   	GPIO_InitTypeDef   GPIO_InitStructure;
									                  //LCD_RST 、LCD_CE、LCD_DC、SDIN、SCLK 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 |GPIO_Pin_4;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

		nokia_LCD_RST_L;
				nokia_delay_1us();
		nokia_LCD_RST_H;
		nokia_LCD_CE_L;
				nokia_delay_1us();
		nokia_LCD_CE_L;
				nokia_delay_1us();
				nokia_LCD_write_byte(0x21, 0);	// 使用扩展命令设置LCD模式
				nokia_LCD_write_byte(0xc8, 0);	// 设置偏置电压
				nokia_LCD_write_byte(0x06, 0);	// 温度校正
				nokia_LCD_write_byte(0x13, 0);	// 1:48
				nokia_LCD_write_byte(0x20, 0);	// 使用基本命令
				nokia_LCD_clear();	        // 清屏
				nokia_LCD_write_byte(0x0c, 0);	// 设定显示模式，正常显示		
		nokia_LCD_CE_H;
}

/*************************************************************************
LCD_clear         : LCD清屏函数
编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
*************************************************************************/
void nokia_LCD_clear(void)
{
    unsigned int i;

    nokia_LCD_write_byte(0x0c, 0);			
    nokia_LCD_write_byte(0x80, 0);			

    for (i=0; i<504; i++)
      nokia_LCD_write_byte(0, 1);			
}

/*************************************************************************
LCD_set_XY        : 设置LCD坐标函数
输入参数：X       ：0－83
          Y       ：0－5
编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
*************************************************************************/
void nokia_LCD_set_XY(unsigned char X, unsigned char Y)
{
    nokia_LCD_write_byte(0x40 | Y, 0);						// column
    nokia_LCD_write_byte(0x80 | X, 0);          	// row
}

/*************************************************************************
LCD_write_char    : 显示英文字符
输入参数：c       ：显示的字符；
编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
*************************************************************************/
void nokia_LCD_write_char(unsigned char c)
{
    unsigned char line;

    c -= 32;
    for (line=0; line<6; line++)
    nokia_LCD_write_byte(nokia_font6x8[c][line], 1);
}

/*************************************************************************
LCD_write_english_String  : 英文字符串显示函数
输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 		
*************************************************************************/
void nokia_LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
{
    nokia_LCD_set_XY(X,Y);
    while (*s) 
    {
				nokia_LCD_write_char(*s);
				s++;
    }
}
/*************************************************************************
LCD_write_chinese_string: 在LCD上显示汉字

输入参数：X、Y    ：显示汉字的起始X、Y坐标；
          ch_with ：汉字点阵的宽度
          num     ：显示汉字的个数；  
          line    ：汉字点阵数组中的起始行数
          row     ：汉字显示的行间距
编写日期          ：2004-8-11 
最后修改日期      ：2004-8-12 
测试：
	LCD_write_chi(0,0,12,7,0,0);
	LCD_write_chi(0,2,12,7,0,0);
	LCD_write_chi(0,4,12,7,0,0);	
*************************************************************************/                       
void nokia_LCD_write_chinese_string(unsigned char X,unsigned char Y,unsigned char ch_with,unsigned char num,unsigned char line,unsigned char row)
{
    unsigned char i,n;
    
    nokia_LCD_set_XY(X,Y);                             //设置初始位置
    
    for (i=0;i<num;)
    {
      	for (n=0; n<ch_with*2; n++)              //写一个汉字
      	{ 
      	    if (n==ch_with)                      //写汉字的下半部分
      	    {
      	        if (i==0) 
										nokia_LCD_set_XY(X,Y+1);
      	        else
      	           nokia_LCD_set_XY((X+(ch_with+row)*i),Y+1);
            }
      	    nokia_LCD_write_byte(nokia_write_chinese[line+i][n],1);
      	}
      	i++;
      	nokia_LCD_set_XY((X+(ch_with+row)*i),Y);
    }
}
  


/*************************************************************************
LCD_draw_map      : 位图绘制函数
输入参数：X、Y    ：位图绘制的起始X、Y坐标；
          *map    ：位图点阵数据；
          Pix_x   ：位图像素（长）
          Pix_y   ：位图像素（宽）
编写日期          ：2004-8-13
最后修改日期      ：2004-8-13 
*************************************************************************/
void nokia_LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,unsigned char Pix_x,unsigned char Pix_y)
{
    unsigned int i,n;
    unsigned char row;
    
    if (Pix_y%8==0) 
				row=Pix_y/8;      				//计算位图所占行数
    else
        row=Pix_y/8+1;
    
    for (n=0;n<row;n++)
    {
      	nokia_LCD_set_XY(X,Y);
        for(i=0; i<Pix_x; i++)
        {
            nokia_LCD_write_byte(map[i+n*Pix_x], 1);
        }
        Y++;                         //换行
    }      
}

/*************************************************************************
LCD_write_byte    : 使用SPI接口写数据到LCD

输入参数：data    ：写入的数据；
          command ：写数据/命令选择；

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-13 
*************************************************************************/
void nokia_LCD_write_byte(unsigned char dat, unsigned char command)
{
		unsigned char i;
	
    nokia_LCD_CE_L;   
			if (command == 0)
					nokia_LCD_DC_L;  //命令
			else
					nokia_LCD_DC_H;  //数据
		
			for(i=0;i<8;i++)
			{
					if(dat&0x80)
							nokia_SDIN_H;
					else
							nokia_SDIN_L;
					nokia_SCLK_L;
							dat = dat << 1;
							nokia_delay_1us();
					nokia_SCLK_H;
							nokia_delay_1us();
			}
    nokia_LCD_CE_H;
}

unsigned char nokia_font6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};
unsigned char nokia_write_chinese[][24]={

//测
	{0x89,0xF2,0x00,0xFF,0x01,0xF9,0xFF,0x00,0xFC,0x00,0xFF,0x00,0x00,0x07,0x04,0x04,0x02,0x01,0x02,0x04,0x01,0x04,0x07,0x00},
//试
	{0x11,0xF6,0x00,0x04,0x24,0xE4,0x24,0x24,0xFF,0x05,0x06,0x00,0x00,0x07,0x02,0x01,0x02,0x03,0x01,0x01,0x01,0x02,0x07,0x00},
//程
	{0x8A,0x6A,0xFF,0x49,0x20,0xAF,0xA9,0xE9,0xA9,0xAF,0x20,0x00,0x01,0x00,0x07,0x00,0x04,0x04,0x04,0x07,0x04,0x04,0x04,0x00},
//序
	{0x00,0xFE,0x42,0x4A,0x4A,0x5B,0xEA,0x5A,0x4A,0xC2,0x42,0x00,0x06,0x01,0x00,0x00,0x04,0x04,0x07,0x00,0x00,0x00,0x00,0x00},
        
       
//电
	{0x00,0xFC,0x94,0x94,0x94,0xFF,0x94,0x94,0x94,0xFE,0x04,0x00,0x00,0x01,0x00,0x00,0x00,0x03,0x04,0x04,0x04,0x04,0x06,0x00},
//子
	{0x20,0x21,0x21,0x21,0x21,0xF9,0x29,0x25,0x23,0x31,0x20,0x00,0x00,0x00,0x00,0x04,0x04,0x07,0x00,0x00,0x00,0x00,0x00,0x00},
//制
	{0x18,0xD6,0x54,0xFF,0x54,0x56,0xD4,0x00,0xFC,0x00,0xFF,0x00,0x00,0x03,0x00,0x07,0x00,0x02,0x03,0x00,0x04,0x04,0x07,0x00},
//作
	{0x10,0x08,0xFC,0x13,0x08,0x04,0xFF,0x24,0x24,0x24,0x04,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x01,0x01,0x01,0x01,0x00},
//杂
	{0x40,0x62,0x52,0xCA,0x47,0xE2,0x42,0xDE,0x50,0x50,0x58,0x00,0x04,0x02,0x01,0x04,0x04,0x07,0x00,0x00,0x01,0x02,0x04,0x00},
//志
	{0x04,0xA4,0x24,0xA4,0x24,0xBF,0x24,0x24,0x24,0xA6,0x04,0x00,0x02,0x01,0x00,0x07,0x04,0x04,0x05,0x04,0x06,0x00,0x03,0x00},
//社
	{0x84,0x44,0xF5,0x2E,0x40,0x10,0x10,0xFF,0x10,0x10,0x10,0x00,0x00,0x00,0x07,0x00,0x04,0x04,0x04,0x07,0x04,0x04,0x04,0x00},
	};


