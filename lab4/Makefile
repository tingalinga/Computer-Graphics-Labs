# For compiling
CC = g++
CFLAGS = -Wall -Wextra -Winline -Wno-deprecated -Og
INCLUDE = -I./include/
OBJS = ImageIO.o Camera.o Image.o Plane.o Sphere.o Triangle.o Util.o Raytrace.o

# For making zip file
MATRIC_NO = A0207552J
DIR = lab4
FILES_TO_ZIP = Main.cpp Raytrace.cpp Sphere.cpp img_r0.png \
				img_r0s.png img_r1.png img_r1s.png img_r2.png \
				img_r2s.png img_scene2.png img_spheres.png

all: main run
.PHONY: all

main: Main.cpp $(OBJS)
	$(CC) $(CFLAGS) -o main Main.cpp $(OBJS)

$(OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

run:
	./main

clean:
	rm -f *.o main ../$(MATRIC_NO)_lab4.zip

# Also remove images. Use with caution
clean-hard:
	@make clean
	rm -f *.png

zip: $(FILES_TO_ZIP)
	cd .. && zip $(MATRIC_NO)_lab4.zip $(foreach file,$(FILES_TO_ZIP),$(DIR)/$(file))
