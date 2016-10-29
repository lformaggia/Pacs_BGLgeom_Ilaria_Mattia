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
	
	std::function<BGLgeom::point<dim>(double)> value_fun;      //! stores the function x_i(s) = f_i(s), i=1:dim, s=0:1, f: [0,1] -> [0,1]
	BGLgeom::point<dim> src;
	BGLgeom::point<dim> tgt;

	public:
	
	//! constructor 
	linear_edge_geometry_srctgt
	(BGLgeom::point<dim> src_, BGLgeom::point<dim> tgt_):src(src_),tgt(tgt_)
	{
		value_fun = [](double s) -> BGLgeom::point<dim>
					{
						std::array<double,dim> coord;
						coord.fill(s);
						BGLgeom::point<dim> P;
						return P;
					}
	};
	
	//! default constructor 
	linear_edge_geometry_srctgt()
	{
		value_fun = [](double s) -> BGLgeom::point<dim>
					{
						std::array<double,dim> coord;
						coord.fill(s);
						BGLgeom::point<dim> P;
						return P;
					}
	};
	
	
	//! Sets the right value for the source (when initialized it has a dummy value, becuase at that point we don't have information about edge descriptor)
	void set_source(BGLgeom::point<dim> src_){
		src = src_; 
	}
	
	void set_target(BGLgeom::point<dim> tgt_){
		tgt = tgt_;
	}
 
    //! returns the point corresponding to s=0:1 
	virtual BGLgeom::point<dim> value (const double parameter)
	{
		//check if param belongs to 0->1
		return (tgt - src)*value_fun(parameter) + src;
	};
	
	//! first derivative
	virtual std::vector<double> 
	first_derivatives(const double x = 0)
	{
		point diff(this -> tgt - src);		
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
		return (tgt - src).norm()*(tgt - src).norm()*parameter;
	};

    //! returns the point corresponding to s=0:1 
	virtual BGLgeom::point<dim> value (const double parameter)
	{
		//check if param belongs to 0->1
		return (tgt - src) * value_fun(parameter) + src;
	};
	
}; //class


} //namespace






#endif
