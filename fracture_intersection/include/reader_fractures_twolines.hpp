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

#ifndef HH_READER_FRACTURES_TWOLINES_HH
#define HH_READER_FRACTURES_TWOLINES_HH

#include "reader_ASCII.hpp"
#include "fracture_graph_properties.hpp"

namespace Fracture{

/*! 
	@brief The reader based on reader_ASCII to read the data about the fractures
	@detail In this reader we will read to lines of input file at a time, because
			the input file has a different structure: it has the coordinates of the 
			source in the first line, and the coordinates of the target in the 
			second one.
*/
class reader_fractures_twolines : public BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop> {
	//inside public we have to override all the abstract method of reader_ASCII and of the constructor
	public:
		//! Constructor
		reader_fractures_twolines(std::string _filename) :	BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop>(_filename),
															SRC(),
															TGT(),
															discard1(),
															discard2() {};
		
		/*! 
			@brief Reading data from line
			@detail The format of the file is: \n
					- 1=source; coordinates; -10=third coordinate \n
					- 2=target; coordinates; -10=third coordinate \n
					This two lines form an edge
		*/
		void get_data(){
			this->in_file >> discard1 >> SRC >> discard2 >> discard1 >> TGT >> discard2;
		}
		
		//! Returning data on the edge
		Fracture::Edge_prop get_edge_data(){
			return Fracture::Edge_prop();
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
		//! Dummy variable
		double discard1, discard2;
};	//reader_fractures_twolines
												 
}	//Fracture
						 
#endif	//HH_READER_FRACTURES_TWOLINES_HH
