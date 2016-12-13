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
	@brief Source code for fractures application
	@datail
*/

#include <iostream>
#include <string>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "fracture_graph_properties.hpp"
#include "reader_fractures.hpp"
#include "reader_fractures_twolines.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace Fracture;

int main(){

	using Graph = adjacency_list<vecS, vecS, directedS, Vertex_prop, Edge_prop>;
	
	std::string filename1("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/fracture_intersection/data/fractureElevenVF.dat");
	std::string filename2("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/fracture_intersection/data/fracture.txt");
	
	Graph G1;
	Graph G2;
	reader_fractures R1(filename1);
	reader_fractures_twolines R2(filename2);
	
	// Utilities to read the data
	Vertex_prop src_prop, tgt_prop;
	Edge_prop e_prop;
	Edge_desc<Graph> e;
	Vertex_desc<Graph> src, tgt;
	unsigned int frac_number = 1;
	
	// Reading the file1
	R1.ignore_dummy_lines(7);
	while(!R1.is_eof()){
		R1.get_data_from_line();
		src_prop = R1.get_source_data();
		tgt_prop = R1.get_target_data();
		e_prop = R1.get_edge_data();
		src = add_vertex(G1);
		tgt = add_vertex(G1);
		e = add_edge(src, tgt, G1).first;
		G1[src] = src_prop;
		G1[tgt] = tgt_prop;
		G1[e] = e_prop;
		G1[e].geometry.set_source(G1[src].coordinates);
		G1[e].geometry.set_target(G1[tgt].coordinates);
		G1[e].frac_num = frac_number;
		frac_number++;
	}	//while
	
	Edge_iter<Graph> e_it, e_end;
	//Printing out data for Graph1
	for(std::tie(e_it, e_end) = edges(G1); e_it != e_end; ++e_it){
		src = source(*e_it, G1);
		tgt = target(*e_it, G1);
		std::cout << "Edge " << G1[*e_it].frac_num << ": " << std::endl;
		std::cout << G1[src].coordinates << ", " << G1[tgt].coordinates << std::endl;
		std::cout << "Parameters: K_t " << G1[*e_it].K_t << "; K_n " << G1[*e_it].K_n << std::endl;
	}
	
	frac_number = 1;
	// Reading file2
	while(!R2.is_eof()){
		R2.get_data_from_line();
		src_prop = R2.get_source_data();
		tgt_prop = R2.get_target_data();
		//e_prop = R1.get_edge_data();
		src = add_vertex(G2);
		tgt = add_vertex(G2);
		e = add_edge(src, tgt, G2).first;
		G2[src] = src_prop;
		G2[tgt] = tgt_prop;
		//G2[e] = e_prop;
		G2[e].geometry.set_source(G2[src].coordinates);
		G2[e].geometry.set_target(G2[tgt].coordinates);
		G2[e].frac_num = frac_number;
		frac_number++;
	}	//while
	
	//Printing out data for Graph2
	for(std::tie(e_it, e_end) = edges(G2); e_it != e_end; ++e_it){
		src = source(*e_it, G2);
		tgt = target(*e_it, G2);
		std::cout << "Edge " << G2[*e_it].frac_num << ": ";
		std::cout << G2[src].coordinates << ", " << G2[tgt].coordinates << std::endl;
	}
	
	return 0;
};
