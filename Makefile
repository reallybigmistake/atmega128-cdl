CC=avr-gcc
FLAG = -mmcu=atmega128 -Os
MAP-FLAG = -Wl,-Map,$(TARGET .hex=.map)
VPATH=cdl/objs
VPATH += apps/objs
TARGET = main.hex
PRE-TARGET = main.axf

$(TARGET): mkcdl mkapps mkhex
program: $(TARGET)
	avrdude -p m128 -c usbasp -e -U flash:w:$(TARGET)

.PHONY: clean mkcdl mkapps mkhex

mkcdl:
	make -C cdl
mkapps:
	make -C apps
mkhex:
	make -f mkhex.mk

clean:clean-cdl clean-main clean-apps
clean-main:
	@make -f mkhex.mk clean
clean-cdl:
	@make -C cdl clean
clean-apps:
	@make -C apps clean