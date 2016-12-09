/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file reader.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Class to read ASCII file for fractures application

	@detail 
*/

#ifndef HH_READER_FRACTURES_HH
#define HH_READER_FRACTURES_HH

#include "reader_ASCII.hpp"
#include "data_structure.hpp"
#include "linear_edge.hpp"

namespace Fracture{

/*
	First of all, we define the data structures to be attached to vertices and edges
	on the graph.
	The Vertex_base_property fits the input data, so it is not needed to define a new one.
	This is not the case for the edge properties, where we have to insert three new numerical
	parameters: K_t, K_n, df.
*/
//! The edge data structure
struct Edge_data : public BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2>{
	//! Parameter K_t
	double K_t;
	//! Parameter K_n
	double K_n;
	//! Degrees of freedom (??)
	double df;
	
	//! Default constructor
	Edge_data() :	BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2>(),
					K_t(),
					K_n(),
					df() {};
					
	//! Constructor
	Edge_data(double const& _K_t, double const& _K_n, double const& _df) :
				BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2>(),
				K_t(_K_t),
				K_n(_K_n),
				df(_df) {};
};	//Edge_data



//! The reader based reader_ASCII to read the data about the fractures
template <typename Edge_data>
class reader_fractures : public BGLgeom::reader_ASCII	<BGLgeom::Vertex_base_property<2>, Edge_data> {
	//inside public we have to override all the abstract method of reader_ASCII and of the constructor
	public:
		//! Constructor
		reader_fractures(std::string _filename) :	BGLgeom::reader_ASCII	<BGLgeom::Vertex_base_property<2>,
																			 Edge_data>(_filename),
													K_t(),
													K_n(),
													df(),
													discard() {};
		//! Nothing to do with this method
		void get_other_data(){};
		
		//! Reading data from line
		void get_data_from_line(){
			this->in_file >> SRC >> TGT >> K_t >> K_n >> df >> discard;
		}
		
		//! Returning data on the edge
		Edge_data get_edge_data(){
			return Edge_data(K_t, K_n, df);
		}
		
		//! Returning data on the source
		BGLgeom::Vertex_base_property<2> get_source_data(){
			return BGLgeom::Vertex_base_property<2>(SRC);
		}
		
		//! Returning data on the target
		BGLgeom::Vertex_base_property<2> get_target_data(){
			return BGLgeom::Vertex_base_property<2>(TGT);
		}
		
		//! Nothing to do
		BGLgeom::no_topological_data get_topological_data(){};
		
	private:
		//! Coordinates of source and target
		BGLgeom::Vertex_base_property<2>::point_t SRC,TGT;
		//! Parameters
		double K_t, K_n, df;
		//! Discarding last value of the row
		int discard;
};	//reader_fractures
												 
}	//Fracture
						 
#endif	//HH_READER_FRACTURES_HH
