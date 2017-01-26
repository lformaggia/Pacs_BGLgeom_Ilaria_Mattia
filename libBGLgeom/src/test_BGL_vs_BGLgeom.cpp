/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	test_BGL_vs_BGLgeom.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Showing how to build a graph with BGL and with BGLgeom
	
	In this test we provide two ways to build the same graph with
	the same properties, in order to show how the native BGL 
	functions work w.r.t. our newer function implemented in BGLgeom
*/

#include <boost/graph/adjacency_list.hpp>
#include <tuple>
#include <iostream>
#include "base_properties.hpp"
#include "graph_builder.hpp"
#include "graph_access.hpp"
#include "linear_geometry.hpp"
#include "point.hpp"

int main(){
	
	// Declaration of the graph, common to BGL and BGLgeom
	using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
										BGLgeom::Vertex_base_property<2>, 
										BGLgeom::Edge_base_property< BGLgeom::linear_geometry<2>, 2> >;
	Graph G1, G2;
	
	// Using Boost Graph Library
	std::cout << "===== CREATING A GRAPH WITH BGL =====" << std::endl;
	// Creating vertex descriptors
	typename boost::graph_traits<Graph>::vertex_descriptor a1, b1, c1, d1;
	
	// Adding the vertices to the graph
	a1 = boost::add_vertex(G1);
	b1 = boost::add_vertex(G1);
	c1 = boost::add_vertex(G1);
	// Adding the last vertex along with its properties
	BGLgeom::point<2> D1(3,3);
	BGLgeom::Vertex_base_property<2> d1_prop(D1);
	d1 = boost::add_vertex(d1_prop, G1);
	
	// Assigning properties to vertices
	G1[a1].coordinates = BGLgeom::point<2>(0,0);
	G1[b1].coordinates = BGLgeom::point<2>(1,1);
	G1[c1].coordinates = BGLgeom::point<2>(2,2);
	
	// Creation of the edge descriptors
	typename boost::graph_traits<Graph>::edge_descriptor e1, f1, h1;
	
	// Adding one edge
	e1 = boost::add_edge(a1, b1, G1).first;
	// Adding the second edge with full return value
	bool inserted;
	std::tie(f1, inserted) = boost::add_edge(b1, c1, G1);
	// Adding the last edge along with its properties
	BGLgeom::linear_geometry<2> lin1(G1[c1].coordinates, G1[d1].coordinates);
	BGLgeom::Edge_base_property<BGLgeom::linear_geometry<2>,2> h1_prop(lin1);
	h1 = boost::add_edge(c1, d1, h1_prop, G1).first;
	
	// Setting up geometries
	G1[e1].geometry.set_source(G1[a1].coordinates);
	G1[e1].geometry.set_target(G1[b1].coordinates);
	G1[f1].geometry.set_source(G1[b1].coordinates);
	G1[f1].geometry.set_target(G1[c1].coordinates);
	
	// Assigning properties
	G1[e1].label = "edge 1 graph 1";
	G1[f1].label = "edge 2 graph 1";
	G1[h1].label = "whatever, graph 1";
	
	// Iterating over the edges and printing an output
	typename boost::graph_traits<Graph>::edge_iterator e_it1, e_end1;
	for(std::tie(e_it1, e_end1) = boost::edges(G1); e_it1 != e_end1; ++e_it1)
		std::cout << G1[*e_it1] << std::endl;
	std::cout << std::endl;
	
	// Now with the BGLgeom
	std::cout << "===== CREATING A GRAPH WITH BGLgeom =====" << std::endl;
	// Creating vertex descriptors
	BGLgeom::Vertex_desc<Graph> a2, b2, c2, d2;
	
	// Adding the vertices to the graph
	a2 = BGLgeom::new_vertex(G2);
	b2 = BGLgeom::new_vertex(G2);
	c2 = BGLgeom::new_vertex(G2);
	// Adding the last vertex along with its properties
	BGLgeom::point<2> D2(10,10);
	BGLgeom::Vertex_base_property<2> d2_prop(D2);
	d2 = BGLgeom::new_vertex(d2_prop, G2);
	
	// Assigning properties to vertices
	G2[a2].coordinates = BGLgeom::point<2>(7,7);
	G2[b2].coordinates = BGLgeom::point<2>(8,8);
	G2[c2].coordinates = BGLgeom::point<2>(9,9);
	
	// Creation of the edge descriptors
	BGLgeom::Edge_desc<Graph> e2, f2, h2;
	
	// Adding one edge
	e2 = BGLgeom::new_edge(a2, b2, G2);
	// Adding the second edge directly with linear geometry (i.e. the linear geometry will be alreadt set up)
	f2 = BGLgeom::new_linear_edge(b2, c2, G2);
	// Adding the last edge along with its properties
	BGLgeom::linear_geometry<2> lin2(G2[c2].coordinates, G2[d2].coordinates);
	BGLgeom::Edge_base_property<BGLgeom::linear_geometry<2>,2> h2_prop(lin2);
	h2 = BGLgeom::new_edge(c2, d2, h2_prop, G2);
	
	// Setting up geometries
	G2[e2].geometry.set_source(G2[a2].coordinates);
	G2[e2].geometry.set_target(G2[b2].coordinates);
	/*	//No need to do this, already done in new_linear_edge
	G2[f2].geometry.set_source(G2[b2].coordinates);
	G2[f2].geometry.set_target(G2[c2].coordinates);
	*/
	
	// Assigning properties
	G2[e2].label = "edge 1 graph 2";
	G2[f2].label = "edge 2 graph 2";
	G2[h2].label = "whatever, graph 2";
	
	// Iterating over the edges and printing an output
	BGLgeom::Edge_iter<Graph> e_it2, e_end2;
	for(std::tie(e_it2, e_end2) = BGLgeom::edges(G2); e_it2 != e_end2; ++e_it2)
		std::cout << G2[*e_it2] << std::endl;
	std::cout << std::endl;
	return 0;
}
