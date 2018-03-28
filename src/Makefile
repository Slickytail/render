OBJDIR = ../obj
INCDIR = ../include

CC = gcc
CFLAGS = -Wall -std=c99 -I$(INCDIR)

_DEPS = pixels.h stb_image_write.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ = render.o pixels.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJ): $(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

render: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~ 