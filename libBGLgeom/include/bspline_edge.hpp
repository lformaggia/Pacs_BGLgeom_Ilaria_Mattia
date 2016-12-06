/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file bspline_edge.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Dec, 2016
	@brief B-spline edge. 
	@detail Given a vector of control points, this class implements a B-spline on the edge 
*/

// AGGIUNGERE TRA GLI AUTORI DE FALCO?

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

#ifndef HH_BSPLINE_EDGE_HH
#define HH_BSPLINE_EDGE_HH

#include <vector>
#include <cassert>
#include <cmath>
#include <functional>

#include "data_structure.hpp"


namespace BGLgeom{

using vect_dbl = std::vector<double>;

template<int dim = 3>
using point = BGLgeom::point<dim>;

template<int dim = 3>
using vect_pts = std::vector<point<dim> >;


// SOME AUXILIARY FUNCTIONS: IS IT CORRECT TO PUT THEM HERE? 
// The following two functions, the only two ones not templated, are implemented in bspline_edge.ccp
int Pfindspan (int n, int p, double u, const vect &U);
void Pbasisfun (int i, double u, int p, const vect &U, vect &N);

template<int dim>
void
Pbspderiv (int d, const vect_pts<dim> &C, int nc,
          const vect &k, int nk, vect_pts<dim> &dC, vect &dk)
// IN:
//    d  - degree of the B-Spline
//    C  - vector of control points  (mc x nc matrix stored as column major)
//    nc - number of control points
//    k  - knot sequence   (nk x 1 vector)
// OUT:
//    dc - vector of derivative control points  
//    dk - knot sequence   ((nk - 1) x 1 vector)
{
  int ierr = 0;
  int i, j;
  double tmp;
  
  for (i = 0; i < nc-1; i++)
    {
      tmp = d / (k[i+d+1] - k[i+1]);
      for (j = 0; j < dim; j++)
        dC[i](j) = tmp * (C[i+1](j) - C[i](j));
    }

  for (i = 1; i < nk-1; i++)
    dk[i-1] = k[i]; 
}


template<int dim>
void
Pbspeval (const int d, const vect_pts<dim> &C, const int nc,
         const vect &k, double u, point<dim> &P)
// Pbspeval:  Evaluate B-Spline at parametric points.
//
//    IN:
//       d - Degree of the B-Spline.
//       C - vector of control points.
//       k - Knot sequence, i_pt vector of size nk.
//       u - Parametric evaluation point.
//    OUT:
//       P - Evaluated point
{
  int s, tmp1, ii, i;
  vect N (d+1, 0.0);
  
  P = point<dim>::Zero(); //! Initialize the point to zero

  s = Pfindspan (nc-1, d, u, k);
  Pbasisfun (s, u, d, k, N);
  tmp1 = s - d;
  for (i = 0; i < dim; ++i)
    for (ii = 0; ii <= d; ++ii)
      P(i) += N[ii] * C[tmp1+ii](i);
}

template<int dim>
void
Pbspeval (const int d, const vect_pts<dim> &C, const int nc,
         const vect &k, const vect &u, vect_pts<dim> & P_vect)
// Pbspeval:  Evaluate B-Spline at parametric points.
//
//    IN:
//       d - Degree of the B-Spline.
//       C - vector of Control Points.
//       k - Knot sequence, i_pt vector of size nk.
//       u - Parametric evaluation points, i_pt vector of size nu.
//    OUT:
//       P_vect - Evaluated points, matrix of size (dim,nu)
{
  int s, tmp1, ii, i_vect, i_pt;
  vect N (d+1, 0.0);
  auto nu = u.size ();
  P_vect.resize (nu); //reserve nu places in the output vector
  
  // INITIALIZE ALL THE POINTS TO 0 (NECESSARY?)
  for(BGLgeom::point<dim> & PP: P_vect){
  	PP = BGLgeom::point<dim>::Zero();
  }
  
  for (i_vect = 0; i_vect < nu; ++i_vect)
    {
      s = Pfindspan (nc-1, d, u[i_vect], k);
      Pbasisfun (s, u[i_vect], d, k, N);
      tmp1 = s - d;
      for (i_pt = 0; i_pt < dim; ++i_pt)
        for (ii = 0; ii <= d; ++ii)
          P_vect[i_vect](i_pt) += N[ii] * C[tmp1 + ii](i_pt);
    }  
}


//HERE THE CLASS BEGINS
template <int dim = 3, int deg = 3>
class
bspline_edge
{

public:

  //! Constructor with control points
  bspline_edge (const vect_pts<dim> &C_)
    : C(C_), nc(C_.size ()), k(make_knots(nc))
  {

    // construction of spline for the vector of first derivative
    dC.resize (nc-1);
    for (point<dim> & PP: dC)
    	PP = point<dim>::Zero(); // initialize all the points to zero
    
    dk.resize (k.size () - 2, 0.0);
    Pbspderiv<dim> (deg, C, nc, k, k.size (), dC, dk);

    // construction of spline for the vector of second derivative
    d2k.resize (dk.size () - 2, 0.0);    
    d2C.resize (nc-2);
    for (point<dim> & PP: d2C)
    	PP = point<dim>::Zero(); // initialize all the points to zero
    	
    Pbspderiv<dim> (deg-1, dC, (nc-1), dk, dk.size (), d2C, d2k);      
    
  };

  //! Constructor with control points and knot vector
  bspline_edge (const vect_pts<dim> &C_, const vect_dbl &k_)
    : C(C_), k(k_), nc(C_.size ())
  {

    // construction of spline for the vector of first derivative
    dC.resize (nc-1);
    for (point<dim> && PP: dC)
    	PP = point<dim>::Zero(); // initialize all the points to zero
    	
    dk.resize (k.size () - 2, 0.0);
    Pbspderiv<dim> (deg, C, dim, nc, k, k.size (), dC, dk);

    // construction of spline for the vector of second derivative
    d2k.resize (dk.size () - 2, 0.0);    
    d2C.resize (nc-2);
    for (point<dim> && PP: d2C)
    	PP = point<dim>::Zero(); // initialize all the points to zero
    	
    Pbspderiv<dim> (deg-1, dC, dim, (nc-1), dk, dk.size (), d2C, d2k);      
    
  };

  point<dim> 
  operator() (double t) const
  {
    point<dim> P = point<dim>::Zero();
    Pbspeval<dim> (deg, C, nc, k, t, P);
    return P;
  };

  vect_pts<dim>
  operator() (const vect_dbl &t) const
  {
    vect_pts<dim> P_vect (t.size ());
    
    for (point<dim> && PP: P_vect)
    	PP = point<dim>::Zero(); // initialize all the points to zero
    
    Pbspeval<dim> (deg, C, nc, k, t, P_vect);
    return P_vect;
  };

  point<dim> 
  first_derivative (double t) const
  {
    point<dim> P = point<dim>::Zero();
    Pbspeval<dim> (deg-1, dC, nc-1, dk, t, P);
    return P;
  };

  vect_pts<dim>
  first_derivative (const vect_dbl &t) const
  {
    vect_pts<dim> P_vect (t.size ());
    for (point<dim> && PP: P_vect)
    	PP = point<dim>::Zero(); // initialize all the points to zero
    	
    Pbspeval<dim> (deg-1, dC, nc-1, dk, t, P_vect);
    return P_vect;
  };

  point<dim> 
  second_derivative (double t) const
  {
    point<dim> P = point<dim>::Zero();
    Pbspeval<dim> (deg-2, d2C, nc-2, d2k, t, P);
    return P;
  };

  vect_pts<dim>
  second_derivative (const vect_dbl &t) const
  {
    vect_pts<dim> P_vect (t.size ());
    
    for (point<dim> && PP: P_vect)
    	PP = point<dim>::Zero(); // initialize all the points to zero
    
    Pbspeval<dim> (deg-2, d2C, nc-2, d2k, t, P_vect);
    return P_vect;
  };

  double
  abscissa (double t) const
  {
    double retval =
      integrate ([&] (double u) {return velocity (u);}, 0, t);
    return retval;
  };

  vect
  abscissa (const vect_dbl &t) const
  {
    vect_dbl retval (t.size (), .0);
    for (int ii = 1; ii < t.size (); ++ii)
      retval[ii] = retval[ii-1] +
        integrate ([&] (double u) {return velocity (u);}, t[ii-1], t[ii]);
    return retval;
  };
  
private:

  const int nc;
  const vect_dbl k;
  const vect_pts<dim> C;
  vect_dbl dk, d2k;
  vect_pts<dim> dC, d2C;

  double
  velocity (double x)
  {
    point<dim> tmp = point<dim>::Zero();
    Pbspeval<dim> (deg-1, dC, (nc-1), dk, x, tmp);
    return tmp.norm();
  };

  vect_dbl 
  make_knots (int n)    // creates n knots in the interval [0,1]
  {
    vect_dbl retval;

    for (int ii = 0; ii < deg+1; ++ii)
      retval.push_back (0.0);

    int nint = n - (deg + 1);
    assert (nint >= 0);

    if (nint > 0)
      {
        double Dk = 1. / (nint + 1);
        for (int ii = 0; ii < nint; ++ii)
          retval.push_back (retval.back () + Dk);
      }
    
    for (int ii = 0; ii < deg+1; ++ii)
      retval.push_back (1.0);
    
    return retval;
  };
}; // class


} //namespace

#endif
