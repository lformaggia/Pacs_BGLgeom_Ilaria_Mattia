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

/*
template<typename Derived>
std::istream & operator>>(std::istream & s, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < m.rows(); ++i){
		for(std::size_t j = 0; j < m.cols(); ++j){
		    s >> m(i,j); 
	   }
	}
	return s;
}
*/

template <unsigned int N>
void read_point(std::istream & in, Eigen::Matrix<double,1,N> & P){
	double a;
	for(std::size_t i = 0; i < N; ++i){
		in >> a;
		P(0,i) = a;
	}
}


//! Overload of operator>> for BGLgeom::point<2>
std::istream & operator>>(std::istream & in, Eigen::Matrix<double,1,2> & P){
	for(std::size_t i = 0; i < P.cols(); ++i)
		in >> P(0,i);
}

//! Overload of operator>> for BGLgeom::point<3>
std::istream & operator>>(std::istream & in, Eigen::Matrix<double,1,3> & P){
	std::cout << P.rows() << ", " << P.cols() << std::endl;
	double a;
	for(int i = 0; i < (int)P.cols(); ++i){
		std::cout << in.peek() << std::endl;
		in >> a;
		P(0,i) = a;
	}
}

/*	//Funzionano, volendo. Non è vero.
//! Overload of operator<< for BGLgeom::point<2>
std::ostream & operator<<(std::ostream & out, Eigen::Matrix<double,1,2> const& P){
	out << "(" << P(0,1) << "," << P(0,2) << ")";
}

//! Overload of operator<< for BGLgeom::point<3>
std::ostream & operator<<(std::ostream & out, Eigen::Matrix<double,1,3> const& P){
	out << "(" << P(0,0) << "," << P(0,1) << ";" << P(0,2) << ")";
}
*/


} //namespace
#endif //HH_GENERIC_POINT_HH
