CC=avr-gcc
OUTPUT = objs
VPATH=src
IPATH = inc
IPATH += include
# CFLAG = -mmcu=atmega128 -Os -I $(IPATH) -c
CFLAG = -mmcu=atmega128 \
		-DF_CPU=16000000 \
		-Os \
		-c \
		$(patsubst %, -I%, $(subst :, ,$(IPATH)))

srcs = uart.c gpio.c ch372.c usb_hid.c

objects = $(addprefix $(OUTPUT)/, $(srcs:.c=.o))
all:$(OUTPUT) $(objects)

$(objects):$(OUTPUT)/%.o:%.c
	$(CC) $(CFLAG) $< -o $@

.PHONY: $(OUTPUT) clean

$(OUTPUT):
	@mkdir -p $(OUTPUT)

clean:
	@-rm -rf $(OUTPUT)