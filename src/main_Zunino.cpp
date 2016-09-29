/*!
	@file main_Zunino.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Source code for Zunino example.
	@detail
*/

#include <iostream>
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

//#include "io_graph.hpp"
#include "generic_point.hpp"
#include "maximum_flow.hpp"
#include "compute_euclidean_distance.hpp"
//#include "topological_distance.hpp"
#include "reader_Zunino_class.hpp"
#include "Zunino_edge_property.hpp"
#include "disjoint_components.hpp"

using namespace boost;

int main(){

	 typedef adjacency_list<vecS,vecS,directedS,point<3>,Zunino_edge_property_t> Graph;
	 typedef graph_traits<Graph> Traits;
	 typedef Traits::edge_descriptor Edge_Descriptor;
	 typedef Traits::edge_iterator Edge_Iterator;
	 typedef Traits::vertex_descriptor Vertex_desc;
	 
	 
	 std::string filename("../data/rattm93a.txt");
	 unsigned int dummy_lines = 2;
	 
	 Graph G;
	 reader_Zunino<Graph> R(G, filename, 2);
	 
	 R.read_input_file();
	 
	 //G = R.get_graph();
	 //R.release_graph();		//brutto però... coi puntatori?
	 
	 
	 //read_zunino_old_format<Graph> (G , "../data/rattm93a.txt");
	 
	 //chiamiamo:
	 std::map<Edge_Descriptor, double> out_residual_capacity;
	 double out_max_flow;
	 Traits::vertex_descriptor s = 2;	//da leggere da qualche parte in qualche modo.
	 Traits::vertex_descriptor t = 14;
	 out_max_flow = maximum_flow<Graph, Edge_Descriptor>(G,s,t,out_residual_capacity);
	 
	 std::cout << "Abbiamo ottenuto: " << out_max_flow << std::endl;
	 
	 Edge_Iterator e_it, e_end;  // print residual capacities
	 std::cout << "Residual capacities:" << std::endl;
	 for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it){
	 	std::cout << *e_it << " " << out_residual_capacity[*e_it] << std::endl;
	 }
	 
	 double d12 = compute_euclidean_distance<Graph>(1,2,G);
	 double d45 = compute_euclidean_distance<Graph>(4,5,G);
	 double d13 = compute_euclidean_distance<Graph>(1,3,G);
	 
	 std::cout << "eudlidean distance 1_2 = " << d12 << std::endl;
	 std::cout << "eudlidean distance 4_5 = " << d45 << std::endl;
	 std::cout << "eudlidean distance 1_3 = " << d13 << std::endl;
	 
	 std::map<Vertex_desc, int> rank = disjoint_components(G);
	 
	 std::cout << "Prova disjoint:" << std::endl;
	 for(std::map<Vertex_desc,int>::iterator map_it = rank.begin(); map_it != rank.end(); ++map_it){
	 	std::cout << (*map_it).first << " is in subgraph " << (*map_it).second << std::endl;
	 }
	 
	 
	 
	return 0;
}
