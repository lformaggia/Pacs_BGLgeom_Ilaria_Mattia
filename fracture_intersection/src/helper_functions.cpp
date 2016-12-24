// CORREGGERE FRAC_NUM (MA ASPETTA CHE AGGIORNIAMO STRUTTURE GENERALI)
// CAMBIARE == IN POINT PER TOLLERANZA (MEGLIO MODIFICARE LA FUNZIONE NEW_VERTEX DOVE PASSI ANCHE LA TOLLERANZA)
// USARE GENERICHE FUNZIONI BGLgeom ADD NEW EDGE ADD NEW VERTEX
// LOCAL DATA STRUCTURE DA SPOSTARE A INIZIO MAIN
// IL MAIN DIVENTA LA FUNZIONE CREATE GRAPH


#include<iostream>

#include"helper_functions.hpp"
#include"graph_builder.hpp" 

using namespace BGLgeom;

namespace BGLgeom{	

	void create_graph(Graph & G, BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop> & R, double tol){

		Edge_it e_it, e_end;
		BGLgeom::Intersection intobj_tmp; //object of class Intersection
		Fracture::Vertex_prop src_prop, tgt_prop; // here we store source and target properties read from the input file
		Fracture::Edge_prop e_prop;
		bool edge_alone; //if the edge doesn't intersect any of the other edges we need to create it, otherwise it will be created step by step
		
		std::vector<BGLgeom::Int_layer<Graph>> intvect;
		
		int count;
		
		unsigned int line_count = 0;
		
		while(!R.is_eof()){
			++line_count;
		
			R.get_data_from_line();
			src_prop = R.get_source_data();
			tgt_prop = R.get_target_data();
			e_prop = R.get_edge_data();
			e_prop.index = line_count; // set the index in the edge_property equal to line_count
			
			std::cout<<"-------------------STARTING A NEW LINE-------------------"<<std::endl;
			int count_v = 0;
			Vertex_it vv,vvend;
			for(std::tie(vv,vvend) = boost::vertices(G); vv!=vvend; ++vv) ++count_v;
			std::cout<<"There are "<<count_v<<" vertices"<<std::endl;
			
			Vertex_d src = new_vertex(src_prop, G, true); //inserts a new vertex and returns the vertex desc or simply returns an already existing vertex desc in the case the coordinates match
			Vertex_d tgt = new_vertex(tgt_prop, G, true);

			edge_alone = true;
			intvect.clear(); // at each line I have to start with the empty vector
			
			count = 0;
	
			// create the linear edge having source in src and target in tgt: this will be the input for the function compute_intersection
			const line L(G[src].coordinates, G[tgt].coordinates);
			
			
			for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){ //for loop on al the edges already in the graph
				++count;
				
				intobj_tmp = BGLgeom::compute_intersection(G[*e_it].geometry, L); //in intersection info about the type of intersection are stored
				if(intobj_tmp.intersect == true){
					BGLgeom::Int_layer<Graph> intobj(intobj_tmp, *e_it); // this is the structure I'm going to use
					edge_alone = false;
					intvect.push_back(intobj);
//					std::cout<<"Intersection with ("<<G[boost::source(*e_it,G)].coordinates<<";"<<G[boost::target(*e_it,G)].coordinates<<")"<<std::endl;
//					std::cout<<intobj<<std::endl;
				}	
				
				else{
					std::cout<<"NO intersection with fracture number "<< G[*e_it].index <<"("<<G[boost::source(*e_it,G)].coordinates<<";"<<G[boost::target(*e_it,G)].coordinates<<")"<<std::endl;
				}
			} //for
			
			std::cout<<count<<" edges"<<std::endl;
			
			if(edge_alone){
				std::cout<<"edge alone"<<std::endl;
				new_lin_edge(src,tgt,e_prop,G);
			}
			else{ // there is at least one intersection		
				Vertex_d next_src;
				Vertex_d current_src = src;
				
				//order intvect in decreasing or decreasing order based on the relative position of src and tgt and on the first elem in the intersection vector
				if(G[src].coordinates < G[tgt].coordinates){
					std::sort(intvect.begin(), intvect.end(), asc_order);
				}
				else{
					std::sort(intvect.begin(), intvect.end(), desc_order);
				}

				std::cout<<"ORDERED VECTOR"<<std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
				std::cout<<I<<std::endl;

				
				//remove multiple "common_extreme" and "T_old" with same intersection vertex_descriptor: they don't affect the graph so we don't consider them
				auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
				intvect.erase(last, intvect.end());
				
				std::cout<<"VECTOR WITHOUT DUPLICATES"<<std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
				std::cout<<I<<std::endl;
				
										
				// first, we resolve the connection between source and first intersection
				refine_graph(G, src, tgt, e_prop, intvect[0], next_src); //collego src alla prima intersection
				
				for(int i=1; i<intvect.size(); ++i){ //here we resolve all the intermediate intersections (if there is only one intrsection this loop is not executed)
					current_src = next_src;
					refine_graph(G, current_src, tgt, e_prop, intvect[i], next_src);	// graph, current source, current intersection object					
				};
				
				// Finally we connect the last intersection point with the target, but only if their vertex_desciptors do not coincide
				current_src = next_src;
				if(!(current_src == tgt));
					new_lin_edge(current_src, tgt, e_prop, G);										
			}// else 
		} //while(!eof)
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
	if(I1.how == intersection_type::Common_extreme || I1.how == intersection_type::T_old){
		for(const point2 & P2: I2.int_pts)
			if(I1.int_pts[0] == P2) return true;
	}
	// Do the same as before, inverting I1 and I2
	if(I2.how == intersection_type::Common_extreme || I2.how == intersection_type::T_old){
		for(const point2 & P1: I1.int_pts)
			if(I2.int_pts[0] == P1) return true;
	}
	return false;
};



