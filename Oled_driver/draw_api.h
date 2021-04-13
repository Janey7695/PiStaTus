#ifndef __DRAW_API_H
#define __DRAW_API_H



#ifndef uchar
#define uchar unsigned char
#endif
//#ifndef uint
//#define uint unsigned int
//#endif


extern unsigned char BMP[1024];
void Draw_Point(uchar x,uchar y);//绘制单个点
void Draw_Line(uchar x1,uchar y1,uchar x2,uchar y2);//绘制一条直线，其中x1<x2。
void Draw_Rect(uchar x1,uchar y1,uchar x2,uchar y2,uchar Full);//绘制一个矩形，(x1,y1)是左上角坐标，另一个是右下角坐标，Full=1为填充模式，=0为不填充
void Draw_Tri(uchar x1,uchar y1,uchar x2,uchar y2,uchar x3,uchar y3);//绘制一个三角形
void Draw_Circle(uchar x,uchar y,uchar r);//绘制一个圆
void OLED_ShowCharRAM(uchar x,uchar y,uchar CHAR,uchar fontsize);
void OLED_ShowStrRAM(uchar x,uchar y,uchar*chr,uchar fontsize);
void OLED_ShowNumRAM(unsigned char x,unsigned char y,long int num,unsigned char len,unsigned char size);
void DisPlay(void);//显示绘制的图像
void CanvasClear(void);//清空画布

#endif


