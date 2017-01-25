/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file main_fractures.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Source code for fractures application
*/

#include <iostream>
#include <string>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "types_definition.hpp"  // using-declarations local to this application 
#include "reader_fractures.hpp"
#include "reader_fractures_twolines.hpp"
#include "graph_builder.hpp"
#include "helper_functions.hpp"
#include "writer_vtp.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace Fracture;

int main(){
	
	// Utilities to create the graph:	
	
	// Edge iterators to iterate over the graph
	Edge_it e_it, e_end;
	// Vertex iterators to iterate over the graph
	Vertex_it v_it, v_end;	
	// The descriptor of the new added edges (it will be overwritten by any new addition)
	Edge_d e;
	
	// Debug utilities: counter for edges and vertices in the graph
	int count_e = 0;
	int count_v = 0;
	
	cout << "===================== GRAPH 1 ===================" << endl;
	std::string filename1("../data/fractureElevenVF.dat");
	cout << "Input filename: " << filename1 << endl << endl;
	
	Graph G1;
	
	// Reading the file1
	reader_fractures R1(filename1);	
	R1.ignore_dummy_lines(7);	
	
	// This function handles the entire construction of the graph while reading the input file contained in the reader
	create_graph(G1,R1);
	
	// Counting total number of vertices and edges	
	for (std::tie(e_it, e_end) = boost::edges(G1); e_it != e_end; ++e_it)
		++count_e;	
		
	for (std::tie(v_it, v_end) = boost::vertices(G1); v_it != v_end; ++v_it)
		++count_v;
			
	cout << endl;
	std::cout << "________________ FINAL SETTING GRAPH 1 ________________" << std::endl;
	std::cout << count_v << " vertices and " << count_e << " edges" <<std::endl;
	
	//Producing output
	std::string filename1_out("../data/graph_fractureEleven.vtp");	
	writer_vtp<Graph,2> W1(filename1_out.c_str());
	W1.export_vtp(G1);
	
	cout<<endl;
	cout<<endl;
	
//==================================== THE OTHER INPUT FILE ==================================
	
	cout<<endl;
	cout<<"========================= GRAPH 2 =========================="<<endl;
	
	// Reinitializing variables
	count_e = 0; 
	count_v = 0;	
	
	Graph G2;
	
	// Reading file 2
	std::string filename2("../data/fracture.txt");
	reader_fractures_twolines R2(filename2);

	// This function handles the entire construction of the graph while reading the input file contained in the reader
	create_graph(G2,R2);
	
	// Counting total number of vertices and edges
	for (std::tie(e_it, e_end) = boost::edges(G2); e_it != e_end; ++e_it)
		++count_e;	

	for (std::tie(v_it, v_end) = boost::vertices(G2); v_it != v_end; ++v_it)
		++count_v;
	std::cout << endl;
	std::cout << "________________ FINAL SETTING GRAPH 2 ________________" << std::endl;
	std::cout << count_v << " vertices and " << count_e << " edges" << std::endl;
	
	// Production of the output
	std::string filename2_out("../data/graph_fracture.vtp");
	writer_vtp<Graph,2> W2(filename2_out.c_str());
	W2.export_vtp(G2);	
	
	return 0;
}
