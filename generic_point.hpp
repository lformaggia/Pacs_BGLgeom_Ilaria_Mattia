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
		//! Default constructor:
		point(){
			for(std::size_t i = 0; i < dim; i++){
				coord[i] = static_cast<Storage_t>(0);
			}
			std::cout << "I'm using default constructor" << std::endl;
		};
	
		//! Default constructor for a 1D point:
		point(const Storage_t& x ) : coord{{x}} { std::cout << "1D constructor" << std::endl;};
		
		//! Default constructor for a 2D point:
		point(const Storage_t& x , const Storage_t& y ) : coord{{x,y}} {std::cout << "2D constructor" << std::endl;};
		
		//! Default constructor for a 3D point:
		point(const Storage_t& x , const Storage_t& y , const Storage_t& z ) : coord{{x,y,z}} {std::cout << "3D constructor" << std::endl;};
		
		//! Copy constructor:
		//point(const std::array<Storage_t, dim>& _coord) = default;
		
		//! Assignement operator:
		point<dim, Storage_t>& operator=(const point<dim, Storage_t>& _point) = default;
		
		//! Getting the first coordinate:
		Storage_t x() const { return coord[0]; };
		
		//! Getting the second coordinate:
		Storage_t y() const { return coord[1]; };
		
		//! Getting the third coordinate:
		Storage_t z() const { return coord[2]; };
		
		//Per tutti i metodi set manca direi un controllo sul numero di argomenti in ingresso e la dimensione del punto.
		
		//! Setting new coordinates for an already existing 1D point:
		void set(Storage_t const& x){
			coord[0] = x;
		};
		
		//! Setting new coordinates for an already existing 2D point:
		void set(Storage_t const& x, Storage_t const& y){
			coord[0] = x;
			coord[1] = y;
		};
		
		//!Setting new coordinates for an already existing 3D point:
		void set(Storage_t const& x, Storage_t const& y, Storage_t const& z){
			coord[0] = x;
			coord[1] = y;
			coord[2] = z;
		};
		
		//! Setting new coordinates for an already existing nD point:
		void set(std::array<Storage_t, dim> const& ppoint){
			for(std::size_t i = 0; i < dim; i++)
				coord[i] = ppoint[i];
		};
		
		//!Printing method:
		void print(){
			std::cout << "this point is: " << coord[0] << " " << coord[1];
			if(dim == 3)
				std::cout << " " << coord[2];
			std::cout << std::endl; 
		};
		
};
/*
template <typename Storage_t = double>
point<1, Storage_t>::point(const Storage_t& x = 0) : coord{{x}} {};

template <typename Storage_t = double>
point<2, Storage_t>::point(const Storage_t& x = 0, const Storage_t& y = 0) : coord{{x,y}} {};

template <typename Storage_t = double>
point<3, Storage_t>::point(const Storage_t& x = 0, , const Storage_t& y = 0, const Storage_t& z = 0) : coord{{x,y,z}} {};
*/
#endif // #ifnedf HH_GENERIC_POINT_HH
