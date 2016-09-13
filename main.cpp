#include<string>
#include<boost/graph/adjacency_list.hpp>
#include"io_graph.hpp"
#include"generic_point.hpp"

#include"edge_property_max_flow.hpp"

using namespace boost;

int main(){
	 typedef adjacency_list<vecS,vecS,directedS,point<3>,edge_prop_t> Graph;
	 Graph G;
	 read_zunino_old_format<Graph, point<3> >( G , "rattm93a.txt"); 
return 0;
}
