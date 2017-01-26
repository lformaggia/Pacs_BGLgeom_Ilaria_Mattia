/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	adaptive_quadrature.cpp
	@author Carlo de Falco
	@date	Jan, 2017
	@brief	Implementation of the integrand function
*/

#include <cmath>
#include <functional>
#include "adaptive_quadrature.hpp"

#define tol 1.0e-12
#define maxdepth 40

using namespace BGLgeom;

namespace BGLgeom{

int depth = 0;
int depth_reached = 0;

double
trapz (std::function<double (double)> f, double a, double b)
{
  double h = b - a;
  return .5 * h * (f(a) + f(b));
};

double
refine (std::function<double (double)> f, double a, double b, double oldval)
{ 
  ++depth;
  if (depth > depth_reached)
    depth_reached = depth;

  double c = .5 * a + .5 * b;

  double oldval_l = trapz (f, a, c);
  double oldval_r = trapz (f, c, b);
  double newval = oldval_l + oldval_r;
  
  double retval = newval;
  
  if (depth < maxdepth && fabs (newval - oldval) > tol)
    retval =
      refine (f, a, c, oldval_l) +
      refine (f, c, b, oldval_r);

  --depth;
  return retval;
};


double
integrate (std::function<double (double)> f, double a, double b)
{
  double res = 0;
  depth = 0;
  depth_reached = 0;
  res = refine (f, a, b, 0.0);
  return res;
};

}	//BGLgeom
