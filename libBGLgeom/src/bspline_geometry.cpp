/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/* 
   Copyright (C) 2010-2016 Carlo de Falco
   Copyright (C) 2007 Daniel Claxton
   Copyright (C) 2000 Mark Spink

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
	@file	bspline_geometry.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief 	Implementation of some helper functions needed by 
			bspline_geometry
*/

#include <functional>
#include <iostream>
#include <cstdlib>
#include "bspline_geometry.hpp"

using namespace BGLgeom;

namespace BGLgeom{

static inline
int
sub2ind (int i, int j, int m, int n)
{ return (i + j * m); }


int
findspan (int n, int p, double t, const vect &U) {
	int ret = 0;
	if (t > U[U.size () - 1] || t < U[0]){
		std::cerr << "Value " << t
	            << " of t is outside the knot span by "
	            << U[U.size () - 1] - t << "\n";
	    exit(EXIT_FAILURE);
	} else {
		while ((ret++ < n) && (U[ret] <= t)) { };
	}
	return (ret-1);
}	//findspan


void
basisfun (int i, double t, int p, const vect &U, vect &N){
	int j,r;
	double saved, temp;

	// work space
	double left[p+1];
	double right[p+1];

	N[0] = 1.0;
	for (j = 1; j <= p; j++) {
	    left[j]  = t - U[i+1-j];
	    right[j] = U[i+j] - t;
	    saved = 0.0;
	    for (r = 0; r < j; r++) {
	        temp = N[r] / (right[r+1] + left[j-r]);
	        N[r] = saved + right[r+1] * temp;
	        saved = left[j-r] * temp;
	    }
	    N[j] = saved;
	}
}	//basisfun

}	//BGLgeom
