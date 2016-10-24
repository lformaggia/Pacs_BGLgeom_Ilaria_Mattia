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

#include <boost/graph/graph_trais.hpp>

//#include "new_reader_Formaggia.hpp"

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


template <typename Graph>
void give_vertex_properties(Graph & G, Vertex_desc const& v, data_from_line const& D){

}	//give_vertex_properties

#endif	//HH_GRAPH_BUILDER_HH
