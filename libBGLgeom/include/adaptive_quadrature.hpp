/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	adaptive_quadrature.hpp
	@author Carlo de Falco
	@date	Jan, 2017
	@brief	Prototype for an integrating function
	
	It will implement an adaptive quadrature algorithm
*/

#ifndef HH_ADAPTIVE_QUADRATURE_HH
#define HH_ADAPTIVE_QUADRATURE_HH

#include <functional>

namespace BGLgeom{

extern "C" {
  double
  integrate (std::function<double (double)>, double, double);
}

}	//BGLgeom

#endif	//HH_ADAPTIVE_QUADRATURE_HH
