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
			the graph. \n
			All the data structures will be provided with a constructor,
			a copy constructor, a move constructor and the assignment
			operator. This because the idea is that the user can inherit
			form them to create his own user-defined data structures. But
			in this way, through inheritance, the user-defined class won't
			be anymore aggragates, and thus their initialization through
			the initializer list will be no longer available. We provide
			constructor for this reason, to allow the user to create
			and use properly his own data structures. \n
			For each data structure will be also provided the definitions
			of all types inside that data structure, in such a way that 
			they will be visible outside the class through the scope
			resolution operator (::). As a rule, the defined type will
			recall in the name that of the variable, to which will be 
			append a "_t" (= _type) at the end
		
		@todo Updating BGLgeom_edge_property in order to contain mesh
				generator and fem problems solutors
*/

#ifndef HH_DATA_STRUCTURE_HH
#define HH_DATA_STRUCTURE_HH

#include "generic_point.hpp"
#include "generic_edge_geometry.hpp"
//#include "boundary_conditions.hpp"

namespace BGLgeom{

/*!
	@brief An enum defining the type of the boundary condition we want to add in the node
	@detail The types of boundary conditions are: \n
			- NONE: the vertex doesn't contain a boundary condition; \n
			- DIR: Dirichlet boundary condition; \n
			- NEU: Neumann boundary condition; \n
			- MIX: Mixed boundary condition (for instance Robin or similar); \n
			- OTHER: A boundary condition of type different from all the previous ones
*/
enum BC_type {NONE, DIR, NEU, MIX, OTHER};

/*!
	@brief Minimal data structure for the vertex geometrical properties
	@detail
	
	@param dim Space dimension
	@param Value_t Defaulted to double. It is templated because one may need to use
					boundary conditions in which are needed more than one associated
					value, for example:
					\n
					The user may use a vector or an array to rapresent this
*/
template <unsigned int dim, typename Value_t = double>
struct Vertex_base_property{
	//!Definition of some types which may be useful to see outside the struct
	using point_t = typename BGLgeom::point<dim>;
	using BC_t = BC_type;
	using BC_value_t = Value_type;

	//! Coordinates of the vertex
	//point_t P;
	point_t coordinates;
	BC_t type;
	BC_value_t value;
	
	//BC_type
	
	
	//! Default constructor
	Vertex_base_property() : coordinates(), type(NONE), value(0.) {};
	
	//! Constructor
	Vertex_base_property(point_t _coordinates, BC_t _type, BC_value_t _value) : coordinates(_coordinates),
																				type(_type),
																				value(_value) {};
	
	//! Copy constructor
	Vertex_base_property(Vertex_base_property const&) = default;
	
	//! Move constructor
	Vertex_base_property(Vertex_base_property &&) = default;
	
	//! Assignment operator
	Vertex_base_property & operator=(Vertex_base_property const&) = default;
	
	//! Move assignment
	Vertex_base_property & operator=(Vertex_base_property &&) = default;
	
};	//Vertex_base_property

/*!
	@brief Minimal data structure for the edge geometrical properties
	@detail
	
	@param dim Space dimension
*/
template <unsigned int dim>
struct Edge_base_property{
	//!Definition of some types which may be useful to see outside the struct
	//using edge_geometry_t = typename BGLgeom::generic_edge_geometry<dim>;

	//! The class handling the parameterization of the edge
	//edge_geometry_t edge_geo;
	//BGLgeom::generic_edge_geometry<dim> edge_geo;
	
	//Mesh
	//Solutore
	
	
	//! Default constructor
	Edge_base_property(){};
	
	//! Constructor
	Edge_base_property(double a){};
	
	//! Copy constructor
	Edge_base_property(Edge_base_property const&) = default;
	
	//! Move_constructor
	Edge_base_property(Edge_base_property &&) = default;
	
	//! Assignment operator
	Edge_base_property & operator=(Edge_base_property const&) = default;
	
	//! Move assignment
	Edge_base_property & operator=(Edge_base_property &&) = default;
	
};	//Edge_base_property

}	//BGLgeom

#endif	//HH_DATA_STRUCTURE_HH

//struct minimali da cui uno se vuole aggiungere roba poi eredita da queste

//Se non uso questa property, ma tipo no_property in adjacency list? va uguale?
