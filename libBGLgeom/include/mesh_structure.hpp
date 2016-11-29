/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*
	@file mesh_structure.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Definition of the types used for meshes inside the library
*/

#ifndef HH_MESH_STRUCTURE_HH
#define HH_MESH_STRUCTURE_HH

#include <vector>

namespace BGLgeom{
/*!
	@brief The structure containing a mesh
	@detail In this library we eill use as container for the mesh a std::vector
	@param dim Dimension of the space
*/
template <unsigned int dim>
using mesh = std::vector<BGLgeom::point<dim>>;

}	//BGLgeom
#endif	//HH_MESH_STRUCTURE_HH
