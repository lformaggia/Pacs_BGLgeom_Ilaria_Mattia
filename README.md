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

### libBGLgeom
This library wants to expand the Boost Graph Library (*BGL*) in order to 
manage geometrical properties needed by graphs for particular applications.
For this purpouse, the library provides easy methods to append geometrical 
properties to a graph, such as coordinates for vertices or different 
geometries for the edges. It also provides functionalities to compute 
meshes, geometrical quantities related to the edges and find intersections 
between linear edges.

### fracture_intersection
This example concerns the creation of a graph starting from a raw set of 
fracture, i.e. lines, in a plane.

### network_diffusion
In this example a simple diffusion problem on a graph representing a 
vascular network is solved.
