/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	generic_geometry.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Concrete class for a generic edge geometry 
*/

#ifndef HH_GENERIC_GEOMETRY_HH
#define HH_GENERIC_GEOMETRY_HH

#include <functional>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "point.hpp"
#include "adaptive_quadrature.hpp"
#include "edge_geometry.hpp"
#include <Eigen/Dense>

//! The tolerance on being zero
#define tol_dist 1e-8

namespace BGLgeom{

/*!
	@brief	Generic geometry for an edge
	@detail To be constructed, it requires the full specification of the expression of the 
			curve, of its firts derivative and of its second derivative (so they must be
			known a priori) They also must be parametrized between 0 and 1.
			Each evaluation method checks if the given parameter is in this range, 
			otherwise the program abort
	
	@param dim Dimension of the space	
*/
template<unsigned int dim>
class generic_geometry : public BGLgeom::edge_geometry<dim> {

	using point = BGLgeom::point<dim>;
	using vect_pts = std::vector<point>;
	using vect_double = std::vector<double>;

	private:
		//! The analytic expression of the parameterization of the curve
		std::function<point(double)> value_fun;
		//! The analytic expression of the first derivative of the curve
		std::function<point(double)> first_der_fun;
		//! The analytic expression of the second derivative of the curve
		std::function<point(double)> second_der_fun;
		
	public:
	
		//! Default constructor
		generic_geometry() : value_fun(), first_der_fun(), second_der_fun() {};
	
		//! Full constructor
		generic_geometry(std::function<point(double)> const& value_,
						 std::function<point(double)> const& first_der_,
						 std::function<point(double)> const& second_der_) :
					 			 value_fun(value_),
					 			 first_der_fun(first_der_),
					 			 second_der_fun(second_der_) {};
					 			 
		//! Copy constructor
		generic_geometry(generic_geometry const&) = default;
		
		//! Move constructor
		generic_geometry(generic_geometry &&) = default;
		
		//! Destructor
		virtual ~generic_geometry() = default;
		
		//! Assignment operator
		generic_geometry & operator=(generic_geometry const&) = default;
		
		//! Move assignment
		generic_geometry & operator=(generic_geometry &&) = default;
		
		/*!
			@defgroup Setting methods
			@{		
		*/
		void
		set_function(std::function<point(double)> const& _value_fun){
			value_fun = _value_fun;
		}
			
		void
		set_first_der(std::function<point(double)> const& _first_der_fun){
			first_der_fun = _first_der_fun;
		}
			
		void
		set_second_der(std::function<point(double)> const& _second_der_fun){
			second_der_fun = _second_der_fun;
		}
		
		void
		set_all(std::function<point(double)> const& _value_fun,
				std::function<point(double)> const& _first_der_fun,
				std::function<point(double)> const& _second_der_fun){
			value_fun = _value_fun;
			first_der_fun = _first_der_fun;
			second_der_fun = _second_der_fun;				 
		}
		/*! @} */
		
		//! Length of the curve
		double length() { return this->curv_abs(1); }
		double length() const { return this->curv_abs(1); }
		
		//! Evaluation of the curve in a given value of the parameter
		point
		operator()(double const& t) const {
			if(t < 0 || t > 1){
				std::cerr << "generic_geometry::operator(): parameter value out of bounds" << std::endl;
				exit(EXIT_FAILURE);
			}
			return value_fun(t);	
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
		first_der(const double & t) const { 
			if(t < 0 || t > 1){
				std::cerr << "generic_geometry::first_der(): parameter value out of bounds" << std::endl;
				exit(EXIT_FAILURE);
			}
			return first_der_fun(t);
		}
		
		//! Evaluation in a vector of parameters
		vect_pts
		first_der(const vect_double & t) const {
			vect_pts Fder(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				Fder[i] = this->first_der(t[i]);
			return Fder;
		}
		
		//! Evaluation of the second derivative in a given value of the parameter
		point 
		second_der(const double & t) const {
			if(t < 0 || t > 1){
				std::cerr << "generic_geometry::second_der(): parameter value out of bounds" << std::endl;
				exit(EXIT_FAILURE);
			}
			return second_der_fun(t);
		}
		
		//! Evaluation ina vector of parameters
		vect_pts
		second_der(vect_double const& t) const {
			vect_pts Sder(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				Sder[i] = this->second_der(t[i]);
			return Sder;
		}
		
		//! Evaluation of the curvilinear abscissa
		double curv_abs(const double & t) const {
			if(t < 0 || t > 1){
				std::cerr << "generic_geometry::curv_abs(): parameter value out of bounds" << std::endl;
				exit(EXIT_FAILURE);
			}	
			//lambda functions that returns the integrand function, i.e. norm(first_derivative(t))
	  		auto abscissa_integrand = [&](double t) -> double{
				return this -> first_der(t).norm();
	  		};
	  		double retval = BGLgeom::integrate(abscissa_integrand,0,t);
	  		return retval;	  		
		}
		
		//! Evaluation in a vector of parameters
		vect_double
		curv_abs(vect_double const& t) const {
			vect_double CA(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				CA[i] = this->curv_abs(t[i]);
			return CA;
		}
		
		//! Evaluation of the curvature
		double curvature(const double & t) const {
			if(t < 0 || t > 1){
				std::cerr << "generic_geometry::curvature(): parameter value out of bounds" << std::endl;
				exit(EXIT_FAILURE);
			}
			if( (this->first_der(t)).norm() < tol_dist )
				return 0; // otherwise at the denominator I will have zero or very close to it
			double numerator;
			if(dim == 3){
				// explicit computation of the determinant
				Eigen::Matrix<double,1,3> tmp( (this->first_der(t)(0,0) * this->second_der(t)(0,1) -
							 					this->first_der(t)(0,1) * this->second_der(t)(0,0)),
							 				   (this->first_der(t)(0,2) * this->second_der(t)(0,0) -
							 					this->first_der(t)(0,0) * this->second_der(t)(0,2)),
							 				   (this->first_der(t)(0,1) * this->second_der(t)(0,2) -
							 					this->first_der(t)(0,2) * this->second_der(t)(0,1))   );
				numerator = tmp.norm();
			} else {	//dim == 2
				numerator = std::abs(this->first_der(t)(0,0) * this->second_der(t)(0,1) - 
							 		 this->first_der(t)(0,1) * this->second_der(t)(0,0));
			}
			double denominator( ((this->first_der(t)).norm()) *
								((this->first_der(t)).norm()) *
								((this->first_der(t)).norm()) );
			return numerator/denominator;
		}
		
		//! Evaluation in a vector of parameters
		vect_double
		curvature(vect_double const& t) const {
			vect_double C(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				C[i] = this->curvature(t[i]);
			return C;
		}
		
		/*!
			@brief	Overload of operator<<
			@detail It only tells the coordinates of its extremes. May be useful for debugging
		*/
		friend std::ostream & operator<<(std::ostream & out, generic_geometry<dim> const& edge) {
			out << "(generic)\tSource: " << edge(0) << ", Target: " << edge(1);
			return out;
		}	

}; //generic_geometry

} //BGLgeom

#endif	//HH_GENERIC_GEOMETRY_HH
