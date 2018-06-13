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

srcs = uart.c
objects = $(addprefix $(OUTPUT)/, $(srcs:.c=.o))
all:$(OUTPUT) $(objects)
$(objects):$(srcs:.c = .h)
	$(CC) $(CFLAG) $< -o $@
.PHONY: $(OUTPUT)
$(OUTPUT):
	@mkdir -p $(OUTPUT)
clean:
	@-rm -rf $(OUTPUT)