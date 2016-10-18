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
	@brief Templete class to handle points in 2D or 3D (or even greater)
*/

#ifndef HH_GENERIC_POINT_HH
#define HH_GENERIC_POINT_HH

#include<array>
#include<iostream>
#include<initializer_list>
#include<type_traits>

namespace BGLgeom{

/*!
	@brief Class template for storing the vertex coordinates in n-dimentional space.
	
	@note Constructors and set method are implemented with std::initializer_list,
			so they have to be called with: method({args})
	@param dim Template argument that specifies the dimension of the space
	@param Storage_t Template argument that specifies the precision type for the coordinates
*/
template <unsigned int dim, typename Storage_t = double>
class point {
	public:
		//! Default constructor
		point(){
			for(auto & i : coord)
				i = static_cast<Storage_t>(0);
		}
		
		//! Constructor
		point(std::initializer_list<Storage_t> args){
			typename std::initializer_list<Storage_t>::iterator init_list_it, init_list_end;
			init_list_it = args.begin();
			init_list_end != args.end();
			std::size_t i = 0;
			for( ; init_list_it != init_list_end; ++init_list_it){
				coord[i] = *init_list_it;
				++i;
			}
		}
		
		//! Constructor from a std::array<Storage_t,dim>
		point(std::array<Storage_t,dim> const& P) : coord(P) {};
		
		//! Copy constructor
		point(point<dim, Storage_t> const&) = default;
		
		//! Assignement operator
		point<dim, Storage_t> & operator=(point<dim, Storage_t> const&) = default;
		
		//! Overload of assignment operator to create conversion directly form std::array<Storage_t, dim>
		point<dim, Storage_t> & operator=(std::array<Storage_t, dim> const& P){
			for(std::size_t i = 0; i < dim; i++){
				coord[i] = P[i];
			}
		}
		
		//======================= GETTING METHODS =========================
		
		//This three methods are useful when dealing with point<3>, more readable for the user
		//! Gets the first coordinate
		Storage_t x(){ return coord[0]; }
		Storage_t x() const { return coord[0]; }
		
		//! Gets the second coordinate
		Storage_t y(){ return coord[1]; }
		Storage_t y() const { return coord[1]; }
		
		//! Gets the third coordinate
		Storage_t z(){ return coord[2]; }
		Storage_t z() const { return coord[2]; }
		
		//! Gets the dimension of the point, and so the number of the coordinates
		std::size_t get_dim(){ return coord.size(); }
		std::size_t get_dim() const { return coord.size(); }
		
		//====================== SETTING METHODS ======================
		
		//This three methods are useful when dealing with point<3>, more readable for the useruseful 
		//! Set coordinate x of the point, corresponding to coord[0]
		void set_x(double const& x){ coord[0] = x; }
		
		//! Set coordinate y of the point, corresponding to coord[1]
		void set_y(double const& y){ coord[1] = y; }
		
		//! Set coordinate z of the point, corresponding to coord[i]
		void set_z(double const& z){ coord[2] = z; }
		
		//! Set method to assign coordinates to an already existing point
		void set(std::initializer_list<Storage_t> args){
			typename std::initializer_list<Storage_t>::iterator init_list_it, init_list_end;
			init_list_it = args.begin();
			init_list_end != args.end();
			std::size_t i = 0;
			for( ; init_list_it != init_list_end; ++init_list_it){
				coord[i] = *init_list_it;
				++i;
			}			
		}
		
		//========================== OPERATOR[] =======================
		
		//! Overloading of operator[], to get the i-th coordinate or write in it.
		Storage_t & operator[](std::size_t i){ return coord[i]; }
		Storage_t & operator[](std::size_t i) const { return coord[i]; }
		
		
		//================	I/O OPERATOR OVERLOADING ==================
		
		//! Overload of operator<<
		friend std::ostream & operator << (std::ostream & out, point<dim,Storage_t> const& P) {
			out << "(";
			std::size_t i = 0;
			for( ; i < dim-1; ++i){
				out << P.coord[i] << ","; 
			}
			out << P.coord[i] << ")";
		}
		
