/*********************************************************************************************************************
** 文件名称：nokia_5110_lcd  driver                                                                                 **
** 作    者：zz                                                                                                     **
** 时    间: 2014.06.30                                                                                             **
** 简    介：在stm32/ucos2 上实现的nokia5110 driver                                                                 **
*********************************************************************************************************************/   

#ifndef __NOKIA5110_H
#define __NOKIA5110_H 

extern unsigned char nokia_font6x8[][6]; 
extern unsigned char nokia_write_chinese[][24];

extern void nokia_LCD_init(void);
extern void nokia_LCD_clear(void);
extern void nokia_LCD_move_chinese_string(unsigned char X, unsigned char Y, unsigned char T); 
extern void nokia_LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
extern void nokia_LCD_write_chinese_string(unsigned char X, unsigned char Y,unsigned char ch_with,unsigned char num,unsigned char line,unsigned char row);
extern void nokia_chinese_string(unsigned char X, unsigned char Y, unsigned char T);                 
extern void nokia_LCD_write_char(unsigned char c);
extern void nokia_LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,unsigned char Pix_x,unsigned char Pix_y);
extern void nokia_LCD_write_byte(unsigned char dat, unsigned char dc);
extern void nokia_LCD_set_XY(unsigned char X, unsigned char Y);
extern void nokia_delay_1us(void);
#endif
