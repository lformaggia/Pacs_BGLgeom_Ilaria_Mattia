#include<string>
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>
#include<set>
#include<utility>
#include<boost/graph/adjacency_list.hpp>

#include"generic_point.hpp"
#include"edge_property.hpp"
#include"io_graph.hpp"

/*!
  @brief Reading from file with list of vertex_set and edges and associated properties
  
  @details
 */
 
 using namespace boost;
 typedef adjacency_list<vecS,vecS,directedS,point<3>,edge_prop_t> Graph;
 
int main()
{  
  /* I want to read from a file where data 
     is written as
     
     line1: description
     line2: description
     from line 3: no - source_v - target_v - diameter - length - source_coord - target_coord
  */
  std::ifstream file("rattm93a.txt");
  
  point<3> SRC,TGT; //! they will store vertex coordinates
  
  int edge_num, src, tgt; // they will read the first 3 numbers of each line 
  double diam, length; // they will read the remaining 8
  
  Graph G; 

  // ignore the first two lines of the file
  std::string dummyLine;
  getline(file, dummyLine);
  getline(file, dummyLine);
  
  // Until I reach end of file
  while (!file.fail() && !file.eof()){
    std::string s;
    getline(file,s); // read the the whole line
    if(s.empty()) continue; // empty line
    std::istringstream tmp(s); // build an input sstream.
    tmp>>edge_num>>src>>tgt>>diam>>length>>SRC>>TGT; // read from the input stream
    if(!tmp.fail()){
    	//! initialize_graph
    	initialize_graph<Graph, point<3> >(src, tgt, G, diam, length, SRC, TGT);
    }
  }

  graph_traits<Graph>::vertex_iterator vb, ve;
  for(tie(vb,ve) = vertices(G); vb != ve; ++vb)
  	std::cout << *vb << std::endl;
  
  //Plot degli archi:
  graph_traits<Graph>::edge_iterator ebegin, eend;
  for( tie(ebegin, eend) = edges(G); ebegin != eend; ebegin++)
  	std::cout << *ebegin << std::endl;
  	
return 0;
}
