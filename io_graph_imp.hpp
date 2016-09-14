/*!
* \file io_graph_imp.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sep 14, 2016
* \brief Declaration of functions related to input and output of the graph
*/

#ifndef HH_IO_GRAPH_IMP_HH
#define HH_IO_GRAPH_IMP_HH

#include<tuple>
#include<boost/graph/adjacency_list.hpp>
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<set>
#include<utility>

#include"generic_point.hpp"
#include"edge_property.hpp"
#include"io_graph.hpp"


/*!
* \brief Inserts a new edge and the corresponding vertices (only if they aren't already in the graph).
*
* The funcion inserts the edge (src,tgt) with its properties, then it checks whether src and tgt are in the graph: if not 
* it associates to src its property, contained in SRC, and then does the same with tgt and TGT.
*/
template<typename Graph, typename Point>
void fill_graph(const int src, const int tgt, Graph & G, double diam, double length, Point const & SRC, Point const & TGT){
	
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
	
	std::set<int> vertex_set; 		/*< using a set, we can easily check if a vertex has already been added */
	bool inserted; 				

	std::pair<std::set<int>::iterator, bool> set_inserter;
	std::set<int>::iterator it;
	
	edge_descriptor e;
	bool edge_inserted;	
	
	// create edge (src, tgt)
	std::tie(e, edge_inserted) = boost::add_edge(src, tgt, G);
	
	G[e].capacity = diam;

	G[e].length = length;
	
	// Inserting property of the source vertex, if the vertex wasn't inserted before
	set_inserter = vertex_set.insert(src);
	if(set_inserter.second)
	 	G[src]=SRC;

	// Inserting property of the target vertex, if the vertex wasn't inserted before		
	set_inserter = vertex_set.insert(tgt);
	if(set_inserter.second)
		G[tgt]=TGT;
}	//initialize_graph


	
/*!
* \brief Reads data about the graph from the input file given by professor Zunino
*
* The funcitions reads from a file where data is written as: \n
* line1: description of file \n
* line2: description of file \n
* from line 3: line_number - source - target - diameter - length - source_coord - target_coord 
* 
*/		
template<typename Graph, typename Point>
void read_zunino_old_format(Graph & G, std::string file_name){ 
  
  std::ifstream file(file_name.c_str());


  Point SRC,TGT; 				// they will store vertex coordinates	  
  int edge_num, src, tgt; 			// they will read the first 3 numbers of each line 
  double diam, length; 				// they will read the remaining 8

  // ignore the first two lines of the file
  std::string dummyLine;
  std::getline(file, dummyLine);
  std::getline(file, dummyLine);
  
  // Until I reach end of file
  while (!file.fail() && !file.eof()){
    std::string s;
    std::getline(file,s); 						// read the the whole line
    if(s.empty()) continue; 						// empty line
    std::istringstream tmp(s); 						// build an input sstream.
    tmp>>edge_num>>src>>tgt>>diam>>length>>SRC>>TGT;			// read from the input stream
    if(!tmp.fail()){
    	fill_graph<Graph, point<3> >(src, tgt, G, diam, length, SRC, TGT);
    }
  }	// while
  
  //Eliminiamo il vertice zero perché nei file di input non c'è, si parte dal vertice 1:
  //remove_vertex(0, G);

  //Plot dei vertici:
  typename boost::graph_traits<Graph>::vertex_iterator vb, ve;
  for(tie(vb,ve) = vertices(G); vb != ve; ++vb)
  	std::cout << *vb << std::endl;
  
  //Plot degli archi:
  typename boost::graph_traits<Graph>::edge_iterator ebegin, eend;
  for( tie(ebegin, eend) = edges(G); ebegin != eend; ebegin++)
  	std::cout << *ebegin << std::endl;
}	//read_zunino_old_format
#endif
