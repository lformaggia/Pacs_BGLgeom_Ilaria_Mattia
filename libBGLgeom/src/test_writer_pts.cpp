/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	test_writer_pts.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Testing class writer_pts to show how it works and how to 
			produce pts output
	
	We perfom two different tests: \n
	- Creation of a graph with two boundary conditions in the 
		vertex properties. Values assigned to them, then 
		computation a uniform mesh on every edge. Production of 
		the pts output files \n
	- Creation of a standard graph. Values assigned to the 
		properties and computation of a uniform mesh on the edges. 
		Production of the pts output files, asking for the 
		evaluation of the main geometrical properties of each
		edge in correspondence of the points of the mesh
*/

#include <iostream>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include "writer_pts.hpp"
#include "base_properties.hpp"
#include "graph_builder.hpp"
#include "linear_geometry.hpp"

using namespace boost;
using namespace BGLgeom;

int main(){

	// testing a graph with two boundary conditions in the vertex properties
	using Graph = adjacency_list<vecS,vecS,undirectedS,Vertex_base_property<3,2>,Edge_base_property<linear_geometry<3>,3> >;
	Graph G;
	
	Vertex_desc<Graph> a,b,c;
	a = add_vertex(G);	//boost
	b = add_vertex(G);	//boost
	c = add_vertex(G);	//boost
	G[a].coordinates = point<3>(0,1,2);
	G[b].coordinates = point<3>(2,3,5);
	G[c].coordinates = point<3>(3,1,6);
	G[a].BC[0].type = BC_type::DIR;
	G[a].BC[0].value = 3.5;
	G[a].BC[1].type = BC_type::DIR;
	G[a].BC[1].value = 3.7;
	G[c].BC[0].type = BC_type::NEU;
	G[c].BC[0].value = 4.5;
	G[c].BC[1].type = BC_type::INT;
	G[c].BC[1].value = 3.5;
	
	Edge_desc<Graph> e1,e2;
	e1 = add_edge(a,b,G).first;	//boost
	e2 = add_edge(b,c,G).first;	//boost
	
	G[e1].index = 1;
	G[e2].index = 2;	
	G[e1].geometry.set_source(G[a].coordinates);
	G[e1].geometry.set_target(G[b].coordinates);
	G[e2].geometry.set_source(G[b].coordinates);
	G[e2].geometry.set_target(G[c].coordinates);
	
	//Creating a uniform mesh
	G[e1].mesh.uniform_mesh(20, G[e1].geometry);	
	G[e2].mesh.uniform_mesh(20, G[e2].geometry);
	
	std::string filename1 = "../data/out_test_2BC_writer_pts.pts";
	writer_pts<Graph,3,2> W(filename1);
	W.export_pts(G);
	
	
	// Now testing the writer on the additional infos to print
	using Graph2 = adjacency_list<vecS,vecS,undirectedS,Vertex_base_property<3>,Edge_base_property<linear_geometry<3>,3> >;
	Graph2 G2;
	
	Vertex_desc<Graph2> d,e,f;
	d = add_vertex(G2);	//boost
	e = add_vertex(G2);	//boost
	f = add_vertex(G2);	//boost
	G2[d].coordinates = point<3>(0,1,2);
	G2[e].coordinates = point<3>(2,3,5);
	G2[f].coordinates = point<3>(3,1,6);
	G2[d].BC[0].type = BC_type::DIR;
	G2[d].BC[0].value = 3.5;
	G2[f].BC[0].type = BC_type::NEU;
	G2[f].BC[0].value = 4.5;
	
	Edge_desc<Graph2> e3,e4;
	e3 = add_edge(d,e,G2).first;	//boost
	e4 = add_edge(e,f,G2).first;	//boost
	
	G2[e3].geometry.set_source(G2[d].coordinates);
	G2[e3].geometry.set_target(G2[e].coordinates);
	G2[e4].geometry.set_source(G2[e].coordinates);
	G2[e4].geometry.set_target(G2[f].coordinates);
	G2[e3].index = 1;
	G2[e4].index = 2;
	
	//Creating a uniform mesh
	G2[e3].mesh.uniform_mesh(20, G[e3].geometry);	
	G2[e4].mesh.uniform_mesh(20, G[e4].geometry);
	
	std::string filename2 = "../data/out_test_moreinfo_writer_pts.pts";
	writer_pts<Graph2,3> W2(filename2);
	W2.export_pts(G2,true);
	
	return 0;
}
