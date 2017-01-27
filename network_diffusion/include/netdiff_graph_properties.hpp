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
	@file	netdiff_graph_properties.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date 	Jan, 2017
	@brief	Properties that the graph for diffusion on network appliocation will have
*/

#ifndef HH_NETDIFF_GRAPH_PROPERTIES_HH
#define HH_NETDIFF_GRAPH_PROPERTIES_HH

#include "base_properties.hpp"
#include "linear_geometry.hpp"

namespace NetDiff{

//! Properties that vertices will have
using Vertex_prop = BGLgeom::Vertex_base_property<3>;

//! Base properties for the edges (no more properties required)
using Edge_prop = BGLgeom::Edge_base_property< BGLgeom::linear_geometry<3>, 3>;

}	//NetDiff

#endif	//HH_NETDIFF_GRAPH_PROPERTIES_HH
