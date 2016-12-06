/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file intersections2D.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Dec, 2016
	@brief Two helper functions for bspline edge geometry
	
	@detail
*/

#include "bspline_edge.hpp"

int
Pfindspan (int n, int p, double u, const vect &U)
// Find the knot span of the parametric point u.
//
// INPUT:
//
//   n - number of control points - 1
//   p - spline degree
//   u - parametric point
//   U - knot sequence
//
// RETURN:
//
//   s - knot span
//
// Note: This is NOT
// Algorithm A2.1 from 'The NURBS BOOK' pg68
// as that algorithm only works for nonperiodic
// knot vectors, nonetheless the results should
// be EXACTLY the same if U is nonperiodic
{
  // FIXME : this implementation has linear, rather than log complexity
  int ret = 0;

  if (u > U[U.size () - 1] || u < U[0])
    std::cerr << "Value " << u
              << " of u is outside the knot span by "
              << U[U.size () - 1] - u << "\n";
  else
    while ((ret++ < n) && (U[ret] <= u)) { };

  return (ret-1);
}

void
Pbasisfun (int i, double u, int p, const vect &U, vect &N)
// Basis Function.
//
// INPUT:
//
//   i - knot span  ( from Pfindspan() )
//   u - parametric point
//   p - spline degree
//   U - knot sequence
//
// OUTPUT:
//
//   N - Basis functions vector[p+1]
//
// Algorithm A2.2 from 'The NURBS BOOK' pg70.
{
  int j,r;
  double saved, temp;

  // work space
  double left[p+1];
  double right[p+1];

  N[0] = 1.0;
  for (j = 1; j <= p; j++)
    {
      left[j]  = u - U[i+1-j];
      right[j] = U[i+j] - u;
      saved = 0.0;
      for (r = 0; r < j; r++)
        {
          temp = N[r] / (right[r+1] + left[j-r]);
          N[r] = saved + right[r+1] * temp;
          saved = left[j-r] * temp;
        }
      N[j] = saved;
    }
}


