/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/* 
   Copyright (C) 2010-2016 Carlo de Falco

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
