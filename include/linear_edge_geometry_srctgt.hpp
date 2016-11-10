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
#include<Eigen/Dense>
#include"point.hpp"
#include"edge_geometry.hpp"

namespace BGLgeom{

template<unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class
linear_edge_geometry_srctgt: public BGLgeom::edge_geometry<dim>
{
	private:
	
	point<dim> src;
	point<dim> tgt;

	public:
	
	//! constructor 
	linear_edge_geometry_srctgt
	(point<dim> src_, point<dim> tgt_):src(src_),tgt(tgt_)
	{};
	
	//! Sets the value for the source
	void set_source(BGLgeom::point<dim> src_){
		src = src_; 
	}
	
	//! Sets the value for the target
	void set_target(point<dim> tgt_){
		tgt = tgt_;
	}
 
    //! returns the point corresponding
	virtual point<dim> value (const double parameter)
	{
		point<dim> P((tgt-src)*parameter+src); // copy-constructor: we copy in P the values of the line in correspondence of the indicated parameter
		return P;
	};
	
	//! first derivative
	virtual Eigen::Matrix<double,dim,1> 
	first_derivatives(const double x = 0)
	{
		return tgt-src;	
	}	
	
	
	//! second derivative
	virtual	Eigen::Matrix<double,dim,1> 					//! the second derivative is null along all the components
	second_derivatives(const double x = 0)
	{
		Eigen::Matrix<double,dim,1> v;
		v.fill(0.0);	
		return v;	
	}
	

	//! curvilinear abscissa
	virtual double curvilinear_abscissa(const double parameter)  // qui richiedo strettamente un parametro tra 0 e 1
	{
		return (tgt-src).norm()*parameter;
	};
	
}; //class


} //namespace






#endif
