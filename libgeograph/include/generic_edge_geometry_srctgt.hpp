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
	@brief The most generic edge. 
	@detail The user must provide the function describing the edge and those describing forst and second derivative, all parametrized with paramenter between 0 and 1), 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_SRCTGT_HH
#define HH_GENERIC_EDGE_GEOMETRY_SRCTGT_HH

#include<array>
#include<functional>
#include<cmath>
#include"point.hpp"
#include"mesh.hpp"
#include"numerical_integration.hpp"
#include"numerical_rule.hpp"
#include "my_muparser_interface.hpp"


namespace BGLgeom{

template<unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class generic_edge_geometry_srctgt {

	using vector = Eigen::Matrix<double,dim,1>;

	private:
	
	std::function<BGLgeom::point<dim>(double)> value_fun;      //! stores the function f:[0,1] ->[src,tgt] representing the edge
	std::function<vector(double)> first_derivatives_fun;   
	std::function<vector(double)> second_derivatives_fun;	

	public:
	
	//! full constructor
	generic_edge_geometry_srctgt
	(std::function<BGLgeom::point<dim>(double)> value_, std::function<vector(double)> first_der_, std::function<vector(double)> second_der_): value_fun(value_), first_derivatives_fun(first_der_), second_derivatives_fun(second_der_)
	{};

    //! returns the point corresponding to s=0:1 
	BGLgeom::point<dim> value (const double & x)
	{
		//check if param belongs to 0->1
		return value_fun(x);
	};		
	
	//! first derivative
	vector 
	first_derivatives(const double & x)
	{
		return first_derivatives_fun(x);	
	};
	
	//! second derivative
	vector 
	second_derivatives(const double & x)
	{
		return second_derivatives_fun(x);	
	};
	
	//! curvilinear abscissa
	double curvilinear_abscissa(const double & x){
  		int nint=1000; // quanto mettiamo il default? Lo leggiamo da GetPot?
  		Geometry::Domain1D domain(0,x);  //estremi di integrazione
  		Geometry::Mesh1D mesh(domain,nint); //definito in mesh.hpp
  		
  		//lambda functions that returns the integrand function, i.e. norm(first_derivative(t))^2
  		auto abscissa_integrand = [&](double t) -> double{
			this -> first_derivatives(t).norm();
  		};
  		  
  		NumericalIntegration::Quadrature s(NumericalIntegration::Simpson(),mesh);

  		std::cout<<"Now the mesh has "<<mesh.numNodes()<<" nodes"<<std::endl; 

  		double approxs=s.apply(abscissa_integrand); //restituisce il valore dell'integrale

  		std::cout<<"Integral computed with Simpson's method. Result: "<<approxs<<std::endl;
  		return approxs; 
	}
	
	//! curvature
	double curvature(const double & s){
		if( (this->first_derivatives(s)).norm() == 0) return 0; // altrimenti al denominatore ho 0 
		double numerator( (this->first_derivative(s).cross(this->second_derivatives(s))).norm() );
		double denominator( pow(this->first_derivatives(s),3) );
		return numerator/denominator;
	}
	 
	
	//! Overload of operator<<
	friend std::ostream & operator << (std::ostream & out, generic_edge_geometry_srctgt<dim>& edge) {
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
