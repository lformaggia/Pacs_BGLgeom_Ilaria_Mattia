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

#include <iostream>
#include <array>
#include <string>

//! Overload of operator= for assignments when there is only 1 BC.
/*
namespace{

template <typename T>
double std::array<T,1>::operator=(double const& val){
	return val;
}


}	//namespace

*/

namespace BGLgeom{
/*!
	@brief An enum class defining the type of the boundary condition we want to add in the node
	@detail The types of boundary conditions are: \n
			- NONE: the vertex doesn't contain a boundary condition; \n
			- DIR: Dirichlet boundary condition; \n
			- NEU: Neumann boundary condition; \n
			- MIX: Mixed boundary condition (for instance Robin or similar); \n
			- OTHER: A boundary condition of type different from all the previous ones
*/
enum class BC_type {NONE, DIR, NEU, MIX, OTHER};

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
template <unsigned int num_bc = 1>
struct boundary_condition{
	// Definition of types:
	//using BC_t = typename BGLgeom::BC_type;
	//using value_type = Value_t;
		
	//! Type of the boundary condition
	BC_type type;
	//! The value associated to the boundary condition
	std::array<double,num_bc> value;
	
	//! Default constructor
	boundary_condition() : type(BC_type::NONE){
		value.fill(.0);
	};
	
	//! Constructor
	boundary_condition(BC_type _type, std::array<double,num_bc> _value) : type(_type), value(_value) {};

	//! Copy constructor
	boundary_condition(boundary_condition const&) = default;
	
	//! Move constructor
	boundary_condition(boundary_condition &&) = default;
		
	//! Assignment operator
	boundary_condition & operator=(boundary_condition const&) = default;
	
	//! Move assignment
	boundary_condition & operator=(boundary_condition &&) = default;
	
	//! Overload of input operator
	friend std::istream & operator>>(std::istream & in, boundary_condition & BC){
		std::string type;
		in >> type;
		if(type == "NONE"){	// we suppose that there is a zero after the type. However, we check
			BC.type = BC_type::NONE;
			for(std::size_t i = 0; i < num_bc; ++i){
				in >> BC.value[i];
				if(BC.value[i] != .0)
					BC.value[i] = .0;
			}
			return in;
		} else if(type == "DIR"){
			BC.type = BC_type::DIR;
			for(std::size_t i = 0; i < num_bc; ++i)
				in >> BC.value[i];
			return in;
		} else if(type == "NEU"){
			BC.type = BC_type::NEU;
			for(std::size_t i = 0; i < num_bc; ++i)
				in >> BC.value[i];
			return in;
		} else if(type == "MIX"){
			BC.type = BC_type::MIX;
			for(std::size_t i = 0; i < num_bc; ++i)
				in >> BC.value[i];
			return in;
		} else if(type == "OTHER"){
			BC.type = BC_type::OTHER;
			for(std::size_t i = 0; i < num_bc; ++i)
				in >> BC.value[i];
			return in;
		}
	}	//operator>>

	//! Overload of output operator
	friend std::ostream & operator<<(std::ostream & out, boundary_condition const& BC){
		if(BC.type == BC_type::NONE)
			out << "BC NONE";	//se faccio << BC.type stampa solo il numero. devo fare controlli per stampare stringa
		else if(BC.type == BC_type::DIR){
			out << "BC DIR ";
			for(std::size_t i = 0; i < num_bc; ++i)
			out << BC.value[i] << " ";
			return out;
		} else if(BC.type == BC_type::NEU){
			out << "BC NEU ";
			for(std::size_t i = 0; i < num_bc; ++i)
			out << BC.value[i] << " ";
			return out;
		} else if(BC.type == BC_type::MIX){
			out << "BC MIX ";
			for(std::size_t i = 0; i < num_bc; ++i)
			out << BC.value[i] << " ";
			return out;
		} else if(BC.type == BC_type::OTHER){
			out << "BC OTHER ";
			for(std::size_t i = 0; i < num_bc; ++i)
			out << BC.value[i] << " ";
			return out;
		}
	}	//operator<<	
};	//boundary_condition

}	//BGLgeom
#endif	//HH_BOUNDARY_CONDITION_HH
