#include <iostream>
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

#include "io_graph.hpp"
#include "generic_point.hpp"
#include "maximum_flow.hpp"

using namespace boost;

int main(){

	 typedef adjacency_list<vecS,vecS,directedS,point<3>,edge_prop_t> Graph;
	 typedef graph_traits<Graph> Traits;
	 typedef Traits::edge_descriptor Edge_Descriptor;
	 typedef Traits::edge_iterator Edge_Iterator;
	 
	 Graph G;
	 read_zunino_old_format<Graph, point<3>> (G , "rattm93a.txt");
	 
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
	 
	 
	return 0;
}
