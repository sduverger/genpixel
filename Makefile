FLGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`
TARGET=genpixel
FILES=main.o gui.o image.o

%.o: %.c
	gcc -g $(FLGS) -c $< -o $@

$(TARGET): $(FILES)
	gcc -g $(FLGS) $^ -o $@ $(LIBS)

clean:
	@rm -f $(TARGET) *.o core
