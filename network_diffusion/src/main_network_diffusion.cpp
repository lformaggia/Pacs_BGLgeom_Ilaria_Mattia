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
	
	std::string filename("../data/bifurcation_network.txt");
	
	Graph G;
	reader_netdiff R(filename);
	
	// Utilities to read the data
	Vertex_prop src_prop, tgt_prop;
	Edge_prop e_prop;
	Edge_desc<Graph> e;
	Vertex_desc<Graph> src, tgt;
	
	// Reading the file
	unsigned int count = 0;
	while(!R.is_eof()){
		R.get_data();
		src_prop = R.get_source_data();
		tgt_prop = R.get_target_data();
		//e_prop = R.get_edge_data();
		src = new_vertex(src_prop, G);
		tgt = new_vertex(tgt_prop, G);
		e = new_linear_edge(src, tgt, G);
		G[e].index = count;
		count++;
	}	//while
	
	Edge_iter<Graph> e_it, e_end;
	//Printing out data for Graph1
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it){
		src = source(*e_it, G);
		tgt = target(*e_it, G);
		std::cout << "Edge " << G[*e_it].index << ": " << std::endl;
		std::cout << G[src].coordinates << ", " << G[tgt].coordinates;
	}
	
	// Creating a mesh on every edge
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it)
		G[*e_it].make_uniform_mesh(20);
	
	// Writing on a pts output
	std::string out_pts_filename("../data/bifurcaion_network.pts");
	writer_pts<Graph,3> W(out_pts_filename);
	W.export_pts(G);
	
	
	// Declare a new problem 
		getfem::transport3d1d p; 
		  
		/// fluid problem: velocity field and pressure
		// Initialize the problem
		p.problem3d1d::init(argc, argv);
		// Build the monolithic system 
		p.problem3d1d::assembly();
		//transport problem: concentration  
		//initialize 
		p.init(argc, argv);
		//assemble        
		p.assembly();    
		//solve     
		if (!p.solve()) GMM_ASSERT1(false, "solve procedure has failed");  // the export is in the solve at each time step 
				      
	
	
	
	return 0;
}
