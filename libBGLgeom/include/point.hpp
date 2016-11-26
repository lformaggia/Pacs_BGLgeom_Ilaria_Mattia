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
#include <Eigen/Dense>

namespace BGLgeom{
/*!
	@brief Alias template for a point in N-th dimensional space, using Eigen::Matrix
*/
template <unsigned int N>
using point = Eigen::Matrix<double,1,N>;

//! Overload of the input operator for Eigen Matrices
template<typename Derived>
std::istream & operator>>(std::istream & in, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < m.rows(); ++i){
		for(std::size_t j = 0; j < m.cols(); ++j){
		    in >> m(i,j); 
	   }
	}
	return in;
}

/*! 
	@brief Overload of the output operator for Eigen Matrices
	@detail It prints each coordinate separated by a tab, in order to suite
			the expected output for the pts file format
*/
template<typename Derived>
std::ostream & operator<<(std::ostream & out, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < m.rows(); ++i){
		for(std::size_t j = 0; j < m.cols()-1; ++j){
		    out << m(i,j) << "\t"; 
	   }
	}
	out << m(m.rows()-1,m.cols()-1);
	return out;
}

} //BGLgeom

#endif //HH_GENERIC_POINT_HH
