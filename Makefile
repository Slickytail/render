OBJDIR = obj
INCDIR = include
OUTDIR = out
SRCDIR = src

CC = gcc
CFLAGS = -Wall -std=c99 -I $(INCDIR)
MATH = -lm

_DEPS = pixels.h stb_image_write.h geometry.h models.h stb_image.h lights.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ = render.o pixels.o geometry.o models.o lights.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

# Optimization/compilation modes

debug: CFLAGS += -DDEBUG -g -Og
debug: render

profile: CFLAGS += -pg
profile: render

build: CFLAGS += -O3
build: render

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

render: $(OBJ)
	gcc -o $(OUTDIR)/$@ $^ $(CFLAGS) $(MATH)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~ 