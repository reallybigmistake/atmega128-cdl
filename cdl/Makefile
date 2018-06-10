CC=avr-gcc
OUTPUT = objs
VPATH=src
IPATH = inc
IPATH += include
# CFLAG = -mmcu=atmega128 -Os -I $(IPATH) -c
CFLAG = -mmcu=atmega128 -Os -c $(patsubst %, -I%, $(IPATH))

srcs = io.c
objects = $(addprefix $(OUTPUT)/, $(srcs:.c=.o))
CFLAG += 
all:$(OUTPUT) $(objects)

$(objects):$(srcs:.c = .h)
	$(CC) $(CFLAG) $< -o $@
.PHONY: $(OUTPUT)
$(OUTPUT):
	@mkdir -p $(OUTPUT)
clean:
	@rm -rf $(OUTPUT)