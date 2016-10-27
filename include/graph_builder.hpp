/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file graph_builder.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Utilities to build a graph
*/

#ifndef HH_GRAPH_BUILDER_HH
#define HH_GRAPH_BUILDER_HH

#include <vector>
#include <boost/graph/graph_traits.hpp>

#include "generic_point.hpp"

/*
template <typename Graph>
class graph_builder{
	public:
		using Vertex_desc = boost::graph_traits<Graph>::vertex_descriptor;
		using Edge_desc = boost::graph_traits<Graph>::edge_descriptor;
	
		give_vertex_properties();	// = 0;
		give_edge_properties();		// = 0;
		void new_vertex();			//tecnicamente non ha senso, per aggiungere un vertex in senso topologico c'è già add_vertex
		new_edge();					//idem
		refine_graph();				// questa dovrei poterla definire io, ma anche qui ci sono le info sugli archi che è un casino
	private:
		Vertex_desc src, tgt;
		Edge_desc e;
};
*/

// Provo a fare solo non_member functions

//using Vertex_desc = boost::graph_traits<Graph>::vertex_descriptor;
//using Edge_desc = boost::graph_traits<Graph>::edge_descriptor;

//! Giving to source node v all properties through assigning the Source_data_structure
// The reference " & D" is correct? mmm...
template <typename Graph, typename Source_data_structure>
void give_source_properties	(Source_data_structure const& D,
							typename boost::graph_traits<Graph>::vertex_descriptor const& v,
							Graph & G){
	G[v] = D;
}	//give_source_properties

//! Giving to target node v all properties through assigning the Target_data_structure
template <typename Graph, typename Target_data_structure>
void give_target_properties	(Target_data_structure const& D,
							typename boost::graph_traits<Graph>::vertex_descriptor const& v,
							Graph & G){
	G[v] = D;
}	//give_target_properties

//! Giving to edge e all properties through assigning the Edge_data_structure
template <typename Graph, typename Edge_data_structure>
void give_edge_properties	(Edge_data_structure const& D,
							typename boost::graph_traits<Graph>::edge_descriptor const& e,
							Graph & G){
							/*
	if(D.edge_geometry.cattivo())
	src = source(e,g)
	tgt = target(e,g)
	SRC = G[src].coord
	TGT = G[tgt].coord
	D.edge_geometry.set(SRC,TGT)*/
	G[e] = D;
}	//give_edge_properties

/*!
	@brief Creates an edge giving the right properties to source and target vertex and to the edge itself
	@detail Topological information, such as source and target vertices, has to be passed as
			standard parameters of the function
*/
template <typename Graph, typename Vertex_data_structure, typename Edge_data_structure>
void create_edge(Graph & G,
				typename boost::graph_traits<Graph>::vertex_descriptor src,
				typename boost::graph_traits<Graph>::vertex_descriptor tgt,
				
				)


template <typename Graph, typename Vertex_data_structure, typename Edge_data_structure,
		 typename Intersections_container = std::vector<BGLgeom::point<2>> >
void refine_graph	(Graph & G,
					//Vertex_data_structure & V,
					//Edge_data_structure & E,
					Intersections_container const& I,
					typename boost::graph_traits<Graph>::edge_descriptor edge1,
					typename boost::graph_traits<Graph>::edge_descriptor edge2){
	
	using Vertex_desc = typename boost::graph_traits<Graph>::vertex_descriptor;
	using Edge_desc = typename boost::graph_traits<Graph>::edge_descriptor;
	
	Vertex_desc src1, tgt1, src2, tgt2;
	src1 = boost::source(edge1, G);
	tgt1 = boost::target(edge1, G);
	src2 = boost::source(edge2, G);
	tgt2 = boost::target(edge2, G);
	
}	//refine_graph

#endif	//HH_GRAPH_BUILDER_HH
