/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	boundary_condition.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Sept, 2016
	@brief	Data structure for boundary condition on a vertex
	@detail 
*/

#ifndef HH_BOUNDARY_CONDITION_HH
#define HH_BOUNDARY_CONDITION_HH

#include <iostream>

namespace BGLgeom{
/*!
	@brief An enum class defining the type of a boundary condition
	@detail The types of boundary conditions are: \n
			- NONE: the vertex doesn't contain a boundary condition. No value (defaulted to zero); \n
			- INT: Means "Internal". Thougth to be used in contiguous vertices of the graph. No value (defaulted to zero); \n
			- DIR: Dirichlet boundary condition; \n
			- NEU: Neumann boundary condition; \n
			- MIX: Mixed boundary condition (for instance Robin or similar); \n
			- OTHER: A boundary condition of type different from all the previous ones
*/
enum class BC_type {NONE, INT, DIR, NEU, MIX, OTHER};

/*!
	@brief The struct defining a single general boundary condition
	@detail It contains the type of the boundary condition (one among those defined
			in BC_t) and the value associated to it.
*/
struct boundary_condition{		
	//! Type of the boundary condition
	BC_type type;
	//! The value associated to the boundary condition
	double value;
	
	//! Default constructor
	boundary_condition() : type(BC_type::NONE), value(0.0) {};
	
	//! Constructor
	boundary_condition(BC_type _type, double _value) : type(_type), value(_value) {};

	//! Copy constructor
	boundary_condition(boundary_condition const&) = default;
	
	//! Move constructor
	boundary_condition(boundary_condition &&) = default;
	
	//! Destructor
	virtual ~boundary_condition() = default;
		
	//! Assignment operator
	boundary_condition & operator=(boundary_condition const&) = default;
	
	//! Move assignment
	boundary_condition & operator=(boundary_condition &&) = default;
	
	/*!
		@brief	Overload of input operator
		@detail Reads input of the form: "BC_type" "value"
	*/
	friend std::istream & operator>>(std::istream & in, boundary_condition & BC);

	/*!
		@brief	Overload of output operator
		@detail	Prints output in the form "BC_type" "value"
	*/
	friend std::ostream & operator<<(std::ostream & out, boundary_condition const& BC);
	
};	//boundary_condition

}	//BGLgeom
#endif	//HH_BOUNDARY_CONDITION_HH
