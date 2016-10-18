/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_edge_geometry.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief class for the generic geometry of an edge
	@detail 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_HH
#define HH_GENERIC_EDGE_GEOMETRY_HH

#include<array>
#include<functional>
#include"generic_point.hpp"

namespace BGLgeom{

template <unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class
generic_edge_geometry : public edge_geometry
{
	private:
	
	std::function<point<dim>(double)> value_fun;               //! stores the N=dim functions which, given the parameter, return the dim-th component
	std::function<std::vector(double)> first_derivative_fun;   //! stores the N=dim functions which, given the parameter, return the 1st derivative of the dim-th component
	std::function<std::vector(double)> second_derivative_fun;  //! stores the N=dim functions which, given the parameter, return the 2nd derivative of the dim-th component
	
	std::function<double(double)> curvilinear_abscissa_fun;    //! stores the function which, given the parameter, returns the corresponding curvilinear abscissa 
	
	public:
	
	//! full constructor
	generic_edge_geometry
	(std::function<point<dim>(double)> value_,
	 std::function<std::vector<double>(double)>  first_derivatives_,
	 std::function<std::vector<double>(double)>  second_derivatives_,
	 std::function<double(double)> curvilinear_abscissa_) :
	value_fun(value_),
	first_derivatives_fun(first_derivatives_),
	second_derivatives_fun(second_derivatives_),
	curvilinear_abscissa_fun(curvilinear_abscissa_)
	{};
	
	//! constructor whith only value_fun passed (the other members are computed inside the constructor)
	generic_edge_geometry
	(std::array<std::function<double(double)>, dim> value_) :
	value_fun(value_)
	{
		first_derivatives_fun = first_derivative;
		second_derivatives_fun = second_derivative; 
	
	};
	

	value (double parameter)
	{return value_fun(parameter);};
	

	
} //class


} //namespace


friend std::vector<double> first_derivative(const double x)
{
	//reads data from a data file
	GetPot   ifl("data.pot");
	double h = ifl("h", 0.05);
	std::cout<<"Spacing "<<h<<std::endl;
	
	// Point where derivative is computed
	std::cout<<"X value "<<x<<std::endl;

	double constexpr half(0.5);
		
	// Compute finite difference depending on the value x +_ h
	if(x+h > 1)
		point diff = (value_fun(x) - value_fun(x-h))/h;

	else if(x-h < 0)
		point diff = (value_fun(x+h) - value_fun(x))/h;

	else 
		point diff = half*(value_fun(x+h)-value_fun(x-h))/h;
		
	// Copy in a vector the coordinates of diff
	std::vector<double> dn(diff.coord, diff.coord + dim); 
		
	for(auto i: dn)
		std::cout << i << " ";
	std::cout << std::endl;
		
	return dn;	
}


friend std::vector<double> second_derivative(const double x)
{
	//reads data from a data file
	GetPot   ifl("data.pot");
	double h = ifl("h", 0.05);
	std::cout<<"Spacing "<<h<<std::endl;
	
	// Point where derivative is computed
	std::cout<<"X value "<<x<<std::endl;

	double constexpr half(0.5);
		
	// Compute finite difference depending on the value x +_ h
	if(x+h > 1)
		point diff = (first_derivative_fun(x) - first_derivative_fun(x-h))/h;

	else if(x-h < 0)
		point diff = (first_derivative_fun(x+h) - first_derivative_fun(x))/h;

	else 
		point diff = half*(first_derivative_fun(x+h)-first_derivative_fun(x-h))/h;
		
	// Copy in a vector the coordinates of diff
	std::vector<double> dn(diff.coord, diff.coord + dim); 
		
	for(auto i: dn)
		std::cout << i << " ";
	std::cout << std::endl;
		
	return dn;	
}
#endif
