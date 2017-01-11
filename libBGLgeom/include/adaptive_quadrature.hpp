/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	adaptive_quadrature.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date	Sept, 2016
	@brief	Prototype for an integrating function
	@detail	It will implement an adaptive quadrature algorithm
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
