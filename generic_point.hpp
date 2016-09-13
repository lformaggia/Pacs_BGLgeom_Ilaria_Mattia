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
		
		//! operator >> overloading
		friend std::istream & operator >> (std::istream & in, point<dim,Storage_t> & P){
			for (int i=0; i < dim; i++){
				in >> P.coord[i];
			}
		}

		//! Getting the first coordinate:
		Storage_t x() const { return coord[0]; };
		
		//! Getting the second coordinate:
		Storage_t y() const { return coord[1]; };
		
		//! Getting the third coordinate:
		Storage_t z() const { return coord[2]; };
		
		//Per tutti i metodi set manca direi un controllo sul numero di argomenti in ingresso e la dimensione del punto.
		
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
