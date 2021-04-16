#ifndef __DRAW_API_H
#define __DRAW_API_H




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
void Draw_Point(unsigned char x,unsigned char y);//���Ƶ�����
void Draw_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);//����һ��ֱ�ߣ�����x1<x2��
void Draw_Rect(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char Full);//����һ�����Σ�(x1,y1)�����Ͻ����꣬��һ�������½����꣬Full=1Ϊ���ģʽ��=0Ϊ�����
void Draw_Tri(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char x3,unsigned char y3);//����һ��������
void Draw_Circle(unsigned char x,unsigned char y,unsigned char r);//����һ��Բ
void OLED_ShowCharRAM(unsigned char x,unsigned char y,unsigned char CHAR,unsigned char fontsize);
void OLED_ShowStrRAM(unsigned char x,unsigned char y,unsigned char*chr,unsigned char fontsize);
void OLED_ShowNumRAM(unsigned char x,unsigned char y,long int num,unsigned char len,unsigned char size);
void Draw_Picture(unsigned char x,unsigned char y,unsigned char picture_Length,unsigned char picture_width,const unsigned char* Img);
void DisPlay(void);//��ʾ���Ƶ�ͼ��
void CanvasClear(void);//��ջ���
void Oled_DrawApi_Init(void);

#endif


