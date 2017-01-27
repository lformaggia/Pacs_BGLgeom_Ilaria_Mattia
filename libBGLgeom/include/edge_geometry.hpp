/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file	edge_geometry.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Abstract base class for the geometry of an edge
*/

#ifndef HH_EDGE_GEOMETRY_HH
#define HH_EDGE_GEOMETRY_HH

#include <vector>
#include <functional>
#include <Eigen/Dense>
#include "point.hpp"

namespace BGLgeom{

/*!
	@brief	Abstract class for an edge
	
	It specifies all the geometric operation that should be carried
	out on an edge, thought as a curve in the space (2 or 3 dimensional). 
	In particular, it defines, for a given value of the parameter: \n
	- evaluation of the curve; \n
	- evaluation of the first derivative; \n
	- evaluation of the second derivative; \n
	- evaluation of the curvature; \n
	- evaluation of the curvilinear ascissa. \n
	It provides also evaluation of this characteristics for a single value
	or for a vector of values of the parameter
	@param dim The dimension of the space
*/
template <unsigned int dim>
class edge_geometry {
	using point = BGLgeom::point<dim>;
	using vect_pts = std::vector<point>;
	using vect_double = std::vector<double>;

	public:
		/*!
			@brief Overload of the calling operator
			
			This represents the evaluation of the curve at a given value of 
			the parameter
			@return A point in the dim-dimensional space.
		*/
		virtual BGLgeom::point<dim>
		operator() (double const&) const = 0;
		
		//! The same as before, but with evaluation on a vector of parameters
		virtual std::vector<BGLgeom::point<dim>>
		operator() (std::vector<double> const&) const = 0;
		
		/*!
			@brief First derivative of the curve
			
			It evaluates each component of the first derivative of the curve
			at a given value of the parameter
			@return An Eigen matrix containing the three components of the evaluation.
					We use again as return value a BGLgeom:point<dim>, since the 
					underlying container is the same
		*/
		virtual BGLgeom::point<dim>
		first_der (double const&) const = 0;
		
		//! The same as before, but with evaluation on a vector of parameters
		virtual std::vector<BGLgeom::point<dim>>
		first_der (std::vector<double> const&) const = 0;
		
		/*!
			@brief Second derivative of the curve
			
			It evaluates each component of the second derivative of the curve
			at a given value of the parameter
			@return An Eigen matrix containing the three components of the evaluation.
					We use again as return value a BGLgeom:point<dim>, since the 
					underlying container is the same
		*/
		virtual BGLgeom::point<dim>
		second_der (double const&) const = 0;
		
		//! The same as before, but with evaluation on a vector of parameters
		virtual std::vector<BGLgeom::point<dim>>
		second_der (std::vector<double> const&) const = 0;
		
		/*!
			@brief Curvilinear abscissa of the curve
			
			It evaluates the curvilinear abscissa of the curve at a given
			value of the parameter
		*/
		virtual double
		curv_abs (double const&) const = 0;
		
		//! The same as before, but with evaluation on a vector of parameters
		virtual std::vector<double>
		curv_abs (std::vector<double> const&) const = 0;
		
		/*!
			@brief Curvature of the curve
			
			It evaluates the curvature at a given value of the parameter
		*/
		virtual double
		curvature (double const&) const = 0;
		
		//! The same as before, but with evaluation on a vector of parameters
		virtual std::vector<double>
		curvature (std::vector<double> const&) const = 0;
}; //edge_geometry

} //namespace

#endif
