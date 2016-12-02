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

#ifndef HH_BSPLINE_EDGE_HH
#define HH_BSPLINE_EDGE_HH

#include <vector>
#include <cassert>
#include <cmath>

#include "data_structure.hpp"


namespace BGLgeom{

template<unsigned int dim, unsigned int deg=3> // dim is the dimension of the space we are working in, deg is the degree of the spline
class bspline_edge { 

	using vect_pts = std::vector<BGLgeom::point<dim> >;
	using vect_dbl = std::vector<double>;

	private:
	
  	const int nc; // number of control points
  	const vect_pts C; // vector of control points  
  	const vect_dbl k; // vector of nodes
  	vect_pts dC, d2C; // vectors of control points for first and second derivatives
  	vect_dbl dk, d2k; // vectors of nodes for first and second derivatives

	public:
	

}; //class



template<unsigned int dim=3>
void
bspeval (const int d, 
		 const  std::vector<BGLgeom::point<dim> > &C,  
		 const int nc,
         const std::vector<double> &k, 
         double u, 
         point<dim> &P){
	
	// BSPEVAL:  Evaluate B-Spline at parametric points.
	//
	//    IN:
	//       d - Degree of the B-Spline.
	//       C - Control Points, vector of points.
	//       k - Knot sequence, row vector of size nk.
	//       u - Parametric evaluation point.
	//    OUT:
	//       P - Evaluated point
	
  	int s, tmp1, ii, col, row;
  	vect N (d+1, 0.0);
  	
  	// initialize all the components of P to 0
  	for(int i=0; i<dim; ++i)
  		P(i)= 0.0;

  	s = findspan (nc-1, d, u, k);
  	basisfun (s, u, d, k, N);
  	tmp1 = s - d;
  	for (int i = 0; i < dim; ++i) // fill the components of the output point
    	for (ii = 0; ii <= d; ++ii)
      		P(i) += N[ii] * c(row, tmp1 + ii);
};

void
bspeval (const int d, const vect &C, const int dim, const int nc,
         const vect &k, const vect &u, vect &P);

void
bspderiv (int d, const vect &C, int mc, int nc,
          const vect &k, int nk, vect &dC, vect &dk);



int
findspan (int n, int p, double u, const std::vector<double> &U)
// Find the knot span of the parametric point u.
//
// INPUT:
//   n - number of control points - 1
//   p - spline degree
//   u - parametric point
//   U - knot sequence
//
// RETURN:
//
//   s - knot span

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
basisfun (int i, double u, int p, const std::vector<double> &U, std::vector<double> &N)
// Basis Function.
//
// INPUT:
//
//   i - knot span  ( from FindSpan() )
//   u - parametric point
//   p - spline degree
//   U - knot sequence
//
// OUTPUT:
//
//   N - Basis functions vector[p+1]
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


} //namespace

#endif
