CC = g++
FRAMEWORKS = -lGLEW -framework GLUT -framework OpenGL
CFLAGS = -Wall -Wextra -Winline -Wno-deprecated -Og
INCLUDE = -I./include/

all:
	make main
	make run && imgcat ./out2.png

main: main.cpp image_io.o utilself.o camera.o image.o plane.o raytrace.o sphere.o triangle.o raytrace.o
	$(CC) $(CFLAGS) $(FRAMEWORKS) -o main main.cpp utilself.o image_io.o camera.o image.o plane.o sphere.o triangle.o raytrace.o

image_io.o: ImageIO.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -o image_io.o -c ImageIO.cpp

camera.o: Camera.cpp Camera.h Vector3d.h
	$(CC) $(CFLAGS) -o camera.o -c Camera.cpp

image.o: Image.cpp Image.h ImageIO.h ImageIO.cpp
	$(CC) $(CFLAGS) -o image.o -c Image.cpp

plane.o: Plane.cpp Plane.h Surface.h
	$(CC) $(CFLAGS) -o plane.o -c Plane.cpp

sphere.o: Sphere.cpp
	$(CC) $(CFLAGS) -o sphere.o -c Sphere.cpp

triangle.o: Triangle.cpp Triangle.h
	$(CC) $(CFLAGS) -o triangle.o -c Triangle.cpp

utilself.o: Util.cpp Util.h
	$(CC) $(CFLAGS) -o utilself.o -c Util.cpp

raytrace.o: Raytrace.cpp Raytrace.h
	$(CC) $(CFLAGS) -o raytrace.o -c Raytrace.cpp

run:
	./main

clean:
	rm -f *.o main
