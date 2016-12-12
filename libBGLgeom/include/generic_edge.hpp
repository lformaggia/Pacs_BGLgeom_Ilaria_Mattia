/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_edge.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief The most generic edge. 
	@detail The user must provide the function describing the edge and those describing forst and second derivative, all parametrized with paramenter between 0 and 1), 
*/

#ifndef HH_GENERIC_EDGE_HH
#define HH_GENERIC_EDGE_HH

#include<array>
#include<functional>
#include<cmath>
#include<iostream>
#include <cmath>
#include"point.hpp"
//#include"mesh.hpp"
//#include"numerical_integration.hpp"
//#include"numerical_rule.hpp"
#include "adaptive_quadrature.hpp"


namespace BGLgeom{

template<unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class generic_edge {

	using vect_Eigen = Eigen::Matrix<double,1,dim>;
	using point = BGLgeom::point<dim>;
	using vect_pts = std::vector<point>;

	private:
	
	std::function<point(double)> value_fun;      //! stores the function f:[0,1] ->[src,tgt] representing the edge
	std::function<vect_Eigen(double)> first_derivatives_fun;   
	std::function<vect_Eigen(double)> second_derivatives_fun;	

	public:
	
	//! full constructor
	generic_edge(std::function<point(double)>  const& value_,
				 std::function<vect_Eigen(double)> const& first_der_,
				 std::function<vect_Eigen(double)> second_der_) :
				 			 value_fun(value_),
				 			 first_derivatives_fun(first_der_),
				 			 second_derivatives_fun(second_der_) {};

    //! returns the point corresponding to s=0:1     
	point operator() (double t) const
	{
		//check if param belongs to 0->1
		return value_fun(t);
	};
	
  	vect_pts
  	operator() (const std::vector<double> &t) const
  	{
    	vect_pts P_vect (t.size ());
    
    	for (point && PP: P_vect)
    		PP = point::Zero(); // initialize all the points to zero
    
   		for (int i = 0; i<t.size(); ++i)
   			P_vect[i] = value_fun(t[i]);
   			
    	return P_vect;
  	};		
	
	//! first derivative
	vect_Eigen 
	first_derivatives(const double & x)
	{
		return first_derivatives_fun(x);	
	};
	
	//! second derivative
	vect_Eigen 
	second_derivatives(const double & x)
	{
		return second_derivatives_fun(x);	
	};
	
	//! curvilinear abscissa
	double curvilinear_abscissa(const double & x){
	/*
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
  		*/
  	//=========== CON ADAPTIVE_QUADRATURE ==============  	
  		//lambda functions that returns the integrand function, i.e. norm(first_derivative(t))^2
  		auto abscissa_integrand = [&](double t) -> double{
			return this -> first_derivatives(t).norm();
  		};
  	
  		return integrate(abscissa_integrand,0,x);
  	
	}
	
	//! curvature
	double curvature(const double & s){
		if( (this->first_derivatives(s)).norm() == 0) return 0; // altrimenti al denominatore ho 0
		// cross() non funziona se dim == 2, vuole almeno dimensione 3
		double numerator;
		if(dim >= 3)
			numerator = 1.4;//(this->first_derivatives(s));//.cross(this->second_derivatives(s))).norm();
		else{	//dim == 2
			numerator = std::abs(this->first_derivatives(s)(0,0) * this->second_derivatives(s)(0,1) - 
						 		 this->first_derivatives(s)(0,1) * this->second_derivatives(s)(0,0));
		}
		double denominator( ((this->first_derivatives(s)).norm()) *
							((this->first_derivatives(s)).norm()) *
							((this->first_derivatives(s)).norm()) );
		return numerator/denominator;
	}
	 
	
	//! Overload of operator<<
	friend std::ostream & operator << (std::ostream & out, generic_edge<dim>& edge) {
		out << "I'm a generic edge" << std::endl;
		return out;
		/*
		out<<"Source: "<<std::endl;
		out<<edge.value(0)<<std::endl;
		out<<std::endl;
		out<<"Value in s=0.5: "<<std::endl;
		out<<edge.value(0.5)<<std::endl;
		out<<std::endl;
		out<<"Target: "<<std::endl;
		out<<edge.value(1)<<std::endl;
		out<<std::endl;
		out << "Length: " << edge.length() << std::endl;
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
		out<<"Mesh on the edge:" << std::endl;
		std::vector<point> mesh = edge.uniform_mesh(0.1);
		for(std::size_t i=0; i<mesh.size(); ++i)
			out << mesh[i] << std::endl;
		out << std::endl;
		return out;
		*/		
	}	

}; //class

} //namespace

#endif
