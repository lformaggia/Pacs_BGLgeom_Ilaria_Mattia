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
#include "fracture_graph_properties.hpp"

namespace Fracture{

//! The reader based on reader_ASCII to read the data about the fractures
template <typename Edge_data>
class reader_fractures : public BGLgeom::reader_ASCII	<Fracture::Vertex_data, Edge_data> {
	//inside public we have to override all the abstract method of reader_ASCII and of the constructor
	public:
		//! Constructor
		reader_fractures(std::string _filename) :	BGLgeom::reader_ASCII	<Fracture::Vertex_data,
																			 Edge_data>(_filename),
													SRC(),
													TGT(),
													K_t(),
													K_n(),
													df(),
													source_term() {};
		//! Nothing to do with this method
		void get_other_data(){};
		
		//! Reading data from line
		void get_data_from_line(){
			this->in_file >> SRC >> TGT >> K_t >> K_n >> df >> source_term;
		}
		
		//! Returning data on the edge
		Edge_data get_edge_data(){
			return Edge_data(K_t, K_n, df, source_term);
		}
		
		//! Returning data on the source
		Fracture::Vertex_data get_source_data(){
			return Fracture::Vertex_data(SRC);
		}
		
		//! Returning data on the target
		Fracture::Vertex_data get_target_data(){
			return Fracture::Vertex_data(TGT);
		}
		
		//! Nothing to do
		BGLgeom::no_topological_data get_topological_data(){
			return BGLgeom::no_topological_data();
		};
		
	private:
		//! Coordinates of source and target
		Fracture::Vertex_data::point_t SRC, TGT;
		//! Parameters
		double K_t, K_n, df, source_term;
};	//reader_fractures
												 
}	//Fracture
						 
#endif	//HH_READER_FRACTURES_HH
