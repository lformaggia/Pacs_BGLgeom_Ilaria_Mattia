/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file data_structure.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Base abstract class to read input file

	@detail This file includes the definition of the minimal set of 
			geometric properties that will be used as vertex and edge
			properties in a graph that requires a geometric description.
			Two struct are defined: one for the vertex properties, one
			for the edge properties. \n
			If a user needs to define other different properties to be
			attached as vertex or edge properties in his graph, he can
			define new struct by himself inheriting from these ones. In
			this way he can continue using the geometrical structure of
			the graph.
		
		@todo Updating BGLgeom_edge_property in order to contain mesh
				generator and fem problems solutors
*/

#ifndef HH_DATA_STRUCTURE_HH
#define HH_DATA_STRUCTURE_HH

#include "generic_point.hpp"
#include "edge_geometry.hpp"

namespace BGLgeom{

/*!
	@brief Minimal data structure for the vertex geometrical properties
	@detail
	
	@param dim Space dimension
*/
template <unsigned int dim>
struct BGLgeom_vertex_property{
	//!Definition of some types which may be useful to see outside the struct
	using point_t = typename BGLgeom::point<dim>;

	//! Coordinates of the vertex
	//point_t P;
	BGLgeom::point<dim> P;
	
	//BC_type
};

/*!
	@brief Minimal data structure for the edge geometrical properties
	@detail
	
	@param dim Space dimension
*/
template <unsigned int dim>
struct BGLgeom_edge_property{
	//!Definition of some types which may be useful to see outside the struct
	using edge_geometry_t = typename BGLgeom::generic_edge_geometry<dim>;

	//! The class handling the parameterization of the edge
	//edge_geometry_t edge_geo;
	//BGLgeom::generic_edge_geometry<dim> edge_geo;
	
	//Mesh
	//Solutore
};

}	//BGLgeom

#endif	//HH_DATA_STRUCTURE_HH

//struct minimali da cui uno se vuole aggiungere roba poi eredita da queste

//create edge e magari qualche utility per settare la generic edge property , una funzionalità in più per l'utente

//Se non uso questa property, ma tipo no_property in adjacency list? va uguale?
