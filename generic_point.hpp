/*!
	@file generic_point.hpp
	@brief Defining 2D or 3D coordition property (coordinates) for vertices.
	
	@author Ilaria Speranza and Mattia Tantardini

*/

#ifndef HH_GENERIC_POINT_HH
#define HH_GENERIC_POINT_HH

#include<array>
#include<iostream>
#include<initializer_list>
#include<type_traits>

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
		
		point(std::initializer_list<Storage_t> args){  //! this method allows a generic number of input parameters
			using init_list_it = typename std::initializer_list<Storage_t>::iterator;
			//! manca eventuale gestione degli errori da parte dell'utente
			int i=0;
			for(init_list_it it = args.begin(); it != args.end(); ++it){
				coord[i] = *it;
				++i;
			}
				
			
		};
		
		//! Copy constructor:
		point(point<dim, Storage_t> const& _point) = default;/*{
			for(std::size_t i = 0; i < dim; i++)
				coord[i] = _coord[i];
		};*/
		
		//! Assignement operator:
		point<dim, Storage_t>& operator=(const point<dim, Storage_t>& _point) = default;
		

		//! operator << overloading
		friend std::ostream & operator << (std::ostream & out, point<dim,Storage_t> const & P) {
			for(int i=0; i < dim; ++i){
				std::cout << P.coord[i] << " "; 
			}
		}

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

		
		//! operator >> overloading
		friend std::istream & operator >> (std::istream & in, point<dim,Storage_t> & P){
			for (int i=0; i < dim; i++){
				in >> P.coord[i];
			}
		}
		
};

/*template<int dim, typename Storage_t>
std::ostream & operator << (std::ostream & out, point<dim,Storage_t> const & P){
			for(int i=0; i < dim; ++i){
				std::cout << P.coord[i] << " "; 
			}
		} */

/*
template <typename Storage_t = double>
point<1, Storage_t>::point(const Storage_t& x = 0) : coord{{x}} {};

template <typename Storage_t = double>
point<2, Storage_t>::point(const Storage_t& x = 0, const Storage_t& y = 0) : coord{{x,y}} {};

template <typename Storage_t = double>
point<3, Storage_t>::point(const Storage_t& x = 0, , const Storage_t& y = 0, const Storage_t& z = 0) : coord{{x,y,z}} {};
*/
#endif // #ifnedf HH_GENERIC_POINT_HH