void refine_graph(Graph &G, const Vertex_d & src, const Vertex_d & tgt, Fracture::Edge_prop & e_prop, BGLgeom::Int_layer<Graph> & I, Vertex_d & next_src){

	using int_type = BGLgeom::intersection_type;
	int_type T = I.how;
	Edge_d e; // this will be the edge descriptor of the new inserted edge
	
	switch(T){
	
		case int_type::Overlap_inside:
		{
			Vertex_d v1;
			Vertex_d v2;
								
			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);						
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);						
			}

			e = new_lin_edge(v1,src,G[I.int_edge],G);
			e = new_lin_edge(src,tgt,G[I.int_edge],G);
			update_edge_properties(e, e_prop, G);	// this edge has been "over-written" so we give the user the possibility to choose how to handle the mix of the two edge_properties, old one and new one				
			
			e = new_lin_edge(tgt,v2,G[I.int_edge],G);		
			
			boost::remove_edge(I.int_edge,G);
			std::cout<<"Edge removed"<<std::endl;						
		}
		
		
		case int_type::Overlap_extreme_inside:
		{
			Vertex_d v1;
			Vertex_d v2;
								
			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);						
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);						
			}
			
			
			if(v1 == src){ //the common extreme is the source, because they have the same vertex descriptor
				e = new_lin_edge(tgt,v2,G[I.int_edge],G);
				e = new_lin_edge(src,tgt, G[I.int_edge],G);
			}						
			else{ //the common extreme is the target
				e = new_lin_edge(v1,src,G[I.int_edge],G);
				e = new_lin_edge(src,tgt,G[I.int_edge],G);
			}
			
			update_edge_properties(e,e_prop,G); // in both cases of the if-else clause e is the edge_descr of the edge connecting src and tgt
			boost::remove_edge(I.int_edge,G);
			std::cout<<"Edge removed"<<std::endl;
		}


	
		case int_type::X:
		{
			// create vertex_descriptor for the intersection point
			Fracture::Vertex_prop v_prop(I.int_pts.front());
			Vertex_d v = new_vertex(v_prop, G);			
			// add edge between src and v
			e = new_lin_edge(src, v, e_prop, G);
			
			// now we need to cut in two the intersected edge
			cut_old_edge(I.int_edge, v, G);
			
			next_src = v;
			break;
		} 
		
		case int_type::T_new:{
			// if the intersection point corresponds to the target (i.e. I.intersected_extreme_new = 1) I add a new edge
			if(I.intersected_extreme_new == 1){
				e = new_lin_edge(src, tgt, e_prop,G);
				cut_old_edge(I.int_edge, tgt,G);
				next_src = tgt;
			}	
			else{
				cut_old_edge(I.int_edge, src, G);
				next_src = src;			
			}
			
			break;
		}
		
		case int_type::T_old:{
			// get the vertex descriptor of the extreme involved in the intersection
			Vertex_d v;
			if(I.intersected_extreme_old== 0) //first extreme, i.e the source
				v = boost::source(I.int_edge, G);
			else
				v = boost::target(I.int_edge, G);
				
			e = new_lin_edge(src, v, e_prop, G);
			
			next_src = v;
			break;
		}
		
		case int_type::Overlap_outside:{
			Vertex_d v1;
			Vertex_d v2; 
			
			if(!I.swapped_comp){ // if the components haven't been inverted, the nearest to src is the source, otherwise the target
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}

			
			if(v1 != src){ // otherwise the edge has already been added
				e = new_lin_edge(src, v1, e_prop, G);
			}
		
 			update_edge_properties(I.int_edge, e_prop,G);
			next_src = v2;				
 			
 			break;
		}
		
		case int_type::Overlap:{
			Vertex_d v1;
			Vertex_d v2;

			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}
			
			if(I.intersected_extreme_new==1 ){//it means that src is outside and tgt inside
				e = new_lin_edge(src,v1, e_prop, G);

				e = new_lin_edge(v1,tgt, G[I.int_edge], G);
				
				update_edge_properties(e,e_prop,G);
				
				e = new_lin_edge(tgt,v2,G[I.int_edge],G);
				
				next_src = tgt; // since next_src = tgt, no new line will be added in the last step (this is necessarily the last intersection of the vector, otherwise it couldn't involve the target)															
			}		
			else{ //it means that src is inside and tgt outside
				e = new_lin_edge(v1,src,G[I.int_edge],G);
				e = new_lin_edge(src,v2,G[I.int_edge],G);

				update_edge_properties(e,e_prop,G);
				
				next_src = v2;
			}	
			
			boost::remove_edge(I.int_edge, G);
			std::cout<<"Edge removed"<<std::endl;		
			break;			
		}
		
		case int_type::Identical:{
 			update_edge_properties(I.int_edge,e_prop, G);
 			
 			Vertex_d v1;
			Vertex_d v2;

			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}
			
			next_src = v2;
 			
 			break;		
		}
		
		case int_type::Common_extreme:{
			// get the vertex descriptor of the extreme involved in the intersection
			Vertex_d v;
			if(I.intersected_extreme_old== 0) //first extreme, i.e the source
				v = boost::source(I.int_edge, G);
			else
				v = boost::target(I.int_edge, G);
				
			
			if(!(src == v)){ // if src has same coordinates as v there's nothing to do, so we consider only the opposite case
				e = new_lin_edge(src, v, e_prop, G);
			}
			
			next_src = v;			
			break;
		}
		
		case int_type::Overlap_extreme_outside:{
			Vertex_d v1;
			Vertex_d v2;

			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}		
			
			if(!(src == v1)){ // it means that the common extreme is the target. I have to connect src to v1	
				e = new_lin_edge(src,v1, e_prop, G);
			}
			// and then I have simply to update the properties of the other part of the edge
			update_edge_properties(I.int_edge, e_prop,G);
			next_src = v2;
			break;
		}
	
	}

}; //refine_graph

void cut_old_edge(Edge_d &e, const Vertex_d & v, Graph & G){
	Vertex_d src = boost::source(e, G);
	Vertex_d tgt = boost::target(e, G);
	
	Edge_d e_tmp;
	
	e_tmp = new_lin_edge(src, v, G[e], G);
	e_tmp = new_lin_edge(v, tgt, G[e], G);	
	
	boost::remove_edge(e, G);
	std::cout<<"Edge removed ("<<G[src].coordinates<<";"<<G[tgt].coordinates<<")"<<std::endl;
}

void update_edge_properties(Edge_d &e, Fracture::Edge_prop & new_edge_prop, Graph &G){
	std::cout << "Updating properties..."<<std::endl;
// in the edges requiring update we inserted as properties the old ones, but there is an overlapping with the new inserted edge. Here you can decide how to "mix" the edge properties: just keep the old ones (as it is, so this function will do nothing in this case), just the new ones, or any other combination of the two
}

} //namespace
