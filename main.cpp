#include <iostream>
#include<string>
#include<boost/graph/adjacency_list.hpp>

#include"io_graph.hpp"
#include"generic_point.hpp"
#include "maximum_flow.hpp"

using namespace boost;

int main(){

	 typedef adjacency_list<vecS,vecS,directedS,point<3>,edge_prop_t> Graph;
	 typedef graph_traits<Graph> Traits;
	 Graph G;
	 read_zunino_old_format<Graph, point<3>> (G , "rat98p.txt");
	 
	 //chiamiamo:
	 double out_max_flow;
	 Traits::vertex_descriptor s = 90;	//da leggere da qualche parte in qualche modo.
	 Traits::vertex_descriptor t = 92;
	 out_max_flow = maximum_flow<Graph>(G,s,t);
	 
	 std::cout << "Abbiamo ottenuto: " << out_max_flow << std::endl;
	 
	return 0;
}
