
#ifndef HAVE_ADAPTIVE_QUADRATURE_H
#define HAVE_ADAPTIVE_QUADRATURE_H
#include <functional>

namespace BGLgeom{

extern "C"
{
  double
  integrate (std::function<double (double)>, double, double);
}

}	//BGLgeom

#endif
