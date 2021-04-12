#ifndef __OLED_H
#define __OLED_H			  	 
  
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#ifndef u8
#define u8 unsigned char
#endif // !u8

#ifndef u16
#define u16 unsigned int
#endif // !u16

#ifndef u32
#define u32 unsigned long int
#endif // !u32

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    
	

#define LCD_CS       	17  //GPIO.0 (BCM->17)
#define LCD_DC        	27 //GPIO.2 (BCM->27)
#define LCD_RST     	22	//GPIO.3(BCM->22)
#define LCD_SCL        	11	// MOSI(BCM->11)
#define LCD_SDA        	10	// MOSI(BCM->10)




#define OLED_CMD  0	//0为写命令
#define OLED_DATA 1	//1为写数据



void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);

void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowString_Reverse(u8 x,u8 y,u8 *chr,u8 font);

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 font);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[]);
#endif 

