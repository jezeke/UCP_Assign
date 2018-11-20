CC = gcc
CFLAGS = -Wall -pedantic -ansi
OBJ = effects.o turtle.o fileio.o main.o debug_turtle.o simple_turtle.o
EXECS = TurtleGraphics TurtleGraphicsSimple TurtleGraphicsDebug

TurtleGraphics: fileio.o turtle.o effects.o main.o
	$(CC) main.o fileio.o turtle.o  effects.o -lm -o TurtleGraphics

TurtleGraphicsSimple: fileio.o simple_turtle.o effects.o main.o
	$(CC) main.o fileio.o simple_turtle.o effects.o -lm -o TurtleGraphicsSimple

TurtleGraphicsDebug: fileio.o debug_turtle.o effects.o main.o
	$(CC) main.o fileio.o debug_turtle.o effects.o -lm -o TurtleGraphicsDebug

all: TurtleGraphics TurtleGraphicsSimple TurtleGraphicsDebug

main.o: main.c fileio.h turtle.h effects.h
	$(CC) -c main.c $(CFLAGS) -o main.o

effects.o: effects.c effects.h
	$(CC) -c effects.c $(CFLAGS) -o effects.o

turtle.o: turtle.c turtle.h effects.o effects.h fileio.o fileio.h
	$(CC) -c turtle.c $(CFLAGS) -o turtle.o

debug_turtle.o: turtle.c turtle.h effects.o effects.h fileio.o fileio.h
	$(CC) -c turtle.c $(CFLAGS) -DDEBUG -o debug_turtle.o

simple_turtle.o: turtle.c turtle.h effects.o effects.h fileio.o fileio.h
	$(CC) -c turtle.c $(CFLAGS) -DSIMPLE -o simple_turtle.o

fileio.o: fileio.c fileio.h linked_list.h
	$(CC) -c fileio.c $(CFLAGS) -o fileio.o

clean:
	rm -f $(OBJ) $(EXECS)
