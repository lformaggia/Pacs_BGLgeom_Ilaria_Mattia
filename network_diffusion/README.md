## Application: diffusion on a vascular network

**Author** : Ilaria Speranza & Mattia Tantardini 

**Date**   : January 2017

----------------------------------------------------
### The problem
----------------------------------------------------

We generate from a matlab script a file containing a particular 
vascular network with some order of successive bifurcations. We 
use the file as input for the BGLgeom library to build the 
corresponding graph, and then we generate a uniform mesh on each 
edge. We output the mesh information in a pts file, that is then 
used as input for a FEM problem. Tools from MANworks library solve
the problem and output some vtk files.

-----------------------------------------------------
### How to build this application
-----------------------------------------------------

#### Prerequisites

To compile this application, you need:

##### BGLgeom library

See `README` file in `../libBGLgeom` or `../README` for all 
information about dependencies for this library and how to 
build it.

##### GetFEM++ library

Version >= 5.0 required. 

Download it from: <http://download.gna.org/getfem/html/homepage/download.html>

To compile it, follows the install instruction contained in the `INSTALL` 
file provided with the library.

##### MANworks library

You need the MANworks library developed by Domenico Notaro and Stefano Brambilla.

Download it from: <https://github.com/stefano-brambilla-853558/MANworks>

To properly install it, follow the instruction contained in the `README`. Note 
that you need to install first the GetFEM library.


#### Configuration variables

You have to go in `../Makefile.inc` and set (if you do not already do that):
- `mkGetFEMHome`   with the path to the home folder of the GetFEM library
- `mkMANworksHome` with the path to the home folder of the MANworks library


#### Build the application

Type
```
make RELEASE=yes exec
```
to build the application. Omit `RELEASE=yes` if you want debug mode.

This command will also execute the matlab script that will generate the 
input file for the executable.


-------------------------------------------------------
### Run the application
-------------------------------------------------------

#### Set input parameters

##### Generate Network

The matlab script `generate_bifurcation_network.m` will produce at the 
compilation stage the input file for the executable. You can also execute 
from matlab or octave to display the plot of the resulting network.

The network consists in an initial straigth edge (always fixed), that 
bifurcates a first time on a plane; then each extreme point of the two 
obtained bifurcates in the orthogonal plane; and then again the fourth 
bifurcate in the same plane as the first bifurcation, and so on. A random 
effect to the coordinates of each point is applied.

You can set some parameter which will affect the resulting network:
- *n*: number of order of bifurcations;
- *re*: entity of the random effect;
- *out_filename*: the path for the output. The default will place the output 
				  file in the `data` folder if the script is executed whitin 
				  the compilation stage.
				  
##### input.param

In the `input.param` file a lot of parameter to describe the problem to be 
solved can be set. The main parameters are the initial flags. We choose to 
solve a simple diffusion problem with tests value for the problem's parameters.

The file is automatically copied at compilation stage in the `bin` folder, 
ready to be passed to the executable.

See documentation in <https://github.com/stefano-brambilla-853558/MANworks> 
to know the meaning of all parameters.

#### Run the executable

Go to `/bin` and type
```
./main_network_diffusion input.param
```

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



