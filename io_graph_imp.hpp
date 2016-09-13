#ifndef HH_IO_GRAPH_IMP_HH
#define HH_IO_GRAPH_IMP_HH

#include<tuple>
#include<boost/graph/adjacency_list.hpp>
#include"edge_property.hpp"
#include<string>
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>
#include<set>
#include<utility>

#include"generic_point.hpp"
#include"edge_property.hpp"
#include"io_graph.hpp"

template<typename Graph, typename Point>
void initialize_graph(const int src, const int tgt, Graph & G, double diam, double length, Point const & SRC, Point const & TGT){
	
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
	
	std::set<int> vertex_set; 		// it's a set in order to avoid multiple additions of the same vertex
	bool inserted; 				//if FALSE the vertex I'm trying to add was already in the set 

	std::pair<std::set<int>::iterator, bool> set_inserter;
	std::set<int>::iterator it;
	
	edge_descriptor e;
	bool edge_inserted;	
	
	//! create edge (src, tgt)
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
	
		
template<typename Graph, typename Point>
void read_zunino_old_format(Graph & G, std::string file_name){ 

  /* I want to read from a file where data 
     is written as
     
     line1: description
     line2: description
     from line 3: no - source_v - target_v - diameter - length - source_coord - target_coord
  */
  
  std::ifstream file(file_name.c_str());


  Point SRC,TGT; 				//! they will store vertex coordinates	  
  int edge_num, src, tgt; 			// they will read the first 3 numbers of each line 
  double diam, length; 				// they will read the remaining 8

  // ignore the first two lines of the file
  std::string dummyLine;
  std::getline(file, dummyLine);
  std::getline(file, dummyLine);
  
  // Until I reach end of file
  while (!file.fail() && !file.eof()){
    std::string s;
    std::getline(file,s); 										// read the the whole line
    if(s.empty()) continue; 									// empty line
    std::istringstream tmp(s); 									// build an input sstream.
    tmp>>edge_num>>src>>tgt>>diam>>length>>SRC>>TGT;			 // read from the input stream
    if(!tmp.fail()){
    	//! initialize_graph
    	initialize_graph<Graph, point<3> >(src, tgt, G, diam, length, SRC, TGT);
    }
  }	// while

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
