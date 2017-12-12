

main.bin: main.o
	avr-objcopy -O binary main.o main.bin

main.o: main.c
	avr-gcc -DF_CPU=16000000 -Os -mmcu=atmega328p main.c -o main.o

install: main.bin
	avrdude -c arduino -P /dev/ttyACM0 -p m328p -U flash:w:main.bin

clean:
	rm main.bin main.o
