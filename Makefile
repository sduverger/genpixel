FLGS=`pkg-config --cflags gtk+-3.0` #-Wall -W
LIBS=`pkg-config --libs gtk+-3.0`
TARGET=genpixel
FILES=main.o gui.o image.o equation.o operators.o bmp.o utils.o

%.o: %.c
	gcc -g $(FLGS) -c $< -o $@

$(TARGET): $(FILES)
	gcc -g $(FLGS) $^ -o $@ $(LIBS)

clean:
	@rm -f $(TARGET) *.o core