		//! operator>> overloading
		friend std::istream & operator >> (std::istream & in, point<dim,Storage_t> & P){
			for (std::size_t i=0; i < dim; i++){
				in >> P.coord[i];
			}
		}
		
		//======================== RELATIONAL OPERATOR =======================
		
		/*!
			 @brief Operator< overloading			 
			 @detail Point1 < Point2 if Point1.x is smaller than Point2.x;
			 		 if they are equal, compare in the same waythe y coordinate, and so on.		
		*/
		bool operator< (point<dim, Storage_t> const& P2) const {
			if(this->get_dim() != P2.get_dim())	//exception!!!
				return false;
								
			for(std::size_t i = 0; i < P2.get_dim(); i++){
				if(this->operator[](i) < P2[i])
					return true;
				else if (this->operator[](i) > P2[i])
					return false;
			}			
			return false;		//if they are equal
		}
		
		/*!
			@brief Operator> overloading			
			@detail It is the negation of operator<
		*/
		bool operator> (point<dim, Storage_t> const& point2) const {
			return !(*this < point2);
		}
		
		//======================= ARITHMETIC OPERATOR =====================
		
	 	//! Overloading of operator- for points
		friend point<dim,Storage_t> operator- (point<dim,Storage_t> const& P, point<dim,Storage_t> const& Q){
			point<dim,Storage_t> _P;
			for(std::size_t i = 0; i < dim; ++i)
				_P[i] = P[i] - Q[i];
			return _P;
		}
		
		/*! 
			@brief Overload of operator-
			@detail It defines difference between points and std::array, to define conversion
					between this two similar classes
		*/
		friend point<dim,Storage_t> operator- (point<dim,Storage_t> const& P, std::array<Storage_t,dim> const& a){
			point<dim,Storage_t> _P;
			for(std::size_t i = 0; i < dim; ++i)
				_P[i] = P[i] - a[i];
			return _P;
		}
		friend point<dim,Storage_t> operator- (std::array<Storage_t,dim> const& a, point<dim,Storage_t> const& P){
			return this->operator-(P,a);
		}
		
		/*!
			@brief Overloading of operator+ for points
		*/
		friend point<dim,Storage_t> operator+ (point<dim,Storage_t> const& P, point<dim,Storage_t> const& Q){
			point<dim,Storage_t> _P;
			for(std::size_t i = 0; i < dim; ++i)
				_P[i] = P[i] + Q[i];
			return _P;
		}
		
		/*! 
			@brief Overload of operator+
			@detail It defines sum between points and std::array, to define conversion
					between this two similar classes
		*/
		friend point<dim,Storage_t> operator+ (point<dim,Storage_t> const& P, std::array<Storage_t,dim> const& a){
			point<dim,Storage_t> _P;
			for(std::size_t i = 0; i < dim; ++i)
				_P[i] = P[i] + a[i];
			return _P;
		}
		friend point<dim,Storage_t> operator+ (std::array<Storage_t,dim> const& a, point<dim,Storage_t> const& P){
			return this->operator+(P,a);
		}
		
		/*! 
			@brief Overloading of operator*
			@detail It represents the multiplication of the coordinates of a point for a scalar
		*/
		friend point<dim,Storage_t> operator* (double const& k, point<dim,Storage_t> const& P){
			point<dim,Storage_t> _P;
			for(std::size_t i = 0; i < dim; ++i)
				_P[i] = k * P[i];
			return _P;
		}
		friend point<dim,Storage_t> operator* (point<dim,Storage_t> const& P, double const& k){
			return this->operator*(k,P);
		}
		
		/*! 
			@brief Overloading of operator/
			@detail It represents the division of the coordinates of a point for a scalar.
					Implemented using operator*
		*/
		friend point<dim,Storage_t> operator/ (point<dim,Storage_t> const& P, double const& k){
			return this->operator*(static_cast<Storage_t>(1/k), P);
		}
		
	private:
		std::array<Storage_t, dim> coord;
};	//point

}	//BGLgeom

#endif //HH_GENERIC_POINT_HH
