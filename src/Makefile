OBJDIR = ../obj
INCDIR = ../include
OUTDIR = ../out

CC = gcc
CFLAGS = -Wall -std=c99 -I$(INCDIR)
MATH = -lm

_DEPS = pixels.h stb_image_write.h geometry.h models.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ = render.o pixels.o geometry.o models.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJ): $(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

render: $(OBJ)
	gcc -o $(OUTDIR)/$@ $^ $(CFLAGS) $(MATH)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~ 