main:main.o oled.o
	gcc -o main main.o oled.o -lwiringPi
main.o:main.c
	gcc -c main.c
oled.o:./Oled_driver/oled.c
	gcc -c ./Oled_driver/oled.c

