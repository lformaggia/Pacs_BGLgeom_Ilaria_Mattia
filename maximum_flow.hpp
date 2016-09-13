template<typename Graph>
long max_flow(Graph const &G);
/*
	create a FlowGraph, made of generic_point as vertex_property and edge_property_max_flow as edge_property
	copy in Flow Graph (which has "bidirectional" edges) the properties of Graph and add residual_capacity and rev_graph
	run the algorithm max_flow on the flowGraph
	print in a file residual capacities (or how can we save this data once we delete FlowGraph????)
	output max flow
*/

