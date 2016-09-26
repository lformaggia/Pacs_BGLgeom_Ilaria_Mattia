
#include <iostream>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "generic_point.hpp"
#include "Zunino_edge_property.hpp"

using namespace boost;

int main(){
	
	typedef adjacency_list<vecS,vecS,directedS,no_property,double> Graph;

	Graph G;
	Graph* G_ptr = &G;	
	
	graph_traits<Graph>::vertex_descriptor a,b,c,d;

	a = add_vertex(*G_ptr);
	b = add_vertex(*G_ptr);
	c = add_vertex(*G_ptr);
	d = add_vertex(*G_ptr);

	graph_traits<Graph>::edge_descriptor e;
	bool inserted;	

	std::tie(e, inserted) = add_edge(a,b,*G_ptr);
	(*G_ptr)[e] = 5.5;
	std::tie(e, inserted) = add_edge(b,c,*G_ptr);
	(*G_ptr)[e] = 41.5;
	std::tie(e, inserted) = add_edge(c,d,*G_ptr);
	(*G_ptr)[e] = 43.5;
	std::tie(e, inserted) = add_edge(d,a,*G_ptr);
	(*G_ptr)[e] = 40.5;
	

	graph_traits<Graph>::edge_iterator e_it, e_end;
	

	for(std::tie(e_it, e_end) = edges(*G_ptr); e_it != e_end; ++e_it){
		std::cout << *e_it << " : " << (*G_ptr)[*e_it] << std::endl;
	}

	return 0;
};
