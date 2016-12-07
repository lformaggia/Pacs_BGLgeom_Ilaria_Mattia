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
		virtual BGLgeom::point<dim>
		operator() (double const&) const = 0;

		virtual Eigen::Matrix<double,dim,1>
		first_derivatives (double const&) = 0;

		virtual Eigen::Matrix<double,dim,1>
		second_derivatives (double const&) = 0;

		virtual double
		curvilinear_abscissa (double const&) = 0;
	
}; //class

} //namespace

#endif
