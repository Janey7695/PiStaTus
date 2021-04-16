PiStaTus:PiStaTus.o oled.o draw_api.o
	gcc -o PiStaTus PiStaTus.o oled.o draw_api.o -lwiringPi -lm

PiStaTus.o:PiStaTus.c
	gcc -c PiStaTus.c

oled.o:./Oled_driver/oled.c
	gcc -c ./Oled_driver/oled.c

draw_api.o:./Oled_driver/draw_api.c ./Oled_driver/oled.c
	gcc -c ./Oled_driver/draw_api.c ./Oled_driver/oled.c


clean:
	rm -f *.o

