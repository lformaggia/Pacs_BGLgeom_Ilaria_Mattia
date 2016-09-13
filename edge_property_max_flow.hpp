#ifndef HH_EDGE_PROPERTY_MAX_FLOW_HH
#define HH_EDGE_PROPERTY_MAX_FLOW_HH

# include<boost/graph/adjacency_list.hpp>


template<typename Graph>
struct edge_prop_max_flow_t{
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
	double capacity;
	double residual_capacity;  
	edge_descriptor edge_reverse;	
};

#endif 
