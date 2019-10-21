CC = g++
FRAMEWORKS = -lGLEW -framework GLUT -framework OpenGL
CFLAGS = -Wall -Wextra -Winline -Wno-deprecated -Og
INCLUDE = -I./include/

all:
	make main
	make run

main: main.cpp image_io.h image_io.o
	$(CC) $(FRAMEWORKS) $(CFLAGS) -o main main.cpp image_io.o

image_io.o: image_io.cpp image_io.h
	$(CC) $(INCLUDE) $(CFLAGS) -c image_io.cpp

run:
	./main

clean:
	rm -f main
	rm -f *.o
