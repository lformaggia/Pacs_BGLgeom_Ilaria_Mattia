#include<iostream>

#include"helper_functions.hpp"
using namespace BGLgeom;

namespace BGLgeom{

	Vertex_d get_vertex_descriptor(const point2 &P, Graph & G){
		// check if in the graph there is already a vertex descriptor with the same coordinates
		Vertex_it v_it, v_end;
		for(std::tie(v_it,v_end)=boost::vertices(G); v_it != v_end; ++v_it){
			if(P == G[*v_it].coordinates)
				return *v_it;
		}

		//if this part is executed it means that there's no vertex desciptor matching the new coordinates
		Vertex_d v = boost::add_vertex(G);
		G[v].coordinates = P;	
		return v;
	}	
	

	void create_graph(Graph & G, const std::vector<line> & Lvect){

		Edge_it e_it, e_end;
		BGLgeom::Intersection intobj_tmp; //object of class Intersection
		
		bool edge_alone; //if the edge doesn't intersect any of the other edges we need to create it, otherwise it will be created step by step
		
		std::vector<BGLgeom::Int_layer<Graph>> intvect;
		
		int count;

	//	while(!eof){
		for(const line & L : Lvect){
		
			std::cout<<"-------------------STARTING A NEW LINE-------------------"<<std::endl;
			//line = getline;
			Vertex_d src = get_vertex_descriptor(L.get_source(), G); //inserts a new vertex and returns the vertex desc or simply returns an already existing vertex desc in the case the coordinates match
			Vertex_d tgt = get_vertex_descriptor(L.get_target(), G);
			
			edge_alone = true;
			intvect.clear(); // at each line I have to start with the empty vector
			
			count = 0;
			
			for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){ //for loop on al the edges already in the graph
				++count;
				
				intobj_tmp = BGLgeom::compute_intersection(G[*e_it].geometry, L); //in intersection info about the type of intersection are stored
				if(intobj_tmp.intersect == true){
					BGLgeom::Int_layer<Graph> intobj(intobj_tmp, *e_it); // this is the structure I'm going to use
					edge_alone = false;
					intvect.push_back(intobj);
					std::cout<<intobj<<std::endl;
				}
				
			}//for
			
			std::cout<<count<<" edges"<<std::endl;
			
			if(edge_alone){
				add_new_edge(src,tgt,G);
			}
			
			else{ // there is at least one intersection		
				Vertex_d next_src;
				Vertex_d current_src = src;
			
				if(intvect.size()==1){
					// if the the type is Overlap_inside both src and tgt are required, so we treat this case separately
					if(intvect[0].how == BGLgeom::intersection_type_new::Overlap_inside){
						BGLgeom::Int_layer<Graph> I = intvect[0];
						
						Vertex_d v1 = boost::source(I.int_edge, G);
						Vertex_d v2 = boost::target(I.int_edge, G);
						boost::remove_edge(I.int_edge,G);
						std::cout<<"Edge removed"<<std::endl;
						add_new_edge(v1,src,G);
						add_new_edge(src,tgt,G);
						Edge_d e = (boost::edge(src,tgt,G)).first;
 						update_edge_properties(e, G);					
						add_new_edge(tgt,v2,G);								
					}
					else{
						refine_graph(G, current_src, intvect[0], next_src);
						current_src = next_src;					
						if(!same_coordinates(current_src, tgt, G))
						add_new_edge(current_src, tgt, G);
					}			
				} 
				else{
				 	//order intvect in decreasing or decreasing order based on the relative position of src and tgt and on the first elem in the intersection vector
				 	if(src < tgt)
						std::sort(intvect.begin(), intvect.end(), asc_order);
					else
						std::sort(intvect.begin(), intvect.end(), desc_order);
					
					std::cout<<"ordered vector"<<std::endl;	
					for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout<<I<<std::endl;
					
					//remove multiple "common_extreme" and "T_old" with same intersection vertex_descriptor: they don't affect the graph so we don't consider them
					auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
					intvect.erase(last, intvect.end());
					
					std::cout<<"Vector without duplicates"<<std::endl;	
					for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout<<I<<std::endl;
					
										
					// now we are ready to refine the graph: 
					// first, we resolve the connection between source and first intersection
					refine_graph(G, src, intvect[0], next_src); //collego src alla prima intersection
					
					for(int i=1; i<intvect.size(); ++i){ //here we resolve all the intermediate intersections
						current_src = next_src;
						refine_graph(G, current_src, intvect[i], next_src);	// graph, current source, current intersection object					
					};
					// Finally we connect the last intersection point with the target
					current_src = next_src;
					if(!same_coordinates(current_src, tgt, G));
						add_new_edge(current_src, tgt, G);									
				}
			
			}
		
		}//while

	}; //create_graph

	
bool asc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2){
	//ordina I1 nel caso ci siano due elementi
	if(I1.int_pts.size()==2 && I1.int_pts[0] > I1.int_pts[1]){
		std::swap(I1.int_pts[0],I1.int_pts[1]);
		I1.swapped_comp = true;
	}
	//ordina I2 nel caso ci siano due elementi
	if(I2.int_pts.size()==2 && I2.int_pts[0] > I2.int_pts[1]){
		std::swap(I2.int_pts[0],I2.int_pts[1]);
		I2.swapped_comp = true;
	}	
	
	return(I1.int_pts[0] < I2.int_pts[0]);
}; //asc_order

