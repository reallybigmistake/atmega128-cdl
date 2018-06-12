CC=avr-gcc
FLAG = -mmcu=atmega128 -Os
FLAG-OBJ = -mmcu=atmega128 -Os -c
VPATH=cdl/objs
TARGET = test.hex
PRE-TARGET = test
objects = cdl/objs/uart.o main.o 

$(TARGET): cdl $(PRE-TARGET)
	avr-objcopy -j .text -j .data -O ihex $(PRE-TARGET) $(TARGET)
$(PRE-TARGET): $(objects)
	$(CC) $(FLAG) -o $@ $^
main.o: test_config.h
	$(CC) $(FLAG-OBJ)  main.c
.PHONY: clean cdl
cdl:
	make -C cdl
clean:clean-cdl clean-main
clean-main:
	-rm $(TARGET) $(PRE-TARGET) main.o 
clean-cdl:
	@make -C cdl clean