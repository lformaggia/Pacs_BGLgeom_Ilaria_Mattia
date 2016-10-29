/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_edge_geometry_srctgt.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief edge geometry source&target dependent
	@detail 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_SRCTGT_HH
#define HH_GENERIC_EDGE_GEOMETRY_SRCTGT_HH

#include<array>
#include<functional>
#include"generic_point.hpp"
#include"edge_geometry.hpp"

namespace BGLgeom{

template<unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class
generic_edge_geometry_srctgt: public BGLgeom::edge_geometry<dim>
{
	private:
	
	std::function<BGLgeom::point<dim>(double)> value_fun;      //! stores the function x_i(s) = f_i(s), i=1:dim, s=0:1, f: [0,1] -> [0,1]
	BGLgeom::point<dim> src; //edge_source
	BGLgeom::point<dim> tgt; //edge_target
	

	public:
	
	//! full constructor
	generic_edge_geometry_srctgt
	(std::function<BGLgeom::point<dim>(double)> value_, BGLgeom::point<dim> src_, BGLgeom::point<dim> tgt_): value_fun(value_), src(src_), tgt(tgt_)
	{};
	
	//! default constructor: linear edge between 0-point (point with all components equal to 0) and 1-point(point with all components equal to 1)
	generic_edge_geometry_srctgt()
	{
		value_fun = [](double s) -> BGLgeom::point<dim> 
					{std::array<double,dim> coordinates;
					 coordinates.fill(s); //x(s)=s; y(s)=s; ...
					 BGLgeom::point<dim> p(coordinates);
					 return p;
					};
	};	
	
	//! first derivative
	virtual std::array<double,dim> 
	first_derivatives(const double x)
	{
		//reads data from a data file
		//GetPot   ifl("data.pot");
		//double h = ifl("h", 0.001);
		const double h = 0.001;
		std::cout<<"Spacing "<<h<<std::endl;


		double constexpr half(0.5);
		
		BGLgeom::point<dim> diff; //! declare the point that will contain the result
		
		// Compute finite difference depending on the value x +_ h
		if(x+h > 1)
			diff = (this->value(x) - this->value(x-h))/h;

		else if(x-h < 0)
			diff = (this->value(x+h) - this->value(x))/h;

		else 
			diff = half*(this->value(x+h)-this->value(x-h))/h;
		
		// exctract the array of the coordinates from the point
		std::array<double,dim> dn(diff.coordinates());
		
		return dn;	
	}		
	
	//! second derivative
	virtual	std::array<double,dim> 
	second_derivatives(const double x)
	{
		//reads data from a data file
		//GetPot   ifl("data.pot");
		//double h = ifl("h", 0.05);
		const double h = 0.001;
		std::cout<<"Spacing "<<h<<std::endl;
		
		BGLgeom::point<dim> diff;
			
		// Compute finite difference depending on the value x +_ h
		if(x+h > 1) //bacward
			diff = (this->value(x) - 2*this->value(x-h) + this->value(x-2*h))/(h*h);

		else if(x-h < 0) //forward
			diff = (this->value(x+2*h) - 2*this->value(x+h) + this->value(x))/(h*h);

		else //central
			diff = (this->value(x+h) - 2*this->value(x) + this->value(x-h))/(h*h);
			
		// exctract the array of the coordinates from the point
		std::array<double,dim> dn(diff.coordinates());

		return dn;		
	}
	

	//! curvilinear abscissa
	
	//! Sets the right value for the source (when initialized it has a dummy value, becuase at that point we don't have information about edge descriptor)
	void set_source(BGLgeom::point<dim> src_){
		src = src_; 
	}
	
	void set_target(BGLgeom::point<dim> tgt_){
		tgt = tgt_;
	}
 
    //! returns the point corresponding to s=0:1 
	virtual BGLgeom::point<dim> value (const double parameter)
	{
		//check if param belongs to 0->1
		return (tgt - src)*value_fun(parameter) + src;
	};
	
	
	//! Overload of operator<<
	friend std::ostream & operator << (std::ostream & out, generic_edge_geometry_srctgt<dim>& edge) {
		out<<"Source: "<< edge.src<<std::endl;
		out<<"Target: "<< edge.tgt<<std::endl;
		out<<"Value in s=0: "<<edge.value(0)<<std::endl;
		out<<"Value in s=0.5: "<<edge.value(0.5)<<std::endl;
		out<<"Value in s=1: "<<edge.value(1)<<std::endl;
		out<<"First derivatives in s=0: ";
		for(const int&& i : edge.first_derivatives(0))
			out<<i<<" ";
		out<<std::endl;
		out<<"First derivatives in s=0.5: ";
		for(const int&& i : edge.first_derivatives(0.5))
			out<<i<<" ";
		out<<std::endl;
		out<<"First derivatives in s=1: ";
		for(const int&& i : edge.first_derivatives(1))
			out<<i<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=0: ";
		for(const int&& i : edge.second_derivatives(0))
			out<<i<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=0.5: ";
		for(const int&& i : edge.second_derivatives(0.5))
			out<<i<<" ";
		out<<std::endl;		
		out<<"Second derivatives in s=1: ";
		for(const int&& i : edge.second_derivatives(1))
			out<<i<<" ";
		out<<std::endl;			
		//out<<"Curvilinear abscissa in s=0: "<<edge.curvilinear_abscissa(0)<<std::endl;
		//out<<"Curvilinear abscissa in s=0.5: "<<edge.curvilinear_abscissa(0.5)<<std::endl;
		//out<<"Curvilinear abscissa in s=1: "<<edge.curvilinear_abscissa(1)<<std::endl;		
	}
	
}; //class


} //namespace






#endif
