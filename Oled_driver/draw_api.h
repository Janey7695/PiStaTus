#ifndef __DRAW_API_H
#define __DRAW_API_H




#ifndef uchar
#define uchar  unsigned char
#endif
#ifndef uint
#define uint unsigned int
#endif

typedef struct 
{
    void (*Point)(unsigned char x,unsigned char y);
    void (*Line)(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
    void (*Rect)(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char Full);
    void (*Tri)(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char x3,unsigned char y3);
    void (*Circle)(unsigned char x,unsigned char y,unsigned char r);
    void (*Picture)(unsigned char x,unsigned char y,unsigned char picture_Length,unsigned char picture_width,const unsigned char* Img);
}dpi_dw;

typedef struct 
{
    void (*Char)(unsigned char x,unsigned char y,unsigned char CHAR,unsigned char fontsize);
    void (*Str)(unsigned char x,unsigned char y,unsigned char*chr,unsigned char fontsize);
    void (*Number)(unsigned char x,unsigned char y,long int num,unsigned char len,unsigned char size);
}dpi_sw;

typedef struct 
{
    void (*Display)(void);
    void (*Clear)(void);
}dpi_cav;

typedef struct 
{
    dpi_dw Draw;
    dpi_sw Show;
    dpi_cav Canvas;
}dpi_funcstr;


extern dpi_funcstr OledPaint;


extern unsigned char BMP[1024];
void Draw_Point(unsigned char x,unsigned char y);//绘制单个点
void Draw_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);//绘制一条直线，其中x1<x2。
void Draw_Rect(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char Full);//绘制一个矩形，(x1,y1)是左上角坐标，另一个是右下角坐标，Full=1为填充模式，=0为不填充
void Draw_Tri(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char x3,unsigned char y3);//绘制一个三角形
void Draw_Circle(unsigned char x,unsigned char y,unsigned char r);//绘制一个圆
void OLED_ShowCharRAM(unsigned char x,unsigned char y,unsigned char CHAR,unsigned char fontsize);
void OLED_ShowStrRAM(unsigned char x,unsigned char y,unsigned char*chr,unsigned char fontsize);
void OLED_ShowNumRAM(unsigned char x,unsigned char y,long int num,unsigned char len,unsigned char size);
void Draw_Picture(uchar x,uchar y,uchar picture_Length,uchar picture_width,const unsigned char* Img);
void DisPlay(void);//显示绘制的图像
void CanvasClear(void);//清空画布
void Oled_DrawApi_Init(void);

#endif


