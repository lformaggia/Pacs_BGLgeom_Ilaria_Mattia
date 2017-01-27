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
	@file types_definition.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Aliases for the types used in the application
	
	It is possible to change here the template parameters for the 
	graph properties if other kind of properties are needed. The 
	other code will remain working
*/

#ifndef HH_TYPES_DEFINITION_HH
#define HH_TYPES_DEFINITION_HH

#include <boost/graph/adjacency_list.hpp>

#include "graph_access.hpp"
#include "fracture_graph_properties.hpp"
#include "point.hpp"
#include "linear_geometry.hpp"
#include "reader_ASCII.hpp"

namespace Fracture{

//! Alias for points
using point2 = BGLgeom::point<2>;

//! Alias for the linear geometry of the edges
using line = BGLgeom::linear_geometry<2>;

//! Alias for the type of the graph used
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Fracture::Vertex_prop , Fracture::Edge_prop>;

//! Alias type for the concrete reader
using Reader = BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop>;

//! Alias for the edge iterators for the graph
using Edge_it = BGLgeom::Edge_iter<Graph>;
//! Alias for the vertex iterator for the graph
using Vertex_it = BGLgeom::Vertex_iter<Graph>;
//! Alias for the vertex descriptors for the graph
using Vertex_d = BGLgeom::Vertex_desc<Graph>;
//! Alias for the edge descriptors for the graph
using Edge_d = BGLgeom::Edge_desc<Graph>;

}	//Fracture

#endif	//HH_TYPES_DEFINITION_HH
