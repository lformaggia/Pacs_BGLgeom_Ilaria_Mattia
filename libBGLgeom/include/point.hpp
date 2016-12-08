/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file point.hpp
	@author Ilaria Speranza and Mattia Tantardini
	@date Sept, 2016
	@brief Description of a point and utilities
*/

#ifndef HH_POINT_HH
#define HH_POINT_HH

#include <iostream>
#include <iomanip>
#include <Eigen/Dense>

namespace BGLgeom{
/*!
	@brief Alias template for a point in N-th dimensional space, using Eigen::Matrix
*/
template <unsigned int N>
using point = Eigen::Matrix<double,1,N>;

//! Overload of the input operator for Eigen Matrices
template <typename Derived>
std::istream &
operator>>(std::istream & in, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < static_cast<unsigned int>(m.rows()); ++i){
		for(std::size_t j = 0; j < static_cast<unsigned int>(m.cols()); ++j){
		    in >> m(i,j); 
	   }
	}
	return in;
}

//! Overload of the output operator for Eigen Matrices
template <typename Derived>
std::ostream &
operator<<(std::ostream & out, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < static_cast<unsigned int>(m.rows()); ++i){
		for(std::size_t j = 0; j < static_cast<unsigned int>(m.cols())-1; ++j){
		    out << m(i,j) << " "; 
	   }
	}
	out << m(m.rows()-1,m.cols()-1);
	return out;
}

//! Function to write in the proper way in a pts file format
template <unsigned int dim>
void
write_point_pts(std::ostream & out, point<dim> const& P){
	out << std::setw(8) << 11 << std::setw(16);
	for(std::size_t i=0; i<dim-1; ++i)
		out << std::fixed << std::setprecision(8) << P(0,i) << std::setw(16);
	out << P(0,dim-1);

}

// Write_ASCII

/*!
	 @brief Operator< overloading			 
	 @detail Point1 < Point2 if Point1.x is smaller than Point2.x;
	 		 if they are equal, compare in the same waythe y coordinate, and so on.		
*/
template <typename Derived>
bool
operator< (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){							
	for(std::size_t i = 0; i < static_cast<unsigned int>(P1.cols()); ++i){
		if(P1(0,i) < P2(0,i))
			return true;
		else if (P1(0,i) > P2(0,i))
			return false;
	}			
	return false;		//if they are equal	
}

/*!
	@brief Operator> overloading			
	@detail It is the negation of operator<
*/
template <typename Derived>
bool
operator> (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){							
	return !(operator<(P1,P2));	
}

/*!
	@brief Operator== overloading			
	@detail It checks if all the coordinates are equal
*/
template <typename Derived>
bool
operator== (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){
	return P1 == P2;
}

/*!
	@brief Operator!= overloading			
	@detail It checks if the two points are different
*/
template <typename Derived>
bool
operator!= (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){
	return P1 != P2;
}


} //BGLgeom

#endif //HH_GENERIC_POINT_HH
