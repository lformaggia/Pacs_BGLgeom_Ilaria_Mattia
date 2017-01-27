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
between linear edges. It can handle both the 2 and the 3 dimensional setting.


#### fracture_intersection
This example concerns the creation of a graph starting from a raw set of 
fractures, i.e. lines, in a plane.

#### network_diffusion
In this example a simple diffusion problem on a graph representing a 
vascular network is solved.

-------------------------------------------------
## How to build the project
------------------------------------------------
### Prerequisites

To compile the whole project you need five libraries: the Boost Graph 
Library, Eigen and the VTK library are required for the whole project, 
while GetFEM++ and the MANworks library are used only for the network 
diffusion application.
We specify here the requirements for the first three libraries; for the 
last two, see the README file in the 'network_diffusion' folder.

##### Boost Graph Library

Version >= 1.61.0 required.

You can download it from here: <https://sourceforge.net/projects/boost/files/>.

You don't need to compile anything, since the BGL is a header only library. 

##### VTK library

Version >= 5.10 required

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


### Installation

#### Configuration variables

Along with this project you are provided with a Makefile.inc in this 
folder. Before doing anything, edit it and change:
- `PROJECT_FOLDER` with the path where this folder resides in your system
- `INSTALL_PATH`   with the path you want to install the library to
- `mkBGLInc`       with the path where the BGL resides in your system
- `TOL_VALUE`      with a value which represents the tolerance on being 
				   zero in the comparison of points. If you do not provide
				   any value, a default one will be used.

Only if you do not use the module system at MOX department or you installed 
a new version of the vtk library, you also need to change:
- `mkVtkInc`   with the path where the include files of the VTK library resides.
- `mkVtkLib`   with the path where the vtk library resides
- `mkEigenInc` with the path where the Eigen library resides

Now you are ready to build the library.

#### Build the library

Just type
```
make RELEASE=yes library
```
to compile the library (static and dynamic version), the associated tests, 
the documentation (LaTeX and html) and to install the library. Omit 
`RELEASE=yes` if you want to compile it in debug mode.

*NOTE*: you will always have to specificy RELEASE=yes if you want 
full optimization for the code. Debug mode mainly enables some further
controls and output messages.

Another way to compile the library is to enter the folder `libBGLgeom` 
and compile it from there. Type 
```
make help
```
to view all possible commands and details.

#### Run the tests



### Applications


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

To open the html documentation, go to `doc/html`, search for the file 
`index.html` and open it with your browser.

-----------------------------------------
##  DEV ENVIRONMENT
----------------------------------------
OS         : Scientific Linux 6.8 (Carbon) 64-bit

Processor  : Intel® Core™ i5-2430M CPU @ 2.40GHz × 4 

Compiler   : g++-4.9

vtk lib : 5.10

GetFEM++ lib: 5.0

Eigen lib: 3

Boost Graph Library: 1.61.0

MANworks lib: 1
