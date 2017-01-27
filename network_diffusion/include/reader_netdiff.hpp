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
	@file reader_netdiff.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Class to read from input for application on network diffusion
*/

#ifndef HH_READER_NETDIFF_HH
#define HH_READER_NETDIFF_HH

#include <string>
#include "reader_ASCII.hpp"
#include "netdiff_graph_properties.hpp"
#include "boundary_conditions.hpp"

namespace NetDiff{

// To allow the compiler find input operator for points
using BGLgeom::operator>>;

/*! 
	@brief The concrete class to read ASCII input file for the network diffusion example
	
	This reader will read information of this type: \n
	'source coordiantes' 'target coordinates' 'source BC' 'target BC' \n
	Each line represent an edge.
*/
class reader_netdiff : public BGLgeom::reader_ASCII	<NetDiff::Vertex_prop,
													 NetDiff::Edge_prop> {
	public:
		//! Constructor
		reader_netdiff(std::string _filename) : BGLgeom::reader_ASCII	<NetDiff::Vertex_prop,
																		 NetDiff::Edge_prop>(_filename),
												SRC(),
												TGT(),
												BC_src(),
												BC_tgt() {}
												
		//! Reading form input
		void get_data(){
			this->in_file >> SRC >> TGT >> BC_src >> BC_tgt;
		}
		
		//! Returning data on the source
		NetDiff::Vertex_prop get_source_data(){
			return NetDiff::Vertex_prop(SRC, {BC_src});
		}
		
		//! Returning data on the target
		NetDiff::Vertex_prop get_target_data(){
			return NetDiff::Vertex_prop(TGT, {BC_tgt});
		}
		
		/*!
			@brief Returning data on the edge
			
			We don't have particular properties on the edges in this application,
			so we return a default edge property. Anyway, there will be no need 
			to use this member, since creating a new edge already means to 
			default construct all its properties
		*/
		NetDiff::Edge_prop get_edge_data(){
			return NetDiff::Edge_prop();	//volendo qua si può restituire vuoto, tanto non c'è nessuna property particolare
		}
		
		/*!
			@brief Returning topological data
			
			No need of topological data, leaving it blank using the empty
			struct provided in the BGLgeom
		*/
		BGLgeom::no_topological_data get_topological_data(){
			return BGLgeom::no_topological_data();
		}
		
	private:
		//! Coordinates of source and target
		NetDiff::Vertex_prop::point_t SRC, TGT;
		//! Boundary condition on a vertex
		BGLgeom::boundary_condition BC_src, BC_tgt;

};	//reader_netdiff



}	//NetDiff

#endif	//HH_READER_NETDIFF_HH
