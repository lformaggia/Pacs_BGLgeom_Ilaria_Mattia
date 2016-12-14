/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file edge_geometry.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Virtual base class for the geometry of an edge
	@detail 
*/

#ifndef HH_EDGE_GEOMETRY_HH
#define HH_EDGE_GEOMETRY_HH

#include<array>
#include<Eigen/Dense>
#include"point.hpp"

namespace BGLgeom{

template<unsigned int dim>
class
edge_geometry{
	public:
		/*!
			@brief Overload of the calling operator
			@detail This represents the evaluation of the curve at a given value of 
					the parameter. The evaluation produces a point in the dim-dimensional
					space.
		*/
		virtual BGLgeom::point<dim>
		operator() (double const&) const = 0;
		
		/*!
			@brief First derivative of the curve
			@detail It evaluates each component of the first derivative of the curve
					at a given value of the parameter. It returns an Eigen vector 
					which contains the three component of the evaluation
		*/
		virtual Eigen::Matrix<double,1,dim>
		first_der (double const&) = 0;
		
		/*!
			@brief Second derivative of the curve
			@detail It evaluates each component of the second derivative of the curve
					at a given value of the parameter. It returns an Eigen vector 
					which contains the three component of the evaluation
		*/
		virtual Eigen::Matrix<double,1,dim>
		second_der (double const&) = 0;
		
		/*!
			@brief Curvilinear abscissa of the curve
			@detail It evaluates the curvilinear abscissa of the curve at a given
					value of the parameter
		*/
		virtual double
		curv_abs (double const&) = 0;
		
		/*!
			@brief Curvature of the curve
			@detail It evaluates the curvature at a given value of the parameter
		*/
		virtual double
		curvature (double const&) = 0;
	
}; //class

} //namespace

#endif
