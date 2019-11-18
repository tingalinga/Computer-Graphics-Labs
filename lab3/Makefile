CC = g++
FRAMEWORKS = -lGLEW -framework GLUT -framework OpenGL
CFLAGS = -Wall -Wextra -Winline -Wno-deprecated -Og
INCLUDE = -I./include/

# For making zip file
MATRIC_NO = A0207552J
DIR = lab3
FILES_TO_ZIP = main.cpp images/birdface.jpg images/feather.jpg

all: main run
.PHONY: all

main: main.cpp image_io.o
	$(CC) $(FRAMEWORKS) $(CFLAGS) -o main main.cpp image_io.o

image_io.o: image_io.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -c image_io.cpp

run:
	./main

clean:
	rm -f main *.o ../$(MATRIC_NO)_lab3.zip

zip: $(FILES_TO_ZIP)
	cd .. && zip $(MATRIC_NO)_lab3.zip $(foreach file,$(FILES_TO_ZIP),$(DIR)/$(file))
