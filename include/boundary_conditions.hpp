/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file boundary_condition.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Data structure for boundary condition on a vertex
	@detail 
*/

#ifndef HH_BOUNDARY_CONDITION_HH
#define HH_BOUNDARY_CONDITION_HH

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
enum BC_t {NONE, DIR, NEU, MIX, OTHER};

/*!
	@brief The struct defining a general boundary condition
	@detail It contains the type of the boundary condition (one among those defined
			in BC_t) and the value associated to it.
	@param Value_t Defaulted to double. It is templated because one may need to use
					boundary conditions in which are needed more than one associated
					value, for example:
					\n
					The user may use a vector or an array to rapresent this
*/
template <typename Value_t = double>
struct BC{
	// Definition of types:
	using BC_type = BC_t;
	using value_type = Value_t;
		
	//! Type of the boundary condition
	BC_type type;
	//! The value associated to the boundary condition
	Value_type value;
	
	//! Default constructor
	BC() : type(NONE), value(0.) {};
	
	//! Constructor
	BC(BC_type _BC_t, Value_type _value) : type(_BC_t), value(_value) {};

	//! Copy constructor
	BC(BC const&) = default;
	
	//! Move constructor
	BC(BC &&) = default;
		
	//! Assignment operator
	BC & operator=(BC const&) = default;
	
	//! Move assignment
	BC & operator=(BC &&) = default;
};

}	//BGLgeom
#endif	//HH_BOUNDARY_CONDITION_HH
