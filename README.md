# Computer Graphics repo

This is my repository for the programs I developed at the course in [CS3241 Computer Graphics] at National University of Singapore. The code is written in C++ and uses OpenGL and Glut.

This is an example of one of the programs (lab2):

![Lab2][Program]

## Compiling and running the code
The source files should work on macOS *as is*, and an accompanying Makefile is provided for all of the labs so you can simply `cd` into the directory of interest and `make` the program.

If you're running this on Linux, you may need to download the [FreeGlut] library and place it in the same directory as the file being compiled.

If you're this on Windows, you will need to set up the compilation process yourself, but otherwise the code should work.

## The four labs

The different lab folders come in increasing difficulty and the main focus of each of them have been:

1. Getting a general idea about the structure of an OpenGL program.
2. Using matrix transformations and clipping planes appropiately.
3. Applying textures to objects and making a reflection using an imaginary viewpoint. Furthermore, creating an aesthetically pleasing and somewhat complex object.
4. Developing a raytracer that supports shadows and an arbitrary number of reflection levels. Furthermore, creating a scene using this raytracer.

More details as well as a visual preview of each lab is provided in their respective README.


[CS3241 Computer Graphics]: https://nusmods.com/modules/CS3241/computer-graphics
[Program]: https://media.giphy.com/media/Sw5rbrZ4MMRzsurBEX/giphy.gif
[FreeGlut]: http://freeglut.sourceforge.net