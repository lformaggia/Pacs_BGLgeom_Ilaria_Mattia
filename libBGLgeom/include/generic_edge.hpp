/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_edge.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Concrete class for a generic edge geometry 
	@detail
*/

#ifndef HH_GENERIC_EDGE_HH
#define HH_GENERIC_EDGE_HH

#include <functional>
#include <cmath>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "point.hpp"
#include "adaptive_quadrature.hpp"
#include <Eigen/Dense>


namespace BGLgeom{

/*!
	@brief Generic geometry for an edge
	@detail It requires as the full specification of the expression of the curve,
			of its firts derivative and of its second derivative (so they must be
			known a priori). It provides reparameterization? Between what??????
			???????????????????????????????????????????????????????????? 
	
	@param dim Dimension of the space	
*/
template<unsigned int dim>
class generic_edge {

	using point = BGLgeom::point<dim>;
	using vect_pts = std::vector<point>;
	using vect_double = std::vector<double>;

	private:
	
		std::function<point(double)> value_fun;      //! stores the function f:[0,1] ->[src,tgt] representing the edge
		std::function<point(double)> first_der_fun;   
		std::function<point(double)> second_der_fun;
		//std::function<double(double)> riparam;
		double scale_factor;
		double known_term;
		bool param_with_curv_abs;

	public:
	
		//! full constructor
		generic_edge(std::function<point(double)>  const& value_,
					 std::function<point(double)> const& first_der_,
					 std::function<point(double)> second_der_) :
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

	    //! Evaluation of the curve ina given value of the parameter     
		point operator() (double const& s) const
		{	
			if(param_with_curv_abs)
				if(s < 0 || s > this->length())
					std::cerr << "generic_edge::operator(): parameter value out of bounds" << std::endl;
			//check if param belongs to 0->1
			return value_fun(scale_factor * s + known_term);
		}
		
		//! Evaluation in a vector of parameters
	  	vect_pts
	  	operator() (const std::vector<double> &t) const	{
	    	vect_pts Pts(t.size());
	   		for(std::size_t i = 0; i < t.size(); ++i)
	   			Pts[i] = this->operator()(t[i]);	   			
	    	return Pts;
	  	}		
		
		//! Evaluation of the first derivative in a given value of the parameter
		point
		first_der(const double & t)	{ return first_der_fun(t); }
		
		//! Evaluation in a vector of parameters
		vect_pts
		first_der(const vect_double & t){
			vect_pts Fder(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				Fder[i] = this->first_der(t[i]);
			return Fder;
		}
		
		//! Evaluation of the second derivative in a given value of the parameter
		point 
		second_der(const double & t) { return second_der_fun(t); }
		
		//! Evaluation ina vector of parameters
		vect_pts
		second_der(vect_double const& t){
			vect_pts Sder(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				Sder[i] = this->second_der(t[i]);
			return Sder;
		}
		
		//! Evaluation of the curvilinear abscissa
		double curv_abs(const double & t){
			//lambda functions that returns the integrand function, i.e. norm(first_derivative(t))
	  		auto abscissa_integrand = [&](double t) -> double{
				return this -> first_der(t).norm();
	  		};	  	
	  		return BGLgeom::integrate(abscissa_integrand,0,t);
		}
		
		//! Evaluation in a vector of parameters
		vect_double
		curv_abs(vect_double const& t){
			vect_double CA(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				CA[i] = this->curv_abs(t[i]);
			return CA;
		}
		
		//! Evaluation of the curvature
		double curvature(const double & s){
			if( (this->first_der(s)).norm() == 0)
				return 0; // altrimenti al denominatore ho 0
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
		
		//! Evaluation in a vector of parameters
		vect_double
		curvature(vect_double const& t){
			vect_double C(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				C[i] = this->curvature(t[i]);
			return C;
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
