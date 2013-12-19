ifneq ($(dbg),)
DBG=-g -Wall -W -DDEBUG
endif

.PHONY: debug

FLGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`
TARGET=genpixel
FILES=main.o gui.o image.o equation.o operators.o bmp.o utils.o

%.o: %.c
	gcc $(FLGS) $(DBG) -c $< -o $@

$(TARGET): $(FILES)
	gcc $(FLGS) $(DBG) $^ -o $@ $(LIBS)

debug:
	@make dbg=1

clean:
	@rm -f $(TARGET) *.o core
