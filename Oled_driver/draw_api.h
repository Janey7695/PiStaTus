#ifndef __DRAW_API_H
#define __DRAW_API_H



#ifndef uchar
#define uchar unsigned char
#endif
//#ifndef uint
//#define uint unsigned int
//#endif


extern unsigned char BMP[1024];
void Draw_Point(uchar x,uchar y);//���Ƶ�����
void Draw_Line(uchar x1,uchar y1,uchar x2,uchar y2);//����һ��ֱ�ߣ�����x1<x2��
void Draw_Rect(uchar x1,uchar y1,uchar x2,uchar y2,uchar Full);//����һ�����Σ�(x1,y1)�����Ͻ����꣬��һ�������½����꣬Full=1Ϊ���ģʽ��=0Ϊ�����
void Draw_Tri(uchar x1,uchar y1,uchar x2,uchar y2,uchar x3,uchar y3);//����һ��������
void Draw_Circle(uchar x,uchar y,uchar r);//����һ��Բ
void OLED_ShowCharRAM(uchar x,uchar y,uchar CHAR,uchar fontsize);
void OLED_ShowStrRAM(uchar x,uchar y,uchar*chr,uchar fontsize);
void OLED_ShowNumRAM(unsigned char x,unsigned char y,long int num,unsigned char len,unsigned char size);
void DisPlay(void);//��ʾ���Ƶ�ͼ��
void CanvasClear(void);//��ջ���

#endif


