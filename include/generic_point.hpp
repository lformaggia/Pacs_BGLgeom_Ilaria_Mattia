/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_point.hpp
	@author Ilaria Speranza and Mattia Tantardini
	@date Sept, 2016
	@brief Defining 2D or 3D coordition property (coordinates) for vertices.
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
				out << P.coord[i] << " "; 
			}
		}
		
		//! operator>> overloading
		friend std::istream & operator >> (std::istream & in, point<dim,Storage_t> & P){
			for (int i=0; i < dim; i++){
				in >> P.coord[i];
			}
		}
		
		/*!
			 @brief Operator< overloading
			 
			 @detail Point1 < Point2 if Point1.x is smaller than Point2.x;
			 		 if they are equal, compare in the same waythe y coordinate, and so on.		
		*/
		bool operator< (point<dim, Storage_t> const& point2) const{
			if(this->get_dim() != point2.get_dim())	//exception!!!
				return false;
								
			for(std::size_t i = 0; i < point2.get_dim(); i++){
				if(this->get(i) < point2.get(i))
					return true;
				else if (this->get(i) > point2.get(i))
					return false;
			}			
			return false;		//if they are equal
		};
		
		/*!
			@brief Operator> overloading
			
			@detail It is the negation of operator<
		*/
		bool operator> (point<dim, Storage_t> const& point2)const{
			return !(*this < point2);
		};

		//Anche qui controllo se c'è effettivamente la coordinata da recuperare.

		//! Gets the first coordinate
		Storage_t x() const { return coord[0]; };
		
		//! Gets the second coordinate
		Storage_t y() const { return coord[1]; };
		
		//! Gets the third coordinate
		Storage_t z() const { return coord[2]; };
		
		//! Gets the i-th coordinate:
		Storage_t get(std::size_t i) const { return coord[i]; }
		
		//! Gets the dimension of the point:
		std::size_t get_dim() const {return coord.size(); };
		
		//! Set method to assign coordinates to an already existing point. It can take an arbitrary number of coordinates thanks to initializer_list
		void set(std::initializer_list<Storage_t> args){
			using init_list_it = typename std::initializer_list<Storage_t>::iterator;
			//! manca eventuale gestione degli errori da parte dell'utente
			int i=0;
			for(init_list_it it = args.begin(); it != args.end(); ++it){
				coord[i] = *it;
				++i;
			}			
		};	//set
		
};

#endif // #ifnedf HH_GENERIC_POINT_HH
