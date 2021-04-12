main:./build/main.o ./build/oled.o
	gcc -o main ./build/mian.o ./build/oled.o -lwiringPi
./build/main.o:main.c
	gcc -c main.c
./build/oled.o:./Oled_driver/oled.c
	gcc -c ./Oled_driver/oled.c

