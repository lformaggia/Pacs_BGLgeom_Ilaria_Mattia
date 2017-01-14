# BGLgeom library
### Course on Advanced programming for scientific computing
#### *Politecnico di Milano* (ITALY)

**Author** : Ilaria Speranza & Mattia Tantardini 

**Date**   : January 2017

-----------------------------------------------
## The project
-----------------------------------------------
The BGLgeom library project is divided into three parts, each one in its 
own folder:
- `libBGLgeom` : Source code for BGLgeom library
- `fracture_intersection` : First application example
- `network_diffusion` : Second application example


#### libBGLgeom
This library wants to expand the Boost Graph Library (*BGL*) in order to 
manage geometrical properties needed by graphs for particular applications.
For this purpouse, the library provides easy methods to append geometrical 
properties to a graph, such as coordinates for vertices or different 
geometries for the edges. It also provides functionalities to compute 
meshes, geometrical quantities related to the edges and find intersections 
between linear edges.


#### fracture_intersection
This example concerns the creation of a graph starting from a raw set of 
fracture, i.e. lines, in a plane.

#### network_diffusion
In this example a simple diffusion problem on a graph representing a 
vascular network is solved.

-------------------------------------------------
## How to build the project
------------------------------------------------
### Prerequisites

You need three libraries to compile this project: the Boost Graph Library,
Eigen and the vtk library.

##### Boost Graph Library

Version >= 1.61.0 required.

You can download it from here: <https://sourceforge.net/projects/boost/files/>

You don't need to compile anything, since the BGL is a header only library. 

##### VTK library

You also need the vtk library, at least version 5.10

For more information see: <http://www.vtk.org/download/>

Alternatively, at Mox cluster just load the correspondent module:
```
module load vtk/5
```

##### Eigen

Version >= 3 required.

For more information see: <http://eigen.tuxfamily.org/index.php?title=Main_Page>

You do not have to compile anything, since the Eigen is a header only library.

Alternatively, at Mox cluster just load the correspondent module:
```
module load eigen/3
```


### Build the library

Along with this project you are provided with a Makefile.inc in this same 
folder. Before doing anything, edit it and change:
- `PROJECT_FOLDER` with the path where this folder resides in your system
- `INSTALL_PATH`   with the path you want to install the library to
- `mkBGLInc`       with the path where the BGL resides in your system

Only if you do not use the module system at MOX department or you installed 
a new version of the vtk library, you also need to change:
- `mkVtkInc`   with the path where the include files of the vtk library resides
- `mkVtkLib`   with the path where the vtk library resides
- `mkEigenInc` with the path where the Eigen library resides

Now you are ready to build the library.

REMEMBER: the default for the compilation is debug mode. If want full 
optimization, type `RELEASE=yes` inbetween `make` and its target.

*NOTE*: the debug mode mainly enables some more controls and output messages.

First of all, enter the folder `libBGLgeom`. To build all the library, the 
examples and the documentation and install the library, just type:
```
make all
```

You can also build each component of the library separately. Type 
```
make help
```
for information about make commands.

### Run the tests for the library

Almost all tests produce an output in pts and vtp format, that will be placed 
in the `/data` folder. All output path are relative path, so to run properly 
the exacutables and produce the expected output you need to enter the `/test`
folder and run each test form there.

### Build the application examples

You need to enter in each of the examples subfolder. In each of them, type
```
make all
```
to produce the exacutables and the doucumentation.

Type
```
make help
```
for other make commands.

REMEMBER: the default for the compilation is debug mode. If want full 
optimization, type `RELEASE=yes` inbetween `make` and its target.

*NOTE*: the debug mode mainly enables some more controls and output messages.

### Run the examples

As for the library's tests, you need to enter the `/bin` folder and run the 
exacutables form there. Produced output are put in the `/data` folder.

### Documentation

Both the library and the application are provided with documentation.
Building them with `make all`, all the documentation will be automatically 
produced. Otherwise, if you built components separately, type
```
make doc
```
in each folder to produce the documentation.

This command will produce two type of documentation: Latex and html. 
To view the Latex documentation, type:
```
evince doc/latex/refman.pdf
```

To open th html documentation, go to `doc/html`, search for the file 
`index.html` and open it with your browser.

-----------------------------------------
##  DEV ENVIRONMENT
----------------------------------------
OS         : Scientific Linux 6.8 (Carbon) 64-bit

Processor  : Intel® Core™ i5-2430M CPU @ 2.40GHz × 4 

Compiler   : g++-4.9

vtk lib : 5.10
