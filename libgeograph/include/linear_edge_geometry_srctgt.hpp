/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file linear_edge_geometry_srctgt.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief linear geometry for the edge
	@detail 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_HH
#define HH_GENERIC_EDGE_GEOMETRY_HH

#include<array>
#include<functional>
#include<Eigen/Dense>
#include"point.hpp"

namespace BGLgeom{

template<unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class
linear_edge_geometry_srctgt
{
	private:
	
	BGLgeom::point<dim> src;
	BGLgeom::point<dim> tgt;

	public:
	
	//! Constructor 
	linear_edge_geometry_srctgt
	(BGLgeom::point<dim> src_, BGLgeom::point<dim> tgt_) : src(src_), tgt(tgt_){};
	
	//! Sets the value for the source
	void set_source(BGLgeom::point<dim> src_){
		src = src_; 
	}
	
	//! Sets the value for the target
	void set_target(BGLgeom::point<dim> tgt_){
		tgt = tgt_;
	}
 
    //! returns the point corresponding
	virtual BGLgeom::point<dim> value (const double & x)
	{
		return BGLgeom::point<dim>((tgt-src)*x+src); // copy-constructor: we copy in P the values of the line in correspondence of the indicated parameter
	};
	
	//! first derivative
	virtual Eigen::Matrix<double,dim,1> 
	first_derivatives(const double & x = 0)
	{
		return tgt-src;	
	}	
	
	
	//! second derivative
	virtual	Eigen::Matrix<double,dim,1> 					//! the second derivative is null along all the components
	second_derivatives(const double & x = 0)
	{
		Eigen::Matrix<double,dim,1> v;
		v.fill(0.0);	
		return v;	
	}
	

	//! curvilinear abscissa
	virtual double curvilinear_abscissa(const double & parameter)  // qui richiedo strettamente un parametro tra 0 e 1
	{
		return (tgt-src).norm()*parameter;
	};
	
	//! Overload of operator<<
	friend std::ostream & operator << (std::ostream & out, linear_edge_geometry_srctgt<dim> & edge) {
		out<<"Source: "<<std::endl;
		out<<edge.value(0)<<std::endl;
		out<<std::endl;
		out<<"Value in s=0.5: "<<std::endl;
		out<<edge.value(0.5)<<std::endl;
		out<<std::endl;
		out<<"Target: "<<std::endl;
		out<<edge.value(1)<<std::endl;
		out<<std::endl;
		out<<"First derivatives in s=0: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.first_derivatives(0))[i]<<" ";
		out<<std::endl;
		out<<"First derivatives in s=0.5: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.first_derivatives(0.5))[i]<<" ";
		out<<std::endl;
		out<<"First derivatives in s=1: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.first_derivatives(1))[i]<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=0: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.second_derivatives(0))[i]<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=0.5: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.second_derivatives(0.5))[i]<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=1: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.second_derivatives(1))[i]<<" ";
		out<<std::endl;			
		out<<"Curvilinear abscissa in s=0: "<<edge.curvilinear_abscissa(0)<<std::endl;
		out<<"Curvilinear abscissa in s=0.5: "<<edge.curvilinear_abscissa(0.5)<<std::endl;
		out<<"Curvilinear abscissa in s=1: "<<edge.curvilinear_abscissa(1)<<std::endl;		
	}
	
}; //class


} //namespace






#endif
