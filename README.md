## Computer Graphics repo

This is my repository for the programs I develop at the course in Computer Graphics. The code is written in C++ and uses OpenGL and Glut.

This is an example of one of the programs (lab2): 
![Lab2](https://media.giphy.com/media/Sw5rbrZ4MMRzsurBEX/giphy.gif)


Given that the source file is `main.cpp`, this is how the files should be compiled and run on macOS:

```
$ gcc main.cpp -o main -framework GLUT -framework OpenGL -Wall -Wextra -Wno-deprecated
$ ./main
```

If you're running this on Windows or Linux, you may need to download the [FreeGlut](http://freeglut.sourceforge.net) library and place it in the same directory as the file being compiled.