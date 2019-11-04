CC = g++
FRAMEWORKS = -lGLEW -framework GLUT -framework OpenGL
CFLAGS = -Wall -Wextra -Winline -Wno-deprecated -Og
INCLUDE = -I./include/

all:
	make main
	make run

main: main.cpp utilself.o image_io.o camera.o image.o plane.o raytrace.o sphere.o triangle.o raytrace.o
	$(CC) $(FRAMEWORKS) $(CFLAGS) -o main main.cpp utilself.o image_io.o camera.o image.o plane.o sphere.o triangle.o raytrace.o

image_io.o: ImageIO.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -o image_io.o -c ImageIO.cpp

camera.o: Camera.cpp
	$(CC) $(CFLAGS) -o camera.o -c Camera.cpp

image.o: Image.cpp
	$(CC) $(CFLAGS) -o image.o -c Image.cpp

plane.o: Plane.cpp
	$(CC) $(CFLAGS) -o plane.o -c Plane.cpp

sphere.o: Sphere.cpp
	$(CC) $(CFLAGS) -o sphere.o -c Sphere.cpp

triangle.o: Triangle.cpp
	$(CC) $(CFLAGS) -o triangle.o -c Triangle.cpp

utilself.o: Util.cpp
	$(CC) $(CFLAGS) -o utilself.o -c Util.cpp

raytrace.o: Raytrace.cpp
	$(CC) $(CFLAGS) -o raytrace.o -c Raytrace.cpp

run:
	./main

clean:
	rm -f *.o main
