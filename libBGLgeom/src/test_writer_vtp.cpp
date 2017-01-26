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
	@file	test_writer_vtp.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Testing class writer_vtp to show how it works and how to 
			produce vtp output
	
	Creation a simple graph, assignment of some properties and 
	production of the vtp output
			
*/

#include <boost/graph/adjacency_list.hpp>
#include <string>
#include "base_properties.hpp"
#include "linear_geometry.hpp"
#include "graph_builder.hpp"
#include "graph_access.hpp"
#include "writer_vtp.hpp"

using namespace BGLgeom;

int main(){

	using Graph = boost::adjacency_list< boost::vecS, 
										 boost::vecS, 
										 boost::directedS, 
										 Vertex_base_property<3>, 
										 Edge_base_property<linear_geometry<3>,3> >;
	Graph G;

	Vertex_desc<Graph> a,b,c;
	a = new_vertex(G);
	b = new_vertex(G);
	c = new_vertex(G);		
	G[a].coordinates = point<3>(0,0,0);
	G[b].coordinates = point<3>(1,1,1);
	G[c].coordinates = point<3>(0,1,0);
	
	Edge_desc<Graph> e;
	e = new_linear_edge(a,b,G);
	e = new_linear_edge(a,c,G);	
	
	std::string filename("../data/out_test_writer_vtp.vtp");
	writer_vtp<Graph,3> W(filename.c_str());
	W.export_vtp(G);
	
	return 0;
}
