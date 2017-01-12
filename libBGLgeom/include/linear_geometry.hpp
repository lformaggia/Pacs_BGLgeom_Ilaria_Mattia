/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	linear_geometry.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Concrete class for a linear geometry on an edge
*/

#ifndef HH_LINEAR_GEOMETRY_HH
#define HH_LINEAR_GEOMETRY_HH

#include <iostream>
#include <vector>
#include <cstdlib>
#include <Eigen/Dense>
#include "point.hpp"
#include "edge_geometry.hpp"
#include "mesh.hpp"

namespace BGLgeom{

/*!
	@brief	The class handling the linear geometry for an edge
	
	It is parametrized between 0 and 1
	
	@param dim Dimension of the space
*/
template <unsigned int dim>
class linear_geometry : public BGLgeom::edge_geometry<dim> {
		
	private:
		//! Coordinates of the source of the edge
		BGLgeom::point<dim> SRC;
		//! Coordinates of the target of the edge
		BGLgeom::point<dim> TGT;

	public:
		using point = BGLgeom::point<dim>;
		using vect_pts = std::vector<point>;
		using vect_double = std::vector<double>;
		
		//! Default constructor 
		linear_geometry() : SRC(), TGT() {};	
	
		//! Constructor 
		linear_geometry(point const& SRC_, point const& TGT_) : SRC(SRC_), TGT(TGT_) {};
		
		//! Copy constructor
		linear_geometry(linear_geometry const&) = default;
		
		//! Move construcotr
		linear_geometry(linear_geometry &&) = default;
		
		//! Destructor
		virtual ~linear_geometry() = default;
		
		//! Assignment operator
		linear_geometry & operator=(linear_geometry const&) = default;
		
		//! Move assignment
		linear_geometry & operator=(linear_geometry &&) = default;
		
		//! Sets the value for the source
		void
		set_source(point const& SRC_) { SRC = SRC_; }
		
		//! Sets the value for the target
		void
		set_target(point const& TGT_) {	TGT = TGT_;	}
		
		//! Getting source's coordinates
		point get_source() { return SRC; }
		point get_source() const { return SRC; }
		
		//! Getting target's coordinates
		point get_target() { return TGT; }
		point get_target() const { return TGT; }
		
		//! Computing the length of the edge
		double length() { return (TGT-SRC).norm(); }
		double length() const { return (TGT-SRC).norm(); }
	 
	    /*! 
	    	@brief	Evaluates the line at a given value of the parameter
	    	
	    	It tests if the given parameter belongs to [0,1]. If not, it gives 
	    	a warning on std::cerr and abort the program
	    */
		point
		operator() (double const& t) const {
			if(t > 1 || t < 0){
				std::cerr << "linear_geometry::operator(): parameter value out of bounds" << std::endl;
				exit(EXIT_FAILURE);
			}
			return point((TGT-SRC)*t+SRC);
		};
		
  		//! It evaluates the line in a vector of values of the parameter
  		vect_pts
  		operator() (vect_double const& t) const {
    		vect_pts P_vect(t.size());
   			for (std::size_t i = 0; i < t.size(); ++i)
   				P_vect[i] = this->operator()(t[i]); //(TGT-SRC)*t[i] + SRC;   			
   		 	return P_vect;
  		}
		
		//! Evaluates the first derivative of the line
		point 
		first_der(double const& t = 0) const { return TGT-SRC; }
		
		//! Evaluates the first derivatives in a vector of values of the parameter
		vect_pts
		first_der(vect_double const& t) const {
			vect_pts Fder(t.size());
			for (std::size_t i = 0; i < t.size(); ++i)
   				Fder[i] = this->first_der();	
   		 	return Fder;
		}
		
		//! Evaluates the second derivative of the line (of course returns zero!)
		point
		second_der(const double & t = 0) const { return point::Zero(); }
		
		//! Evaluates the second derivative of the line in a vector of parameters
		vect_pts
		second_der(vect_double const& t) const {
			return vect_pts(t.size(), point::Zero());
		}

		/*! 
			@brief Curvilinear abscissa.
			
			It tests if the given parameter belongs to [0,1]. If not, it gives 
	    	a warning on std::cerr and abort the program
	    	
			@param t Value of the parameter (between 0 and 1) where to evaluate the curvilinear abscissa
		*/
		double
		curv_abs(const double & t) const {
			if(t < 0 || t > 1){
				std::cerr << "linear_geometry::curv_abs(): parameter value out of bounds" << std::endl;
				exit(EXIT_FAILURE);
			}
			return (TGT-SRC).norm() * t;
		}
		
		//! Evaluates the cuvilinear abscissa in a vector of parameters
		vect_double
		curv_abs(vect_double const& t) const {
			vect_double C(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				C[i] = this->curv_abs(t[i]);
			return C;
		}
		
		//! Evaluates the curvature of the line (of course zero again!)
		double
		curvature(const double & x) const { return 0; }
		
		//! Evaluates the curvature of the line in a vector of parameters
		vect_double
		curvature(vect_double const& t) const {
			return vect_double(t.size(),0.0);
		}
		
		/*!
			@brief	Overload of operator<<
			
			It only tells the coordinates of its extremes. May be useful for debugging
		*/
		friend std::ostream & operator<<(std::ostream & out, linear_geometry<dim> const& edge) {
			out << "(linear)\tSource: " << edge.SRC << ", Target: " << edge.TGT;
			return out;
		}
			
}; //linear_geometry

} //BGLgeom

#endif	//HH_LINEAR_GEOMETRY_HH
