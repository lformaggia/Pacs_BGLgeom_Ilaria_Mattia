#include<string>
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>
#include<set>
#include<utility>
/*!
  @brief Reading from file with list of vertices and edges and associated properties
  
  @details
 */
int main()
{  
  /* A different use of a string stream.
     I want to read from a file where data 
     is written as
     
     line1: description
     line2: description
     from line 3: no - source_v - target_v - diameter - length - source_coord - target_coord
  */
  std::ifstream file("rattm93a.txt");
  
  std::set<int> vertices; // it's a set in order to avoid multiple additions of the same vertex
  bool inserted; //if FALSE the vertex I'm trying to add was already in the set 
  std::pair<std::set<int>::iterator, bool> set_inserter;
  std::set<int>::iterator it;
  
  std::vector<std::pair<int, int> > edges; // or better a set to check for multigraph?
  
  int edge_num, src, tgt; // they will read the first 4 numbers of each line 
  double diam, length, x_src, y_src, z_src, x_tgt, y_tgt, z_tgt; // they will read the remaining 8
  
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
    tmp>>edge_num>>src>>tgt>>diam>>length>>x_src>>y_src>>z_src>>x_tgt>>y_tgt>>z_tgt; // read from the input stream
    if(!tmp.fail()){
	set_inserter = vertices.insert(src);
	if(set_inserter.second)
		{
		// [create the node and store as property its coordinates]
		}
	set_inserter = vertices.insert(tgt);
	if(set_inserter.second)
		{
		// [create the node and store as property its coordinates]
		}
	// [create the new edge src -> tgt and store as property diam and legth]
	edges.push_back(std::make_pair(src,tgt));
    }
  }
  std::cout<<"I've read "<<vertices.size()<<" vertices and "<<edges.size()<<" edges from file rattm93a.txt:"<<std::endl;
  std::cout<<"Vertices:"<<std::endl;
  for (it = vertices.begin(); it != vertices.end(); ++it) 
  	std::cout<<*it<<std::endl;
  for (unsigned int i=0; i<edges.size(); ++i)
  	std::cout<<"("<<edges[i].first<<","<<edges[i].second<<")"<<std::endl;
  	
return 0;
}
