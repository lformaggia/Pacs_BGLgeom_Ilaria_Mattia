/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file reader_fractures_twolines.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Class to read ASCII file for fractures application
*/

#ifndef HH_READER_FRACTURES_TWOLINES_HH
#define HH_READER_FRACTURES_TWOLINES_HH

#include "reader_ASCII.hpp"
#include "fracture_graph_properties.hpp"

namespace Fracture{

/*! 
	@brief The reader based on reader_ASCII to read the data about the fractures
	
	In this reader we will read to lines of input file at a time, because
	the input file has a different structure: it has the coordinates of the 
	source in the first line, and the coordinates of the target in the 
	second one.
*/
class reader_fractures_twolines : public BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop> {
	//inside public we have to override all the abstract method of reader_ASCII and the constructor
	public:
		//! Constructor
		reader_fractures_twolines(std::string _filename) :	BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop>(_filename),
															SRC(),
															TGT(),
															discard1(),
															discard2() {};
		
		/*! 
			@brief Reading data from line
			
			The format of the file is: \n
			- 1=source; coordinates; -10=third coordinate (ignored) \n
			- 2=target; coordinates; -10=third coordinate (ignored) \n
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
