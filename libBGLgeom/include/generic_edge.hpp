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

#include <array>
#include <functional>
#include <cmath>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "point.hpp"
//#include"mesh.hpp"
//#include"numerical_integration.hpp"
//#include"numerical_rule.hpp"
#include "adaptive_quadrature.hpp"
#include <Eigen/Dense>


namespace BGLgeom{

/*!
	@brief Generic geometry for an edge
	@detail
	
	@param dim Dimension of the space	
*/
template<unsigned int dim>
class generic_edge {

	using vect_Eigen = Eigen::Matrix<double,1,dim>;
	using point = BGLgeom::point<dim>;
	using vect_pts = std::vector<point>;

	private:
	
	std::function<point(double)> value_fun;      //! stores the function f:[0,1] ->[src,tgt] representing the edge
	std::function<vect_Eigen(double)> first_der_fun;   
	std::function<vect_Eigen(double)> second_der_fun;
	//std::function<double(double)> riparam;
	double scale_factor;
	double known_term;
	bool param_with_curv_abs;

	public:
	
	//! full constructor
	generic_edge(std::function<point(double)>  const& value_,
				 std::function<vect_Eigen(double)> const& first_der_,
				 std::function<vect_Eigen(double)> second_der_) :
				 			 value_fun(value_),
				 			 first_der_fun(first_der_),
				 			 second_der_fun(second_der_),
				 			 //riparam(),
				 			 scale_factor(1),
				 			 known_term(0),
				 			 param_with_curv_abs(false) {};
				 			 
	//! Length of the curve
	double length() const {
		return 100;		//I need to store inside the class t0 and tf to compute this...
	}
	
	/*!
		@brief Method to riparametrize the curve w.r.t the curvilinear abscissa
		@detail 
	*/
	void riparam_to_curv_abs(double const& t0, double const& tf){
		scale_factor = (tf - t0)/this->curv_abs(tf);
		known_term = t0;
		param_with_curv_abs = true;
	}
	
	/*!
		#brief Method to riparametrize the curve form curvilinear abscissa
				to original parameter interval
	*/
	void riparam_original(){
		if(param_with_curv_abs){
			scale_factor = 1;
			known_term = 0;
			param_with_curv_abs = false;
		} else {
			std::cerr << "Error, generic_edge::riparam_original()" << std::endl;
			std::cerr << "This curve is not parametrized w.r.t curvilinear abscissa" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

    //! returns the point corresponding to s=0:1     
	point operator() (double const& s) const
	{	
		if(param_with_curv_abs)
			if(s < 0 || s > this->length())
				std::cerr << "generic_edge::operator(): parameter value out of bounds" << std::endl;
		//check if param belongs to 0->1
		return value_fun(scale_factor * s + known_term);
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
	first_der(const double & x)
	{
		return first_der_fun(x);	
	};
	
	//! second derivative
	vect_Eigen 
	second_der(const double & x)
	{
		return second_der_fun(x);	
	};
	
	//! curvilinear abscissa
	double curv_abs(const double & x){
	/*
  		int nint=1000; // quanto mettiamo il default? Lo leggiamo da GetPot?
  		Geometry::Domain1D domain(0,x);  //estremi di integrazione
  		Geometry::Mesh1D mesh(domain,nint); //definito in mesh.hpp
  		
  		//lambda functions that returns the integrand function, i.e. norm(first_derivative(t))^2
  		auto abscissa_integrand = [&](double t) -> double{
			this -> first_der(t).norm();
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
			return this -> first_der(t).norm();
  		};
  	
  		return BGLgeom::integrate(abscissa_integrand,0,x);
  	
	}
	
	//! curvature
	double curvature(const double & s){
		if( (this->first_der(s)).norm() == 0) return 0; // altrimenti al denominatore ho 0
		// cross() non funziona se dim == 2, vuole almeno dimensione 3
		double numerator;
		if(dim == 3){
			// explicit computation of the determinant
			Eigen::Matrix<double,1,3> tmp( (this->first_der(s)(0,0) * this->second_der(s)(0,1) -
						 					this->first_der(s)(0,1) * this->second_der(s)(0,0)),
						 				   (this->first_der(s)(0,2) * this->second_der(s)(0,0) -
						 					this->first_der(s)(0,0) * this->second_der(s)(0,2)),
						 				   (this->first_der(s)(0,1) * this->second_der(s)(0,2) -
						 					this->first_der(s)(0,2) * this->second_der(s)(0,1))   );
			numerator = tmp.norm();
		} else {	//dim == 2
			numerator = std::abs(this->first_der(s)(0,0) * this->second_der(s)(0,1) - 
						 		 this->first_der(s)(0,1) * this->second_der(s)(0,0));
		}
		double denominator( ((this->first_der(s)).norm()) *
							((this->first_der(s)).norm()) *
							((this->first_der(s)).norm()) );
		return numerator/denominator;
	}
	 
	
	//! Overload of operator<<
	friend std::ostream & operator << (std::ostream & out, generic_edge<dim>& edge) {
		out << "I'm a generic edge";
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
			out<<(edge.first_der(0))[i]<<" ";
		out<<std::endl;
		out<<"First derivatives in s=0.5: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.first_der(0.5))[i]<<" ";
		out<<std::endl;
		out<<"First derivatives in s=1: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.first_der(1))[i]<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=0: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.second_der(0))[i]<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=0.5: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.second_der(0.5))[i]<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=1: ";
		for(int i=0; i<dim; ++i)
			out<<(edge.second_der(1))[i]<<" ";
		out<<std::endl;			
		out<<"Curvilinear abscissa in s=0: "<<edge.curv_abs(0)<<std::endl;
		out<<"Curvilinear abscissa in s=0.5: "<<edge.curv_abs(0.5)<<std::endl;
		out<<"Curvilinear abscissa in s=1: "<<edge.curv_abs(1)<<std::endl;
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
