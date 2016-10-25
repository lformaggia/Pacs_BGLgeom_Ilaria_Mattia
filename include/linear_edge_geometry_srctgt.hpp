/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_edge_geometry_srctgt.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief edge geometry source&target dependent
	@detail 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_HH
#define HH_GENERIC_EDGE_GEOMETRY_HH

#include<array>
#include<functional>
#include"generic_point.hpp"
#include "edge_geometry.hpp"

namespace BGLgeom{

template<unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class
linear_edge_geometry_srctgt: public BGLgeom::edge_geometry<dim>
{
	private:
	
	std::function<BGLgeom::point<dim>(double)> value_fun;      //! stores the function x_i(s) = f_i(s), i=1:dim, s=0:1, f: [0,1] -> [src,tgt]

	public:
	
	//! constructor 
	linear_edge_geometry_srctgt
	(BGLgeom::point<dim> src, BGLgeom::point<dim> tgt)
	{
		value_fun = [](double s) -> BGLgeom::point<dim>
					{
						return (tgt - src)*s + src;
					}
	};
	
	//! first derivative
	virtual std::vector<double> 
	first_derivatives(const double x = 0)
	{
		point diff(value_fun(1) - value_fun(0));		//! the value of the first derivative is tgt-src
		// Copy in a vector the coordinates of diff
		std::vector<double> dn(diff); 
		
		for(auto i: dn)
			std::cout << i << " ";
		std::cout << std::endl;
		
		return dn;	
	}	
	
	
	//! second derivative
	virtual	std::vector<double> 					//! the second derivative is null along all the components
	second_derivatives(const double x)
	{
		std::vector<double> dn(dim,0);  		
		return dn;	
	}
	

	//! curvilinear abscissa
	virtual double curvilinear_abscissa(const double parameter)
	{
	
	};

    //! returns the point corresponding to s=0:1 
	virtual BGLgeom::point<dim> value (const double parameter)
	{
		//check if param belongs to 0->1
		return value_fun(parameter);
	};
	
}; //class


} //namespace






#endif
