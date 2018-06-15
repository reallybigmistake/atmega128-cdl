CC=avr-gcc
FLAG = -mmcu=atmega128 -Os
MAP-FLAG = -Wl,-Map,$(TARGET:.hex=.map)
VPATH=cdl/objs
VPATH += apps/objs
TARGET = main.hex
PRE-TARGET = main.axf
objects =  main.o console.o uart.o

$(TARGET): $(PRE-TARGET)
	avr-objcopy -j .text -j .data -O ihex $(PRE-TARGET) $(TARGET)
$(PRE-TARGET): $(objects)
	$(CC) $(FLAG) $(MAP-FLAG) -o $@ $^

.PHONY: clean
clean:
	-rm $(TARGET) $(PRE-TARGET)