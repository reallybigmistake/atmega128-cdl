CC=avr-gcc
FLAG = -mmcu=atmega128 -Os
FLAG-OBJ = -mmcu=atmega128 -Os -c
VPATH=cdl/objs
VPATH += apps/objs
TARGET = test.hex
PRE-TARGET = test
objects =  main.o console.o uart.o

$(TARGET): cdl apps $(PRE-TARGET)
	avr-objcopy -j .text -j .data -O ihex $(PRE-TARGET) $(TARGET)
$(PRE-TARGET): $(objects)
	$(CC) $(FLAG) -o $@ $^
main.o: test_config.h
	$(CC) $(FLAG-OBJ)  main.c
.PHONY: clean cdl apps
cdl:
	make -C cdl
apps:
	make -C apps
clean:clean-cdl clean-main clean-apps
clean-main:
	-rm $(TARGET) $(PRE-TARGET) main.o 
clean-cdl:
	@make -C cdl clean
clean-apps:
	@make -C apps clean