bool desc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2){
	//ordina I1 nel caso ci siano due elementi
	if(I1.int_pts.size()==2 && I1.int_pts[0] < I1.int_pts[1]){
		std::swap(I1.int_pts[0],I1.int_pts[1]);
		I1.swapped_comp = true;
	}
	//ordina I2 nel caso ci siano due elementi
	if(I2.int_pts.size()==2 && I1.int_pts[0] < I1.int_pts[1]){
		std::swap(I2.int_pts[0],I2.int_pts[1]);
		I2.swapped_comp = true;
	}	
	return (I1.int_pts[0] > I2.int_pts[0]);
}; //desc_order


bool is_duplicate(const BGLgeom::Int_layer<Graph> & I1, const BGLgeom::Int_layer<Graph> & I2){
	//check if the first of the 2 elements is of type Common_extreme or T_old. If so check if the second one shares with it one intersection point
	if(I1.how == intersection_type_new::Common_extreme || I1.how == intersection_type_new::T_old){
		for(const point2 & P2: I2.int_pts)
			if(I1.int_pts[0] == P2) return true;
	}
	// Do the same as before, inverting I1 and I2
	if(I2.how == intersection_type_new::Common_extreme || I2.how == intersection_type_new::T_old){
		for(const point2 & P1: I1.int_pts)
			if(I2.int_pts[0] == P1) return true;
	}
	return false;
};



void refine_graph(Graph &G, const Vertex_d & src, BGLgeom::Int_layer<Graph> & I, Vertex_d & next_src){
	using int_type = BGLgeom::intersection_type_new;
	int_type T = I.how;
	
	switch(T){
		case int_type::X:
		{
			// create vertex_descriptor for the intersection point
			Vertex_d v = add_new_vertex(I.int_pts.front(), G);
			
			// add edge between src and v
			add_new_edge(src, v, G);
			
			// now we need to cut in two the intersected edge
			cut_old_edge(I.int_edge, v, G);
			
			next_src = v;
			break;
		} 
		
		case int_type::T_new:{
			// create vertex_descriptor for the intersection point
			Vertex_d v = add_new_vertex(I.int_pts.front(), G);
			 
			if(!same_coordinates(src, v, G))
				add_new_edge(src, v, G);
				
			cut_old_edge(I.int_edge, v, G);
			next_src = v;
			break;
		}
		
		case int_type::T_old:{
			// get the vertex descriptor of the extreme involved in the intersection
			Vertex_d v;
			if(I.intersected_extreme == 0) //first extreme, i.e the source
				v = boost::source(I.int_edge, G);
			else
				v = boost::target(I.int_edge, G);
				
			add_new_edge(src, v, G);
			
			next_src = v;
			break;
		}
		
		case int_type::Overlap_outside:{
			Vertex_d v1 = boost::source(I.int_edge, G);
			Vertex_d v2 = boost::target(I.int_edge, G);
			
			if(!I.swapped_comp){ // if the components haven't been inverted, the nearest to src is the source, otherwise the target
				add_new_edge(src, v1, G);
				next_src = v2;				
			}
			else{
				add_new_edge(src, v2, G);
				next_src = v1;			
			}
				
 			update_edge_properties(I.int_edge, G);
 			break;
		}
		
		case int_type::Overlap:{
			Vertex_d v1;
			Vertex_d v2;

			if(!swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}
			
			if(I.intersected_extreme==0){//it means that src is outside and tgt inside
				add_new_edge(src,v1,G);
				//recover the vertex_descriptor having the coordinates of the inside point of intersection (which is always the second in vector int_pts)
				Vertex_d v = get_vertex_descriptor(I.int_pts[1], G);
				add_new_edge(v1,v,G);
				update_edge_properties((boost::edge(v1,v)).first, G);
				add_new_edge(v,v2,G);
				update_edge_properties((boost::edge(v,v2)).first, G);															
			}		
			else{ //it means that src is inside and tgt outside
				add_new_edge(v1,src,G);
				add_new_edge(src,v2,G);
				update_edge_properties((boost::edge(src,v2)).first, G);
				next_src = v2;
			}	
			
			boost::remove_edge(I.int_edge, G);		
			break;			
		}
		
		case int_type::Overlap_extreme{
			
			break;
		}
		
		case int_type::Identical{
 			update_edge_properties(I.int_edge, G);
 			break;		
		}
	
	}

}; //refine_graph

bool same_coordinates(const Vertex_d & v1, const Vertex_d & v2, const Graph & G){
	return (G[v1].coordinates == G[v2].coordinates);
}

void add_new_edge(const Vertex_d & src, const Vertex_d & tgt, Graph & G){
	Edge_d e = boost::add_edge(src, tgt, G).first;
	std::cout<<"New edge created"<<std::endl;
	
	point2 SRC = G[src].coordinates;
	point2 TGT = G[tgt].coordinates;
	
	G[e].geometry.set_source(SRC);
	G[e].geometry.set_target(TGT);
}; // add_new_edge


Vertex_d add_new_vertex(const point2 & P, Graph & G){
	Vertex_d v = boost::add_vertex(G);
	std::cout<<"New vertex created"<<std::endl;
	G[v].coordinates = P;
	return v;
}

void cut_old_edge(Edge_d &e, const Vertex_d & v, Graph & G){
	Vertex_d src = boost::source(e, G);
	Vertex_d tgt = boost::target(e, G);

	boost::remove_edge(e, G);
	std::cout<<"Edge removed"<<std::endl;	
	add_new_edge(src, v, G);
	add_new_edge(v, tgt, G);	
}

void update_edge_properties(Edge_d &e, Graph &G){
	std::cout << "Updating properties..."<<std::endl;
}

} //namespace
