## Application: fractures intersection

**Author** : Ilaria Speranza & Mattia Tantardini 

**Date**   : January 2017

----------------------------------------------------
### The problem
----------------------------------------------------

The goal is to build a graph given a raw set of fractures 
(i.e. lines) in a plane. The final graph will have to consider 
each intersection point as a new vertex, and all the intersecting 
edges will have to be connected in the proper way and with the 
right properties. 

We also provide a simple hand-made test to show the how the algorithm 
works in all the different intersection situations (there are very 
unlucky to happen).

-----------------------------------------------------
### How to build this application
-----------------------------------------------------

#### Prerequisites

To compile this application, you need:

##### BGLgeom library

See `README` file in `../libBGLgeom` or `../README` for all 
information about dependencies for this library and how to 
build it.

#### Configuration variables

You should have already set all the needed variables in the `../Makefile.inc` 
when you compiled the BGLgeom library. If not, read `../README` and set the 
configuration variables.

#### Build the application

Type
```
make RELEASE=yes exec
```
to build the application. Omit `RELEASE=yes` if you want debug mode.

#### Build the test

The test is meaningful if compiled in debug mode: in this way all the 
output messages will be produced. To compile it, type
```
make -B test
```
The `-B` option force to recompile the source file with the helper 
functions in debug mode. If you omit it, you won't display the messages.

-------------------------------------------------------
### Run the application and the test
-------------------------------------------------------

Go to `/bin` and type
```
./main_fracture
```
to run the main application.

In the same folder, type
```
./main_example
```
to run the test executable.

Some output will be produced. They are all stored in the `/data` folder. 
The .vtk files can be displayed using Paraview. The `/data/vtk` folder 
contains all output files produced by the main application.

------------------------------------------
### Documentation
-----------------------------------------

To build the documentation for this application, type
```
make doc
```

This command will produce two type of documentation: LaTeX and html. 
To view the Latex documentation, type:
```
evince doc/latex/refman.pdf
```

To open the html documentation, go to `doc/html`, search for the file 
`index.html` and open it with your browser.

