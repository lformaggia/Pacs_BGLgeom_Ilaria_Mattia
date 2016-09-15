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

//! Class template for storing the vertex coordinates in n-dimentional space.

template <int dim, typename Storage_t = double>
class point {
	private:
		std::array<Storage_t, dim> coord;
	
	public:
		//! Default constructor
		point(){
			for(std::size_t i = 0; i < dim; i++)
				coord[i] = static_cast<Storage_t>(0);
		};
		
		//! Constructor that takes a variable number of input parameters, thanks to initializer_list
		point(std::initializer_list<Storage_t> args){
			using init_list_it = typename std::initializer_list<Storage_t>::iterator;
			//! manca eventuale gestione degli errori da parte dell'utente
			std::size_t i = 0;
			for(init_list_it it = args.begin(); it != args.end(); ++it){
				coord[i] = *it;
				++i;
			}
		};
		
		//! Copy constructor
		point(point<dim, Storage_t> const& _point) = default;
		
		//! Assignement operator
		point<dim, Storage_t>& operator=(const point<dim, Storage_t>& _point) = default;
		

		//! operator<< overloading
		friend std::ostream & operator << (std::ostream & out, point<dim,Storage_t> const & P) {
			for(int i=0; i < dim; ++i){
				std::cout << P.coord[i] << " "; 
			}
		}
		
		//! operator>> overloading
		friend std::istream & operator >> (std::istream & in, point<dim,Storage_t> & P){
			for (int i=0; i < dim; i++){
				in >> P.coord[i];
			}
		}

		//Anche qui controllo se c'è effettivamente la coordinata da recuperare.

		//! Gets the first coordinate
		Storage_t x() const { return coord[0]; };
		
		//! Gets the second coordinate
		Storage_t y() const { return coord[1]; };
		
		//! Gets the third coordinate
		Storage_t z() const { return coord[2]; };
		
		//! Gets the i-th coordinate:
		Sotrage_t get(std::size_t i) { return coord[i]; }
		
		//! Gets the dimension of the point:
		std::size_t get_dim() {return coord.size(); };
		
		//! Set method to assign coordinates to an already existing point. It can take an arbitrary number of coordinates thanks to initializer_list
		void set(std::initializer_list<Storage_t> args){
			using init_list_it = typename std::initializer_list<Storage_t>::iterator;
			//! manca eventuale gestione degli errori da parte dell'utente
			int i=0;
			for(init_list_it it = args.begin(); it != args.end(); ++it){
				coord[i] = *it;
				++i;
			}			
		};
		
};

#endif // #ifnedf HH_GENERIC_POINT_HH
