/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file point.hpp
	@author Ilaria Speranza and Mattia Tantardini
	@date Sept, 2016
	@brief Alias template of point
*/

#ifndef HH_POINT_HH
#define HH_POINT_HH

#include <iostream>
#include <Eigen/Dense>

namespace BGLgeom{
/*!
	@brief Alias template: point<N> is the following Eigen::array
*/
template <unsigned int N>
using point = Eigen::Matrix<double,1,N>;

template<typename Derived>
std::istream & operator>>(std::istream & s, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < m.rows(); ++i){
		for(std::size_t j = 0; j < m.cols(); ++j){
		    s >> m(i,j); 
	   }
	}
	return s;
}


/*
//! Overload of operator>> for BGLgeom::point<N>
template <unsigned int N>
std::istream & operator>>(std::istream & in, Eigen::Matrix<double,1,N> & P){
	for(std::size_t i = 0; i < N; ++i)
		in >> P[i];
}
*/
} //namespace
#endif //HH_GENERIC_POINT_HH
