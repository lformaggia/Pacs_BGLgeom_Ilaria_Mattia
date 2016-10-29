#include <iostream>
#include <cmath>
#include "GetPot"
#include "numerical_integration.hpp"
#include "numerical_rule.hpp"


using namespace NumericalIntegration;
using namespace Geometry;



int main(int argc, char** argv){
  int nint; // quanto mettiamo il default? Lo leggiamo da GetPot?
  Domain1D domain(a,b);  // definito in domain.hpp incluso da mesh.hpp
  Mesh1D mesh(domain,nint); //definito in mesh.hpp
  
  Quadrature s(Simpson(),mesh); //def in numerical_integration.hpp che include quadratureRule.hpp e mesh.hpp. Simpson() is in numerical_rule

  cout<<" Now the mesh has "<<mesh.numNodes()<<" nodes"<<endl; //inutle

  double approxs=s.apply(f); //restituisce il valore dell'integrale

  cout<<"Integral computed with Simpson's method. Result: "<<approxs<<endl; 
}
  
