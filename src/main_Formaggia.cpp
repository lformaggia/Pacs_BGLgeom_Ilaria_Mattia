/*======================================================================
                        "[nome_progetto]"
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
#include <boost/graph/adjacency_list.hpp>

#include "io_graph.hpp"
#include "Forma_vertex_property.hpp"
#include "Forma_edge_property.hpp"

using namespace boost;

int main(){

	typedef adjacency_list <vecS, vecS, directedS, Forma_vertex_property_t, Forma_edge_property_t> Graph;
	typedef graph_traits<Graph> Traits;
	
	Graph G;
	
	read_Formaggia_format<Graph> (G, "/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/data/input_Formaggia_prova.dat");
	
	Traits::edge_iterator e_it, e_end;
	Traits::vertex_iterator v_it, v_end;
	for( std::tie(e_it, e_end) = edges(G); e_it != e_end; e_it++){
		std::cout << "Arco " << *e_it << ": frattura " << G[*e_it].frac_num << std::endl;
	}
	
	for( std::tie(v_it, v_end) = vertices(G); v_it != v_end; v_it++){
		std::cout << "Vertice " << *v_it << "   " << G[*v_it].coord << std::endl;
	}
	return 0;
};
