/*!
	@file generic_point.hpp
	@brief Defining 2D or 3D coordition property (coordinates) for vertices.
	
	@author Ilaria Speranza and Mattia Tantardini

*/

#ifndef HH_GENERIC_POINT_HH
#define HH_GENERIC_POINT_HH

#include<array>
#include<iostream>

//! Class template for storing the vertex coordition in two or three dimentional space.
template <int dim, typename Storage_t = double>
class point {
	private:
		std::array<Storage_t, dim> coord;
	
	public:
		//! Default constructor for a 1D point:
		point<1, Storage_t>(const Storage_t& x = 0) : coord{{x}} {};
		
		//! Default constructor for a 2D point:
		point<2, Storage_t>(const Storage_t& x = 0, const Storage_t& y = 0) : coord{{x,y}} {};
		
		//! Default constructor for a 3D point:
		point<3, Storage_t>(const Storage_t& x = 0, , const Storage_t& y = 0, const Storage_t& z = 0) : coord{{x,y,z}} {};
		
		//! Copy constructor:
		point(const std::array<Storage_t, dim>& _coord) = default;
		
		//! Assignement operator:
		point<dim, Storage_t>& operator=(const point<dim, Storage_t>& _point) = default;
		
		
		//!Printing method:
		void print(){
			std::cout << "this point is: " << coord[0] << " " << coord[1];
			if(dim == 3)
				std::cout << " " << coord[2];
			std::cout << std::endl; 
		};
		
};

#endif // #ifnedf HH_GENERIC_POINT_HH
