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
#include <iostream>
#include "point.hpp"

namespace BGLgeom{
/*!
	@brief The structure containing a mesh
	@detail In this library we eill use as container for the mesh a std::vector
	@param dim Dimension of the space
*/
template <unsigned int dim>
using mesh = std::vector<BGLgeom::point<dim>>;

/*! 
	@brief Overload of the output operator 
	@detail We don't put in the output the first and the last point of the mesh,
			which coincide with source and target. The infos about them can be 
			recoverd through vertex properties
*/
tempalte <unsigned int dim>
std::ostream & operator<<(std::ostream & out, mesh<dim> const& M){
	mesh<dim>::iterator m_it = M.begin()+1;
	mesh<dim>::iterator m_end = M.end()-1;
	for( ; m_it != m_end; ++m_it)
		out << *m_it << std::endl;
	return out;
}	//operator<<

}	//BGLgeom
#endif	//HH_MESH_STRUCTURE_HH
