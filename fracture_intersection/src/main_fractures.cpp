/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
	@file main_Formaggia.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Source code for Formaggia's example
	@datail
*/

#include <iostream>
#include <string>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "reader_fractures.hpp"
#include "data_structure.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace Fracture;

int main(){

	using Graph = adjacency_list<vecS, vecS, directedS, BGLgeom::Vertex_base_property<2>, Edge_data>;
	
	std::string filename("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/fracture_intersection/data/fractureElevenVF.dat");
	
	Graph G;
	reader_fractures<Edge_data> R(filename);
	
	// Utilities to read the data
	Vertex_base_property<2> src_prop, tgt_prop;
	Edge_data e_prop;
	Edge_desc<Graph> e;
	Vertex_desc<Graph> src(1), tgt(2);	//Initialized
	
	// Reading the file
	R.ignore_dummy_lines(7);
	while(!R.is_eof()){
		R.get_data_from_line();
		src_prop = R.get_source_data();
		tgt_prop = R.get_target_data();
		e_prop = R.get_edge_data();
		e = add_edge(src, tgt, G).first;
		src = source(e, G);
		tgt = target(e, G);
		G[src] = src_prop;
		G[tgt] = tgt_prop;
		G[e] = e_prop;
		G[e].geometry.set_source(G[src].coordinates);
		G[e].geometry.set_target(G[tgt].coordinates);
		//This two lines to make possible execute add_edge with always different vertices
		src += 2;
		tgt += 2;	
	}	//while
	
	unsigned int count = 0;
	Edge_iter<Graph> e_it, e_end;
	//Printing out data
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it){
		count++;
		src = source(*e_it, G);
		tgt = target(*e_it, G);
		std::cout << "Edge " << count << ": " << std::endl;
		std::cout << G[src].coordinates << ", " << G[tgt].coordinates << std::endl;
		std::cout << "Parameters: K_t " << G[*e_it].K_t << "; K_n " << G[*e_it].K_n << std::endl;
	}
	
	return 0;
};
