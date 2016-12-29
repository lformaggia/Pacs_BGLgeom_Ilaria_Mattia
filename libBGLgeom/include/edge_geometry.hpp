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

#include <vector>
#include <functional>
#include <Eigen/Dense>
#include "point.hpp"
#include "domain.hpp"

namespace BGLgeom{



/*!
	@brief Abstract class for an edge
	@detail	It specifies all the geometric operation that should be carried
			out on the edge, thougth as a curve in the space (2 or 3 dimensional).
			It provide also evaluation of this characteristics for a single value
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
			@detail This represents the evaluation of the curve at a given value of 
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
			@detail It evaluates each component of the first derivative of the curve
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
			@detail It evaluates each component of the second derivative of the curve
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
			@detail It evaluates the curvilinear abscissa of the curve at a given
					value of the parameter
		*/
		virtual double
		curv_abs (double const&) const = 0;
		
		//! The same as before, but with evaluation on a vector of parameters
		virtual std::vector<double>
		curv_abs (std::vector<double> const&) const = 0;
		
		/*!
			@brief Curvature of the curve
			@detail It evaluates the curvature at a given value of the parameter
		*/
		virtual double
		curvature (double const&) const = 0;
		
		//! The same as before, but with evaluation on a vector of parameters
		virtual std::vector<double>
		curvature (std::vector<double> const&) const = 0;
		
		/*
		virtual
		std::pair<vect_pts,std::vector<double>>
		uniform_mesh(unsigned int const& n) {
			std::vector<double> parametric_mesh;
			vect_pts real_mesh;
			BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n);
			parametric_mesh = temp_mesh.getMesh();
			real_mesh = this->operator()(parametric_mesh);
			return std::make_pair(real_mesh, parametric_mesh);
		}
		
		virtual
		std::pair<vect_pts,std::vector<double>>
		variable_mesh(unsigned int const& n, std::function<double(double)> const& spacing_function){
			vect_pts real_mesh;
			vect_double parametric_mesh;
			BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n, spacing_function);
			parametric_mesh = temp_mesh.getMesh();
			real_mesh = this->operator()(parametric_mesh);
			return std::make_pair(real_mesh, parametric_mesh);
		}
		*/
}; //edge_geometry

} //namespace

#endif
