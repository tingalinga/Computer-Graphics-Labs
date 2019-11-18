CC = g++
FRAMEWORKS = -framework GLUT -framework OpenGL
CFLAGS = -Wall -Wextra -Winline -Wno-deprecated -Og
MATRIC_NO = A0207552J
DIR = lab1

all: main run
.PHONY: all

main: main.cpp
	$(CC) $(FRAMEWORKS) $(CFLAGS) -o main main.cpp

run:
	./main

clean:
	rm -f main ../$(MATRIC_NO)_lab1.zip

zip: main.cpp
	cd .. && zip $(MATRIC_NO)_lab1.zip $(DIR)/main.cpp
