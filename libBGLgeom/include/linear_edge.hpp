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
#include "point.hpp"
#include "edge_geometry.hpp"

namespace BGLgeom{

/*!
	@brief The class handling the linear geometry for an edge
	@detail It is parametrized using the curvilinear abscissa
	
	@param dim Dimension of the space
*/
template <unsigned int dim>
class linear_edge : public BGLgeom::edge_geometry<dim> {

	using vect_Eigen = Eigen::Matrix<double,dim,1>;
	using point = BGLgeom::point<dim>;
	using vect_pts = std::vector<point>;

	private:
		//! Coordinates of the source of the edge
		point SRC;
		//! Coordinates of the starget of the edge
		point TGT;

	public:
		//! Default constructor 
		linear_edge() : SRC(), TGT(){};	
	
		//! Constructor 
		linear_edge(point SRC_, point TGT_) : SRC(SRC_), TGT(TGT_){};
		
		//! Sets the value for the source
		void
		set_source(point SRC_) { SRC = SRC_; }
		
		//! Sets the value for the target
		void
		set_target(point TGT_) {	TGT = TGT_;	}
		
		//! Getting source's coordinates
		point	get_source() { return SRC; }
		point	get_source() const { return SRC; }
		
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
		point operator() (double t) const{
			if(t > 1 || t < 0){
				std::cerr << "linear_edge::value(): parameter value out of bounds" << std::endl;
				if(t > 1)	//t=1
					return TGT;
				else	//t=0
					return SRC;
			}
			return point((TGT-SRC)*t+SRC); // copy-constructor: we copy in P the values of the line in correspondence of the indicated parameter
		};
		
  		
  		vect_pts
  		operator() (const std::vector<double> &t) const
  		{
    		vect_pts P_vect (t.size ());
    
    		for (point && PP: P_vect)
    			PP = point::Zero(); // initialize all the points to zero
    
   			for (int i = 0; i<t.size(); ++i)
   				P_vect[i] = (TGT-SRC)*t[i]+SRC;
   			
   		 	return P_vect;
  		};
		
		//! first derivative
		vect_Eigen 
		first_derivatives(const double & x = 0)	{ return TGT-SRC; }		
		
		//! second derivative
		vect_Eigen 					//! the second derivative is null along all the components
		second_derivatives(const double & x = 0) {
			return vect_Eigen::Zero();	
		}		

		/*! 
			@brief Curvilinear abscissa. We require a parameter between 0 and 1
			@detail If the parameter is out of bound, the method sets it to the nearest
					extreme value and continues the computation, giving a warning on std::cerr
		*/
		double
		curvilinear_abscissa(const double & x) {
			if(x > 1 || x < 0){
				std::cerr << "linear_edge::curvilinear_abscissa(): parameter value out of bounds" << std::endl;
				if(x > 1)	//x=1
					return (TGT-SRC).norm();
				else	//x=0
					return 0;;
			}
			return (TGT-SRC).norm() * x;
		};
		
		//! curvature
		double
		curvature(const double & x) { return 0; }
		
		/*! 
			@brief Creating a mesh on the edge
			@detail SRC and TGT are included in the mesh points
			@param h Spacing between the points of the mesh (uniform mesh) in terms of spatial length.
		*/
		std::vector<point>
		uniform_mesh(double const& h = 0.01) {
			unsigned int n_points = std::ceil(this->length()/h);
			double h_abscissa = 1./n_points;
			double s = 0;
			std::vector<point> retval;
			retval.push_back(SRC);
			for(std::size_t i=0; i < n_points-1; ++i){	//n_points-1 per non includere già qui TGT
				s += h_abscissa;
				retval.emplace_back(point(this->value(s)));
			}
			retval.push_back(TGT);
			return retval;			
		}
		
		//! Overload of operator<<
		friend std::ostream & operator << (std::ostream & out, linear_edge<dim> & edge) {
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
		}
	
}; //linear_edge

} //namespace

#endif
