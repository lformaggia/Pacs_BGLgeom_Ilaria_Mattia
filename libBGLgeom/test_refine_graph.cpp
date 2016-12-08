#include<utility>

#include <boost/graph/adjacency_list.hpp>

#include"data_structure.hpp"
#include"linear_edge.hpp"
#include"intersection_layer.hpp"
#include"intersections2D.hpp"

using namespace boost; 

using Graph = adjacency_list<vecS,vecS,directedS,BGLgeom::Vertex_base_property<2>,BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2> >;
using point = BGLgeom::point<2>;
using line = BGLgeom::linear_edge<2>;

using Edge_iter = BGLgeom::Edge_iter<Graph>;
using Vertex_iter = BGLgeom::Vertex_iter<Graph>;
using Vertex_desc = BGLgeom::Vertex_desc<Graph>;
using Edge_desc = BGLgeom::Edge_desc<Graph>;

void create_graph(Graph & G, const std::vector<line> & Lvect);
Vertex_desc get_vertex_descriptor(const point &P, Graph & G);

int main(){

	//initialize reader R passing input filename
	
	//create a 2D graph and then pass it by reference to the method R.create_graph
	Graph G;
	
	point p1(0.0,0.0);
    point p2(0.0,2.0);
    point p3(2.0,0.0);
    point p4(1.0,3.0);
    point p5(4.0,4.0);
    point p6(3.0,1.0);
    
	line line1(p2,p3);
	line line2(p4,p6);
	line line3(p1,p5);    
 
 	std::vector<line> line_vect{line1,line2,line3};
 	
 	create_graph(G,line_vect);
	//initialize writer pts/vtk passing output filename
	
	return 0;
}


	void create_graph(Graph & G, const std::vector<line> & Lvect){

		Edge_iter e_it, e_end;
		BGLgeom::Intersection intobj_tmp; //object of class Intersection
		
		bool edge_alone; //if the edge doesn't intersect any of the other edges we need to create it, otherwise it will be created step by step
		
		std::vector<BGLgeom::Int_layer> intvect;

	//	while(!eof){
		for(const line & L : Lvect){
			//line = getline;
			Vertex_desc src = get_vertex_descriptor(L.get_source(), G); //inserts a new vertex and returns the vertex desc or simply returns an already existing vertex desc in the case the coordinates match
			Vertex_desc tgt = get_vertex_descriptor(L.get_target(), G);
			
			edge_alone = true;
			intvect.clear(); // at each line I have to start with the empty vector
			
			for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){ //for loop on al the edges already in the graph
				intobj_tmp = BGLgeom::compute_intersection(L, G[*e_it].geometry); //in intersection info about the type of intersection are stored
				BGLgeom::Int_layer intobj(intobj_tmp, *e_it); // this is the structure I' going to use
				
				if(intobj.intersect == true){ //otherwise we don't do anything
					edge_alone = false;
					intvect.push_back(intobj);
				}	
			} //for
			
			/*if(edge_alone)
				add_edge(SRC,TGT, G);
			else{
				 //order intvect in decreasing or decreasing order based on the relative position of src and tgt and on the first elem in the intersection vector
				if(intvect.size()==1){
					refine_grah(G, SRC, TGT, intvect[0], newinserted_edges, toremove_edges);
				} 
				else{
					intvect.sort(begin,end,sort_fun(SRC,TGT));
					togli_common extreme(intvect);
					refine_grah(G, SRC, intvect[1].intpoint[0], intvect[0], newinserted_edges, toremove_edges); //collego src alla prima intersection
					for(int i=1; i<intvect.size()-1; ++i)
						refine_grah(G, intvect[i-1].intpoint[intvect[i-1].size()-1], intvect[i+1].intpoint[0], intvect[i], newinserted_edges, toremove_edges);	//ovviamente migliorare sintassi ora illeggibile
					refine_grah(G, intvect[N-2].intpoint[intvect[i-1].size()-1], TGT, intvect[N-1], newinserted_edges, toremove_edges); //collego ultima intersection to tgt
					remove_edges(toremove_edges);											
				}
			
			}*/
		
		}//while

	} //function


	Vertex_desc get_vertex_descriptor(const point &P, Graph & G){
		// check if in the graph there is already a vertex descriptor with the same coordinates
		Vertex_iter v_it, v_end;
		for(std::tie(v_it,v_end)=boost::vertices(G); v_it != v_end; ++v_it){
			if(P == G[*v_it].coordinates)
				return *v_it;
		}

		//if this part is executed it means that there's no vertex desciptor matching the new coordinates
		Vertex_desc v = boost::add_vertex(G);
		G[v].coordinates = P;	
		return v;
	}

