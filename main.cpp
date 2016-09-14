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
	 Graph G;
	 read_zunino_old_format<Graph, point<3>> (G , "rattm93a.txt");
	 
	 //chiamiamo:
	 std::vector<double> out_residual_capacity(num_edges(G),0.0);
	 double out_max_flow;
	 Traits::vertex_descriptor s = 2;	//da leggere da qualche parte in qualche modo.
	 Traits::vertex_descriptor t = 14;
	 out_max_flow = maximum_flow<Graph>(G,s,t,out_residual_capacity);
	 
	 std::cout << "Abbiamo ottenuto: " << out_max_flow << std::endl;
	 	
	 for(std::size_t j = 0; j < out_residual_capacity.size(); j++)
		std::cout << "Capacità residua: " << out_residual_capacity[j] << std::endl;
	 
	return 0;
}
