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

#include "local_data_structure.hpp"
#include "reader_fractures.hpp"
#include "reader_fractures_twolines.hpp"
#include "helper_functions.hpp"
#include "writer_vtk.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace Fracture;

int main(){
	
	std::string filename1("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/fracture_intersection/data/fractureElevenVF.dat");
	std::string filename2("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/fracture_intersection/data/fracture.txt");
	
	Graph G1;
	Graph G2;
	reader_fractures R1(filename1);
	reader_fractures_twolines R2(filename2);
	
	// Reading the file1
	R1.ignore_dummy_lines(7);
	
 	create_graph(G1,R1);
	
	int count_vertices = 0;
	int count_edges = 0;
	
	Edge_it e_it, e_end;
	Vertex_it v_it, v_end;
	
	for (std::tie(e_it, e_end) = boost::edges(G1); e_it != e_end; ++e_it) //for loop on al the edges already in the graph
		++count_edges;
		
	for (std::tie(v_it, v_end) = boost::vertices(G1); v_it != v_end; ++v_it) //for loop on al the edges already in the graph
		++count_vertices;
	
	std::cout << "________________ FINAL COUNT ________________"<<std::endl;
	std::cout<<count_vertices<<" vertices and " <<count_edges<<" edges"<<std::endl;
	
	std::string filename("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/libBGLgeom/data/G1.vtp");
	
	writer_vtk<Graph,2> W1(filename.c_str());
	W1.export_vtp(G1);
	

/*
 	create_graph(G2,R2);
 	
	//initialize writer pts/vtk passing output filename
	
	count_vertices = 0;
	count_edges = 0;

	for (std::tie(e_it, e_end) = boost::edges(G2); e_it != e_end; ++e_it) //for loop on al the edges already in the graph
		++count_edges;
		
	for (std::tie(v_it, v_end) = boost::vertices(G2); v_it != v_end; ++v_it) //for loop on al the edges already in the graph
		++count_vertices;
	
	std::cout << "________________ FINAL COUNT ________________"<<std::endl;
	std::cout<<count_vertices<<" vertices and " <<count_edges<<" edges"<<std::endl;
	
	filename = "/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/libBGLgeom/data/G2.vtp";
	
	writer_vtk<Graph,2> W2(filename.c_str());
	W2.export_vtp(G2);
*/
	
	return 0;
};
