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
	@file main_fractures.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Source code for fractures application
*/

#include <iostream>
#include <string>
#include <tuple>

#include "types_definition.hpp"  // using-declarations local to this application 
#include "reader_fractures.hpp"
#include "reader_fractures_twolines.hpp"
#include "helper_functions.hpp"
#include "writer_vtp.hpp"

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
	
	std::cout << "===================== GRAPH 1 ===================" << std::endl;
	std::string filename1("../data/fractureElevenVF.dat");
	std::cout << "Input filename: " << filename1 << std::endl << std::endl;
	
	Graph G1;
	
	// Reading the file1
	reader_fractures R1(filename1);	
	R1.ignore_dummy_lines(7);	
	
	// This function handles the entire construction of the graph while reading the input file contained in the reader
	create_graph(G1,R1);
	
	// Counting total number of vertices and edges	
	for (std::tie(e_it, e_end) = edges(G1); e_it != e_end; ++e_it)
		++count_e;		
	for (std::tie(v_it, v_end) = vertices(G1); v_it != v_end; ++v_it)
		++count_v;
			
	std::cout << std::endl;
	std::cout << "________________ FINAL SETTING GRAPH 1 ________________" << std::endl;
	std::cout << count_v << " vertices and " << count_e << " edges" << std::endl;
	
	//Producing output
	std::string filename1_out("../data/graph_fractureEleven.vtp");	
	writer_vtp<Graph,2> W1(filename1_out.c_str());
	W1.export_vtp(G1);
	
	std::cout << std::endl << std::endl;
	
//==================================== THE OTHER INPUT FILE ==================================
	
	std::cout << std::endl;
	std::cout << "========================= GRAPH 2 ==========================" << std::endl;
	
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
	for (std::tie(e_it, e_end) = edges(G2); e_it != e_end; ++e_it)
		++count_e;	
	for (std::tie(v_it, v_end) = vertices(G2); v_it != v_end; ++v_it)
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
