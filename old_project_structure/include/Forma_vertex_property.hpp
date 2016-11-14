/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file Forma_vertex_property.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief This contains the struct for vertex properties that has to be used for Formaggia's example
	@detail 
*/

#ifndef HH_FORMA_VERTEX_PROPERTY_HH
#define HH_FORMA_VERTEX_PROPERTY_HH

#include "generic_point.hpp"

/*!
	@brief This struct contains the vertex property for Formaggia's example
	
	@detail
*/
struct Forma_vertex_property_t{
	//! It contains the vertex coordinates
	point<2> coord;
	//! It tracks if this is an external point (that is: it was in the input file, or it has degree = 1)
	bool is_external;
};	//Forma_vertex_property_t


#endif //HH_FORMA_VERTEX_PROPERTY_HH
