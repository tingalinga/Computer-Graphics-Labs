## Lab 4

The purpose of this lab was to implement the Whitted Ray Tracing algorithm on some skeleton code and define an additional scene with it. For now, the raytracer works with the simple shapes of planes, spheres and triangles (from which a number of other shapes can be made with varying difficulti).

### Compiling and running the program
To compile, run

```
$ make main
```

To run the program and render the images, run

```
$ make run
```

### Images generated

The raytracer supports an arbitrary number of reflective levels and shadows. When run, the program will generate six images of scene 1 named the following:

![pics](https://i.imgur.com/TLnGRmr.jpg)


As well as the file `img_scene2` that looks like the following:

![scene2](https://i.imgur.com/W18D9g6.png)