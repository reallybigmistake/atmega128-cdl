CC=avr-gcc
ROOT = .
OUTPUT = objs
VPATH=src
IPATH = inc
IPATH += include
IPATH += $(ROOT)/../cdl/include
CFLAG = -mmcu=atmega128 -D F_CPU=16000000 -Os -c $(patsubst %, -I%, $(subst :, ,$(IPATH)))

srcs = console.c main.c

objects = $(addprefix $(OUTPUT)/, $(srcs:.c=.o))
all:$(OUTPUT) $(objects)

$(objects):$(OUTPUT)/%.o:%.c
	$(CC) $(CFLAG) $< -o $@

.PHONY: $(OUTPUT)

$(OUTPUT):
	@mkdir -p $(OUTPUT)

clean:
	@-rm -rf $(OUTPUT)