/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
* \file io_graph_imp.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sep 14, 2016
* \brief Declaration of functions related to input and output of the graph
*/

#ifndef HH_IO_GRAPH_IMP_HH
#define HH_IO_GRAPH_IMP_HH

#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <set>
#include <utility>

#include "generic_point.hpp"
#include "edge_property.hpp"
#include "io_graph.hpp"
#include "Forma_vertex_property.hpp"
#include "Forma_edge_property.hpp"
	
/*!
* \brief Reads data about the graph from the input file given by professor Zunino
*
* The funcitions reads from a file where data is written as: \n
* line1: description of file \n
* line2: description of file \n
* from line 3: line_number - source - target - diameter - length - source_coord - target_coord 
* 
*/		
template<typename Graph, typename Point>			//eventualmente si può togliere Point come template parameter e definirli dentro
void read_zunino_old_format(Graph & G, std::string file_name){ 
	
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
	
	std::ifstream file(file_name.c_str());
	
	// variables for storing data
	Point SRC,TGT; 													// they will store vertex coordinates	  
	int src, tgt; 													// they will read source and target of eah edge
	int edge_num;													// dummy variable;
	double diam, length; 											// they will properties associated to edges

	// ignore the first two lines of the file
	std::string dummyLine;
	std::getline(file, dummyLine);
	std::getline(file, dummyLine);
	
	// Utilities for reading and constructing the Graph
	std::set<int> vertex_set; 		/*< using a set, we can easily check if a vertex has already been added */
	std::pair<std::set<int>::iterator, bool> set_inserter;
	std::string s;			
	edge_descriptor e;
	bool edge_inserted;
	
	while (!file.fail() && !file.eof()){
	    std::getline(file,s); 													// reading the the whole line
	    if(s.empty())
	    	continue; 															// empty line
	    std::istringstream tmp(s); 												// build an input sstream.
	    tmp >> edge_num >> src >> tgt >> diam >> length >> SRC >> TGT;			// reading data from the istringstream
	    if(!tmp.fail()){			
			// create edge (src, tgt)
			std::tie(e, edge_inserted) = boost::add_edge(src, tgt, G);			
			G[e].capacity = diam;
			G[e].length = length;
					
			// Inserting property of the source vertex, if the vertex wasn't inserted before
			set_inserter = vertex_set.insert(src);
			if(set_inserter.second)
			 	G[src] = SRC;
			 	
			// Inserting property of the target vertex, if the vertex wasn't inserted before		
			set_inserter = vertex_set.insert(tgt);
			if(set_inserter.second)
				G[tgt] = TGT;
    }	//if(!tmp.fail())
  }	// while
    
	//Eliminiamo il vertice zero perché nei file di input non c'è, si parte dal vertice 1:
	//remove_vertex(0, G);

	//Plot dei vertici:
	typename boost::graph_traits<Graph>::vertex_iterator vb, ve;
	for( tie(vb,ve) = vertices(G); vb != ve; ++vb)
		std::cout << *vb << std::endl;

	//Plot degli archi:
	typename boost::graph_traits<Graph>::edge_iterator ebegin, eend;
	for( tie(ebegin, eend) = edges(G); ebegin != eend; ebegin++)
		std::cout << *ebegin << std::endl;
}	//read_zunino_old_format

/*
	@brief Reads data from input file with Formaggia's format.
	
	@detail Until now the input file is:\n
	- first three lines are dummy\n
	- then there are three columns: fracture number, coordinates of origin node, coordinates of target node
*/


template <typename Graph>
void read_Formaggia_format(Graph & G, std::string file_name){
	
	typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;

	std::ifstream file(file_name.c_str());
	
	//variables for storing data:
	unsigned int frac_number;
	point<2> SRC, TGT;
	
	//Ignoring the first lines:
	std::string dummy_line;
	std::getline(file, dummy_line);
	std::getline(file, dummy_line);
	std::getline(file, dummy_line);
	
	//Utilities for reading and constructing the Graph:
	std::string s;
	Edge_desc e;
	bool inserted;
	//creo due vertex_descriptor (non li ho ancora, quindi poi ci pensa lui ad indicizzarli) (a meno che ho già un indice in ingresso, nel qual caso bisognerebbe creare un id per ogni vertice per tenere traccia)
	Vertex_desc u,v;
	
	//Reading:
	while(!file.fail() && !file.eof()){
		std::getline(file, s);
		if(s.empty())
			continue;
		std::istringstream temp(s);
		//Leggo i vari dati (ogni utente qui metterà il suo)
		temp >> frac_number >> SRC >> TGT;
		if(!temp.fail()){
			//Qui ci va un altro if per capire se si intersecano. Se no, si procede come sotto, se sì, bisogna spezzare un arco, inserire due vertici in più nelle intersezioni, e riattaccare tutti gli archi.
			
			//creo l'arco
			std::tie(e, inserted) = boost::add_edge(u, v, G);		//PROBLEMA QUI!!! sembra che non gli piaccia che assegno come vertex descriptor u e v perché li ho creati dentro la funzione, e dopo uscendo verranno distrutti.
			//Assegno le proprietà.
			G[u].coord = SRC;		//G[u] and G[v] are point<2>
			G[v].coord = TGT;
			G[u].is_external = true;		//o forse bisogna fare un ciclo alla fine della costruzione per vedere quali nodi hanno grado 1
			G[v].is_external = true;
			G[e].frac_num = frac_number;
			
		}	//if		
	}	//while
};	//read_Formaggia_format
#endif
