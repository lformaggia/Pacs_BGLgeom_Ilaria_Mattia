/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file bspline_geometry.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Dec, 2016
	@brief B-spline edge. 
	@detail Implementation of some helper functions for bspline_geometry
*/

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
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <functional>
#include <iostream>
#include <cstdlib>
#include "bspline_geoemtry.hpp"

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
}

}	//BGLgeom;

/*

void
bspderiv (int d, const vect &C, int mc, int nc,
          const vect &k, int nk, vect &dC, vect &dk)
// IN:
//    d - degree of the B-Spline
//    c - control points  (mc x nc matrix stored as column major)
//    k - knot sequence   (nk x 1 vector)
// OUT:
//    dc - derivative control points  (mc x (nc - 1) matrix stored as column major)
//    dk - knot sequence   ((nk - 1) x 1 vector)
{
  int ierr = 0;
  int i, j;
  double tmp;
  
  // control point indexing
  auto ctrl = [&] (int ii, int jj) -> const double&
    { return C[sub2ind(ii,jj,mc,nc)]; };
  
  // indexing control points of the derivative
  auto dctrl = [&] (int ii, int jj) -> double&
    { return dC[sub2ind(ii,jj,mc,nc-1)]; };
  
  for (i = 0; i < nc-1; i++)
    {
      tmp = d / (k[i+d+1] - k[i+1]);
      for (j = 0; j < mc; j++)
        dctrl(j,i) = tmp * (ctrl(j,i+1) - ctrl(j,i));
    }

  for (i = 1; i < nk-1; i++)
    dk[i-1] = k[i]; 
}


void
bspeval (const int d, const vect &C, const int dim, const int nc,
         const vect &k, double u, vect &P)
// BSPEVAL:  Evaluate B-Spline at parametric points.
//
//    IN:
//       d - Degree of the B-Spline.
//       c - Control Points, matrix of size (dim,nc).
//       k - Knot sequence, row vector of size nk.
//       u - Parametric evaluation points, row vector of size nu.
//    OUT:
//       p - Evaluated points, matrix of size (dim,nu)
{
  int s, tmp1, ii, col, row;
  vect N (d+1, 0.0);
  P.assign (dim, 0.0);
  
  // control point indexing
  auto c = [&] (int iii, int jjj) -> const double&
    { return C[sub2ind(iii,jjj,dim,nc)]; };

  s = findspan (nc-1, d, u, k);
  basisfun (s, u, d, k, N);
  tmp1 = s - d;
  for (row = 0; row < dim; ++row)
    for (ii = 0; ii <= d; ++ii)
      P[row] += N[ii] * c(row, tmp1 + ii);
}

void
bspeval (const int d, const vect &C, const int dim, const int nc,
         const vect &k, const vect &u, vect &P)
// BSPEVAL:  Evaluate B-Spline at parametric points.
//
//    IN:
//       d - Degree of the B-Spline.
//       c - Control Points, matrix of size (dim,nc).
//       k - Knot sequence, row vector of size nk.
//       u - Parametric evaluation points, row vector of size nu.
//    OUT:
//       p - Evaluated points, matrix of size (dim,nu)
{
  int s, tmp1, ii, col, row;
  vect N (d+1, 0.0);
  auto nu = u.size ();
  P.assign (dim*nu, 0.0);
  
  // control point indexing
  auto c = [&] (int iii, int jjj) -> const double&
    { return C[sub2ind(iii,jjj,dim,nc)]; };
  // evaluated points indexing
  auto p = [&] (int iii, int jjj) -> double&
    { return P[sub2ind(iii,jjj,dim,nu)]; };

  for (col = 0; col < nu; ++col)
    {
      s = findspan (nc-1, d, u[col], k);
      basisfun (s, u[col], d, k, N);
      tmp1 = s - d;
      for (row = 0; row < dim; ++row)
        for (ii = 0; ii <= d; ++ii)
          p(row,col) += N[ii] * c(row, tmp1 + ii);
    }  
}


*/
