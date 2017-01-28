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
	@file reader_fractures.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Class to read ASCII file for fractures application
*/

#ifndef HH_READER_FRACTURES_HH
#define HH_READER_FRACTURES_HH

#include "reader_ASCII.hpp"
#include "fracture_graph_properties.hpp"

namespace Fracture{

using BGLgeom::operator>>;	// To let the compiler find the overload of operator>> for BGLgeom::point

//! The reader based on reader_ASCII to read the data about the lines
class reader_lines : public BGLgeom::reader_ASCII	<Fracture::Vertex_prop, Fracture::Edge_prop> {
	//inside public we have to override all the abstract method of reader_ASCII and the constructor
	public:
		//! Constructor
		reader_lines(std::string _filename) :	BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop>(_filename),
												SRC(),
												TGT() {};
													
		//! Reading data from line
		void get_data(){
			this->in_file >> SRC >> TGT;
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

};	//reader_lines

}  // namespace Fracture											 						 
#endif	//HH_READER_FRACTURES_HH
