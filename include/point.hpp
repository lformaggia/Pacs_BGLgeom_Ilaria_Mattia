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

#include<Eigen/Dense>


namespace BGLgeom{
/*!
	@brief Alias template: point<N> is the following Eigen::array
*/
template <unsigned int N>
using point = Eigen::Array<double,N,1>;
#endif //HH_GENERIC_POINT_HH
