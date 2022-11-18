PiStaTus:PiStaTus.o oled.o draw_api.o getTemperature.o
	@gcc -o PiStaTus PiStaTus.o oled.o draw_api.o getTemperature.o -lwiringPi -lm
	@echo "Build PiStaTus successfully"
PiStaTus.o:PiStaTus.c
	@gcc -c PiStaTus.c
	@echo "Build dep PiStaTus.o successfully"
oled.o:./Oled_driver/oled.c
	@gcc -c ./Oled_driver/oled.c
	@echo "Build oled base driver"
draw_api.o:./Oled_driver/draw_api.c ./Oled_driver/oled.c
	@gcc -c ./Oled_driver/draw_api.c ./Oled_driver/oled.c
	@echo "Build oled api"

getTemperature.o:./utils/getTemperature.c
	@gcc -c ./utils/getTemperature.c
	@echo "Build utils - getTemperature "

clean:
	rm -f *.o PiStaTus

