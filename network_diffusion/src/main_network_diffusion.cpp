/*!
	@file main_network_diffusion.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Application on diffusion on a network
	@detail
*/

#include <iostream>
#include <string>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "netdiff_graph_properties.hpp"
#include "reader_netdiff.hpp"
#include "writer_pts.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace NetDiff;

int main(){

	using Graph = adjacency_list<vecS, vecS, directedS, Vertex_prop, Edge_prop>;
	
	std::string filename("../data/rattm93a.txt");
	
	Graph G;
	reader_netdiff R(filename);
	
	// Utilities to read the data
	Vertex_prop src_prop, tgt_prop;
	Edge_prop e_prop;
	Topological_prop topo_prop;
	Edge_desc<Graph> e;
	Vertex_desc<Graph> src, tgt;
	
	// Reading the file
	unsigned int count = 1;
	R.ignore_dummy_lines(2);
	while(!R.is_eof()){
		R.get_data();
		src_prop = R.get_source_data();
		tgt_prop = R.get_target_data();
		e_prop = R.get_edge_data();
		topo_prop = R.get_topological_data();
		e = add_edge(topo_prop.src, topo_prop.tgt, G).first;
		G[topo_prop.src] = src_prop;
		G[topo_prop.tgt] = tgt_prop;
		G[e] = e_prop;
		G[e].index = count;
		count++;
		// Commenting: now we put this commandas directly into the reader
		/*
		G[e].geometry.set_source(G[topo_prop.src].coordinates);
		G[e].geometry.set_target(G[topo_prop.tgt].coordinates);
		*/
	}	//while
	
	Edge_iter<Graph> e_it, e_end;
	//Printing out data for Graph1
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it){
		src = source(*e_it, G);
		tgt = target(*e_it, G);
		std::cout << "Edge " << G[*e_it].index << ": " << std::endl;
		std::cout << G[src].coordinates << ", " << G[tgt].coordinates;
		std::cout << "; Diameter:  " << G[*e_it].diam << std::endl;
	}
	
	// Creating a mesh on every edge
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it)
		G[*e_it].make_uniform_mesh(20);
	
	// Writing on a pts output
	std::string out_pts_filename("../data/rattm93a.pts");
	writer_pts<Graph,3> W(out_pts_filename);
	W.export_pts(G);
	
	return 0;
}
