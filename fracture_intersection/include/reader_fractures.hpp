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
	@date Dec, 2016
	@brief Class to read ASCII file for fractures application
*/

#ifndef HH_READER_FRACTURES_HH
#define HH_READER_FRACTURES_HH

#include "reader_ASCII.hpp"
#include "fracture_graph_properties.hpp"

namespace Fracture{

using BGLgeom::operator>>;	// To let the compiler find the overload of operator>> for BGLgeom::point

//! The reader based on reader_ASCII to read the data about the fractures
class reader_fractures : public BGLgeom::reader_ASCII	<Fracture::Vertex_prop, Fracture::Edge_prop> {
	//inside public we have to override all the abstract method of reader_ASCII and of the constructor
	public:
		//! Constructor
		reader_fractures(std::string _filename) :	BGLgeom::reader_ASCII	<Fracture::Vertex_prop,
																			 Fracture::Edge_prop>(_filename),
													SRC(),
													TGT(),
													K_t(),
													K_n(),
													df(),
													source_term() {};
													
		//! Reading data from line
		void get_data(){
			this->in_file >> SRC >> TGT >> K_t >> K_n >> df >> source_term;
		}
		
		//! Returning data on the edge
		Fracture::Edge_prop get_edge_data(){
			return Fracture::Edge_prop(K_t, K_n, df, source_term);
		}
		
		//! Returning data on the source
		Fracture::Vertex_prop get_source_data(){
			return Fracture::Vertex_prop(SRC);
		}
		
		//! Returning data on the target
		Fracture::Vertex_prop get_target_data(){
			return Fracture::Vertex_prop(TGT);
		}
		
		//! Nothing to do
		BGLgeom::no_topological_data get_topological_data(){
			return BGLgeom::no_topological_data();
		};
		
	private:
		//! Coordinates of source and target
		Fracture::Vertex_prop::point_t SRC, TGT;
		//! Parameters
		double K_t, K_n, df, source_term;
};	//reader_fractures
												 
}	//Fracture
						 
#endif	//HH_READER_FRACTURES_HH
