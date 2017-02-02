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
	@file main_example.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Source code for fractures application: simulated points in order to show all the intersections types
*/

#include <iostream>
#include <string>
#include <tuple>

#include "types_definition.hpp"  // using-declarations local to this application 
#include "reader_example.hpp"
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
	std::string filename("../data/test_all_cases.txt");
	std::cout << "Input filename: " << filename << std::endl << std::endl;
	
	Graph G;
	
	// Reading the file1
	reader_example R(filename);	
	R.ignore_dummy_lines(1);	
	
	// This function handles the entire construction of the graph while reading the input file contained in the reader
	create_graph(G,R);
	
	// Counting total number of vertices and edges	
	for (std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it)
		++count_e;		
	for (std::tie(v_it, v_end) = vertices(G); v_it != v_end; ++v_it)
		++count_v;
			
	std::cout << std::endl;
	std::cout << "________________ FINAL SETTING ________________" << std::endl;
	std::cout << count_v << " vertices and " << count_e << " edges" << std::endl;
	
	//Producing output
	std::string filename_out("../data/out_main_example.vtp");	
	writer_vtp<Graph,2> W(filename_out.c_str());
	W.export_vtp(G);
	
	std::cout << std::endl << std::endl;
	
	return 0;
}
