main:main.o oled.o draw_api.o
	gcc -o main main.o oled.o draw_api.o -lwiringPi

main.o:main.c
	gcc -c main.c

oled.o:./Oled_driver/oled.c
	gcc -c ./Oled_driver/oled.c

draw_api.o:./Oled_driver/draw_api.c
	gcc -c ./Oled_driver/draw_api.c

