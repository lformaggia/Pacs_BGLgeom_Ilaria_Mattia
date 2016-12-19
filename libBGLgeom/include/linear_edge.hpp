/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file linear_edge.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief linear geometry for the edge
	@detail 
*/

#ifndef HH_LINEAR_EDGE_HH
#define HH_LINEAR_EDGE_HH

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <Eigen/Dense>
#include <memory>
#include "point.hpp"
#include "edge_geometry.hpp"
#include "parametric_mesh_generator.hpp"

namespace BGLgeom{

/*!
	@brief The class handling the linear geometry for an edge
	@detail It is parametrized with parameter between 0 and 1
	
	@param dim Dimension of the space
*/
template <unsigned int dim>
class linear_edge : public BGLgeom::edge_geometry<dim>, public parametric_mesh_generator {
		
	private:
		//! Coordinates of the source of the edge
		BGLgeom::point<dim> SRC;
		//! Coordinates of the starget of the edge
		BGLgeom::point<dim> TGT;
		//! A pointer to the class that will handle the parametric mesh
		//std::unique_ptr<parametric_mesh_generator> param_mesh;

	public:
		using point = BGLgeom::point<dim>;
		using vect_pts = std::vector<point>;
		using vect_double = std::vector<double>;
		
		//! Default constructor 
		linear_edge() : SRC(), TGT() {};	
	
		//! Constructor 
		linear_edge(point const& SRC_, point const& TGT_) : SRC(SRC_), TGT(TGT_) {};
		
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
	    	@brief Returns the point corresponding to that curvilinear abscissa
	    	@detail It test if the given parameter belongs to [0,1]. If not, it sets the
	    			parameter to the nearest extreme value, giving a warning on std::cerr
	    */
		point operator() (double const& t) const {
			if(t > 1 || t < 0){
				std::cerr << "linear_edge::operator(): parameter value out of bounds" << std::endl;
				if(t > 1)	//t=1
					return TGT;
				else	//t=0
					return SRC;
			}
			return point((TGT-SRC)*t+SRC);
		};
		
  		/*!
  			@brief It evaluates a vector of values for the parameter
  		*/
  		vect_pts
  		operator() (vect_double const& t) const {
    		vect_pts P_vect(t.size());
   			for (std::size_t i = 0; i < t.size(); ++i)
   				P_vect[i] = this->operator()(t[i]); //(TGT-SRC)*t[i] + SRC;   			
   		 	return P_vect;
  		}
		
		//! Evaluates the first derivative of the line
		point 
		first_der(double const& t = 0)	{ return TGT-SRC; }
		
		//! Evaluates the first derivatives in a vector of values of the parameter
		vect_pts
		first_der(vect_double const& t){
			vect_pts Fder(t.size());
			for (std::size_t i = 0; i < t.size(); ++i)
   				Fder[i] = this->first_der(); //(TGT-SRC)*t[i] + SRC;   			
   		 	return Fder;
		}
		
		//! Evaluates the second derivative of the line (of course returns zero!)
		point
		second_der(const double & t = 0) { return point::Zero(); }
		
		//! Evaluates the second derivative of the line in a vector of parameters
		vect_pts
		second_der(vect_double const& t){
			return vect_pts(t.size(), point::Zero());
		}

		/*! 
			@brief Curvilinear abscissa.
			@detail If the parameter is out of bound, the method sets it to the nearest
					extreme value and continues the computation, giving a warning on std::cerr
			@param t Value of the parameter (between 0 and 1) where to evaluate the curvilinear abscissa
		*/
		double
		curv_abs(const double & t) {
			if(t < 0 || t > 1){
				std::cerr << "linear_edge::curv_abs(): parameter value out of bounds" << std::endl;
				if(t > 1)	//x=length
					return (TGT-SRC).norm();
				else	//x=0
					return 0;;
			}
			return (TGT-SRC).norm() * t;
		}
		
		//! Evaluates the cuvilinear abscissa in a vector of parameters
		vect_double
		curv_abs(vect_double const& t){
			vect_double C(t.size());
			for(std:size_t i = 0; i < t.size(); ++i)
				C[i] = this->curv_abs(t[i]);
			return C;
		}
		
		//! Evaluates the curvature of the line (of course zero again!)
		double
		curvature(const double & x) { return 0; }
		
		//! Evaluates the curvature of the line in a vector of parameters
		vect_double
		curvature(vect_double const& t){
			return vect_double(t.size(),0.0);
		}
		
			/*	
		//! Getting the real points from the parametric mesh
		vect_pts
		evaluate_param_mesh(){
			return this->operator()(this->p_mesh);
		}
		*/
		
		/*! 
 			@brief Creating a mesh on the edge
 			@detail SRC and TGT are included in the mesh points
 			@param n Number of intervals
 		*/
 		/*
		std::vector<point>
		vect_pts
 		uniform_mesh_boh(unsigned int const& n) {
 			double h_abscissa = static_cast<double>(1/n);
 			double s = 0;
 			vect_double = param_mesh;
 			param_mesh.resize(n+1);
			vect_pts retval;
 			param_mesh.push_back(0);
 			for(std::size_t i=0; i < n-1; ++i){	//n_points-1 per non includere già qui TGT
 				s += h_abscissa;
 				retval.emplace_back(point(this->operator()(s)));
 			}
 			param_mesh.push_back(1);
 			return retval;			
 		}
 		*/
		
		//! Overload of operator<<
		friend std::ostream & operator << (std::ostream & out, linear_edge<dim> & edge) {
			out << "Hi! I'm edge from " << edge.SRC << " to " << edge.TGT << ".";
			return out;
		}
			
}; //linear_edge

} //namespace

#endif
