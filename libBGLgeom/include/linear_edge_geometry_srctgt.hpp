/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file linear_edge_geometry_SRCTGT.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief linear geometry for the edge
	@detail 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_HH
#define HH_GENERIC_EDGE_GEOMETRY_HH

#include<array>
#include<functional>
#include<Eigen/Dense>
#include"point.hpp"

namespace BGLgeom{

/*!
	@brief The class handling the linear geometry for an edge
	@detail
	
	@param dim Dimension of the space
*/
template<unsigned int dim>
class linear_edge_geometry_srctgt : public edge_geometry<dim> {
	private:
		//! Coordinates of the source of the edge
		BGLgeom::point<dim> SRC;
		//! Coordinates of the starget of the edge
		BGLgeom::point<dim> TGT;

	public:
	
		//! Constructor 
		linear_edge_geometry_srctgt
		(BGLgeom::point<dim> SRC_, BGLgeom::point<dim> TGT_) : SRC(SRC_), TGT(TGT_){};
		
		//! Sets the value for the source
		void set_source(BGLgeom::point<dim> SRC_) { SRC = SRC_; }
		
		//! Sets the value for the target
		void set_target(BGLgeom::point<dim> TGT_) {	TGT = TGT_;	}
		
		//! Getting source's coordinates
		BGLgeom::point<dim> get_source() { return SRC; }
		BGLgeom::point<dim> get_source() const { return SRC; }
		
		//! Getting target's coordinates
		BGLgeom::point<dim> get_target() { return TGT; }
		BGLgeom::point<dim> get_target() const { return TGT; }
	 
	    //! returns the point corresponding
		BGLgeom::point<dim> value (const double & x)
		{
			return BGLgeom::point<dim>((TGT-SRC)*x+SRC); // copy-constructor: we copy in P the values of the line in correspondence of the indicated parameter
		};
		
		//! first derivative
		Eigen::Matrix<double,dim,1> 
		first_derivatives(const double & x = 0)
		{
			return TGT-SRC;	
		}	
		
		
		//! second derivative
		Eigen::Matrix<double,dim,1> 					//! the second derivative is null along all the components
		second_derivatives(const double & x = 0)
		{
			Eigen::Matrix<double,dim,1> v;
			v.fill(0.0);	
			return v;	
		}
		

		//! curvilinear abscissa
		double curvilinear_abscissa(const double & parameter)  // qui richiedo strettamente un parametro tra 0 e 1
		{
			return (TGT-SRC).norm()*parameter;
		};
		
		//! curvature
		double curvature(const double & s){
			return 0;
		}
		
		//! Overload of operator<<
		friend std::ostream & operator << (std::ostream & out, linear_edge_geometry_srctgt<dim> & edge) {
			out<<"Source: "<<std::endl;
			out<<edge.value(0)<<std::endl;
			out<<std::endl;
			out<<"Value in s=0.5: "<<std::endl;
			out<<edge.value(0.5)<<std::endl;
			out<<std::endl;
			out<<"Target: "<<std::endl;
			out<<edge.value(1)<<std::endl;
			out<<std::endl;
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
		}
	
}; //class


} //namespace






#endif
