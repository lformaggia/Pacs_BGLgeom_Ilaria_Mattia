## BGLgeom library

**Author** : Ilaria Speranza & Mattia Tantardini 

**Date**   : January 2017

-------------------------------------------------
## How to build the library
------------------------------------------------
### Prerequisites

To compile the library you need:

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
- `mkGetFEMHome`   with the path to the home folder of the GetFEM library (needed 
				   only to compile the `netwrok_diffusion` application)
- `mkMANworksHome` with the path to the home folder of the MANworks library (needed 
				   only to compile the `netwrok_diffusion` application)
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
make RELEASE=yes all
```
to compile the library (static and dynamic version), the associated tests, 
the documentation (LaTeX and html) and to install the library. Omit 
`RELEASE=yes` if you want to compile it in debug mode.

**NOTE**: you will always have to specificy `RELEASE=yes` if you want 
full optimization for the code. Debug mode mainly enables some further
controls and output messages.

You can also compile separately each component: type
```
make help
```
to view all possible commands and details.

#### Tests

Some test are provided. They want to be a way to show how some tools of 
the library works. You may also take a look to the source code to a more
exhaustive comprehencion.

Once compiled the tests, go to `/test` folder and execute them typing
```
./test_"name_of_the_test"
```

Many of the test will produce a .pts or a .vtp output. You can find 
them all in the `/data` folder. Their name respect the name of the 
test they come from.


#### Documentation

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
