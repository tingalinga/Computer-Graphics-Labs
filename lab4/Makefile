CC = g++
CFLAGS = -Wall -Wextra -Winline -Wno-deprecated -Og
INCLUDE = -I./include/
MATRIC_NO = A0207552J

all:
	make main
	make run

main: Main.cpp image_io.o util.o camera.o image.o plane.o raytrace.o \
		sphere.o triangle.o raytrace.o
	$(CC) $(CFLAGS) -o main Main.cpp util.o image_io.o \
			camera.o image.o plane.o sphere.o triangle.o raytrace.o

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

util.o: Util.cpp Util.h
	$(CC) $(CFLAGS) -o util.o -c Util.cpp

raytrace.o: Raytrace.cpp Raytrace.h
	$(CC) $(CFLAGS) -o raytrace.o -c Raytrace.cpp

run:
	./main

clean:
	rm -f *.o main ../$(MATRIC_NO)_lab4.zip

# Also remove images
clean-hard:
	make clean
	rm -f *.png

# Zips Main.cpp, Raytrace.cpp and Sphere.cpp as well as all images generated
zip: Main.cpp Raytrace.cpp Sphere.cpp img_r0.png img_r0s.png img_r1.png \
    	img_r1s.png img_r2.png img_r2s.png img_scene2.png img_spheres.png
	cd .. && zip $(MATRIC_NO)_lab4.zip lab4/Main.cpp lab4/Raytrace.cpp \
				lab4/Sphere.cpp lab4/img_r0.png lab4/img_r0s.png \
				lab4/img_r1.png lab4/img_r1s.png lab4/img_r2.png \
				lab4/img_r2s.png lab4/img_scene2.png lab4/img_spheres.png
