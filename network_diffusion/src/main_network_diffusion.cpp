/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file main_network_diffusion.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Application on diffusion on a network
*/

#include <iostream>
#include <string>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "netdiff_graph_properties.hpp"
#include "reader_netdiff.hpp"
#include "writer_pts.hpp"
#include "graph_builder.hpp"

#include "problem3d1d.hpp"
#include "transport3d1d.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace NetDiff;

int main(int argc, char* argv[]){

	using Graph = adjacency_list<vecS, vecS, directedS, Vertex_prop, Edge_prop>;
	
	std::string in_filename("../data/bifurcation_network.txt");
	
	Graph G;
	reader_netdiff R(in_filename);
	
	// Utilities to read the data and build the graph
	Vertex_prop src_prop, tgt_prop;
	//Edge_prop e_prop;
	Edge_desc<Graph> e;
	Vertex_desc<Graph> src, tgt;
	unsigned int count = 0;
	
	// Reading the file
	while(!R.is_eof()){
		// Reading data
		R.get_data();
		// Creating edge and setting it up
		src_prop = R.get_source_data();
		tgt_prop = R.get_target_data();
		//e_prop = R.get_edge_data();
		src = new_vertex(src_prop, G);
		tgt = new_vertex(tgt_prop, G);
		e = new_linear_edge(src, tgt, G);
		G[e].index = count;
		count++;
	}
	
	// Creating a mesh on every edge
	Edge_iter<Graph> e_it, e_end;
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it)
		G[*e_it].make_uniform_mesh(20);
	
	// Writing on a pts output
	std::string out_pts_filename("../data/bifurcation_network.pts");
	writer_pts<Graph,3> W(out_pts_filename);
	W.export_pts(G);	
	
	std::cout << "--- Solving diffusion problem ---" << std::endl << std::endl;	
	// Declare a new problem 
	getfem::transport3d1d p;	  
	// Fluid problem: velocity field and pressure
	p.problem3d1d::init(argc, argv);	// Initialize the problem
	p.problem3d1d::assembly();
	p.init(argc, argv);
	p.assembly();    					// Build the monolithic system
	// Solve the problem
	if (!p.solve()) GMM_ASSERT1(false, "solve procedure has failed");
		
	return 0;
}
