int main(){

	//initialize reader R passing input filename
	
	//create a 2D graph and then pass it by reference to the method R.create_graph

	//initialize writer pts/vtk passing output filename
}


create_graph(Graph G){

	Edge_iter e_it, e_end;
	BGLgeom::Intersection_t intobj; //object of class Intersection_t
	
	bool edge_alone; //if the edge doesn't intersect any of the other edges we need to create it, otherwise it will be created step by step
	
	std::vector<BGLgeom::Intersection_t> intvect;

	while(!eof){
		line = getline;
		extract_source_target(&SRC, &TGT) // private method inside the reader class
		edge_alone = true;
		intvect.clear(); // at each line I have to start with the empty vector
		toremove_edges.clear();
		newinserted_edges.clear();
		
		for (std::pair(e_it, e_end) = boost::edges(G), e_it != e_end; ++e_it){ //for loop on al the edges already in the graph
			intobj = BGLgeom::are_intersected(SRC, TGT, e_it); //in intersection info about the type of intersection are stored
			if(intobj.intersection_exists == true){ //otherwise we don't do anything
				edge_alone = false;
				intvect.push_back(intobj);
			}	
		} //for
		
		if(edge_alone)
			add_edge(SRC,TGT, G);
		else{
			 //order intvect in decreasing or decreasing order based on the relative position of src and tgt and on the first elem in the intersection vector
			if(intvect.size()==1){
				refine_grah(G, SRC, TGT, intvect[0], newinserted_edges, toremove_edges);
			} 
			else{
				intvect.sort(begin,end,sort_fun(SRC,TGT));
				refine_grah(G, SRC, intvect[1].intpoint[0], intvect[0], newinserted_edges, toremove_edges); //collego src alla prima intersection
				for(int i=1; i<intvect.size()-1; ++i)
					refine_grah(G, intvect[i-1].intpoint[intvect[i-1].size()-1], intvect[i+1].intpoint[0], intvect[i], newinserted_edges, toremove_edges);	//ovviamente migliorare sintassi ora illeggibile
				refine_grah(G, intvect[N-2].intpoint[intvect[i-1].size()-1], TGT, intvect[N-1], newinserted_edges, toremove_edges); //collego ultima intersection to tgt
				remove_edges(toremove_edges);											
			}
		
		}
	
	}//while

} //function
