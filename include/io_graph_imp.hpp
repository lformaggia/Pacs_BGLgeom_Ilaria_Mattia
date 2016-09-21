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
	
template<typename Graph>
void read_zunino_old_format(Graph & G, std::string file_name){ 
	
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

	std::ifstream file(file_name.c_str());
	
	// variables for storing data
	
	//crei variabili per leggere dati (attributi della classe utente)
	point<3> SRC,TGT; 												// they will store vertex coordinates	  
	int src, tgt; 													// they will read source and target of eah edge
	int edge_num;													// dummy variable;
	double diam, length; 											// they will properties associated to edges

	// ignore the first two lines of the file
	
	//Ignora intestazioni		R.ignore_dummy_lines(2,file);
	std::string dummyLine;
	std::getline(file, dummyLine);
	std::getline(file, dummyLine);
	
	// Utilities for reading and constructing the Graph
	
	//volendo check doppioni
	std::set<int> vertex_set; 		/*< using a set, we can easily check if a vertex has already been added */
	std::pair<std::set<int>::iterator, bool> set_inserter;
	std::string s;			
	edge_descriptor e;
	bool edge_inserted;
	
	while (!file.fail() && !file.eof()){
		std::getline(file,s); 													// reading the the whole line
	    if(s.empty())
	    	continue; 
	    //leggi dalla riga tutte le variabili che mi interessano														// empty line
	    std::istringstream tmp(s); 												// build an input sstream.
	    tmp >> edge_num >> src >> tgt >> diam >> length >> SRC >> TGT;			// reading data from the istringstream
	    
	    if(!tmp.fail()){			
			// create edge (src, tgt)
			
			//crea nodi
			
			//associa proprietà ai nodi
			
			//crea arco?
			std::tie(e, edge_inserted) = boost::add_edge(src, tgt, G);
			
			//associa le proprietà agli archi	
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
	//creo due vertex_descriptor (non li ho ancora, quindi poi ci pensa lui ad indicizzarli)
	Vertex_desc src, tgt;
	
	//Reading:
	while(!file.fail() && !file.eof()){
		std::getline(file, s);
		if(s.empty())
			continue;
		std::istringstream temp(s);
		//Leggo i vari dati (ogni utente qui metterà il suo)
		temp >> frac_number >> SRC >> TGT;
		std::cout << SRC << "  " << TGT << std::endl;
		if(!temp.fail()){
			// this function returns a vertex descriptor: a new one if not present in G, or the respctive and already existing vertex
			src = vertex_insertion_or_identification(G, SRC); 
			tgt = vertex_insertion_or_identification(G, TGT);
			
			// find intersections with other edges
			std::vector<std::pair<point<2>, Edge_desc> > intersections;
			check_for_intersections(intersections, SRC, TGT, G); // checks for intersections with every edge in the graph and re-orders them
			
			// if there are intersection points, refine the graph
			refine_graph(G, intersections, frac_number, src, tgt);			
		}	//if		
	}	//while
	
};	//read_Formaggia_format


template <typename Graph>
typename boost::graph_traits<Graph>::vertex_descriptor     //return type
vertex_insertion_or_identification(Graph & G, point<2> const& P){

	typename boost::graph_traits<Graph>::vertex_iterator v_it, v_end;
	typename boost::graph_traits<Graph>::vertex_descriptor v;
	
	double diff_x, diff_y;
	double toll = 0.001;	 // the threshold under which two points have two be considered the same point. From GetPot???
	
	for(std::tie(v_it, v_end) = boost::vertices(G); v_it != v_end; ++v_it){
		diff_x = std::abs(P.x() - G[*v_it].coord.x()); 		// absolute difference between x coordinates
		if(diff_x < toll){
			diff_y = std::abs(P.y() - G[*v_it].coord.y()); 
			if(diff_y < toll){
				v = *v_it;
				return v;
			}
		}
	} //for
	
	// if you arrive here it means the vertex you are considering isn't already in the graph so you add it
	v = boost::add_vertex(G);
	std::cout << "new_vertex_created " << v << std::endl; 
	G[v].coord = P;  // assign the property
	G[v].is_external = true;		//check if it is the right condition
	return v;
	
} // vertex_insertion_or_identification


template<typename Graph>
void check_for_intersections(std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > & vect,
							 point<2> const & SRC,
							 point<2> const & TGT, 
							 Graph const & G){

	typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
	typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter;	
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
	
	Edge_iter e_it, e_end;
	Vertex_desc u,v;
	std::pair<point<2>, point<2> > line1, line2;
	line1 = std::make_pair(SRC, TGT);
	bool ok_intersection;				// output of are_intersected
	point<2> intersection_point;		// output of are_intersected
	
	for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; e_it++){
		u = boost::source(*e_it, G);
		v = boost::target(*e_it, G);
		line2 = std::make_pair(G[u].coord, G[v].coord);		
				
		//Qua ci va la funzione del prof che calcola le intersezioni
		std::tie(ok_intersection, intersection_point) = are_intersected(line1, line2);		//prototipo di definizione di quella funzione.
		
		if(ok_intersection)		//se intersecano, inserisco il punto
			vect.push_back(std::make_pair(intersection_point, *e_it));
	}	//for
	
	// Re-ordering
	enum {src_greater_than_tgt, src_less_than_tgt};
	
	if(SRC < TGT)
		std::sort(vect.begin(), vect.end(), compare<Graph, src_less_than_tgt>);
	else
		std::sort(vect.begin(), vect.end(), compare<Graph, src_greater_than_tgt>);
	
}	//check_for_intersection


template<typename Graph, bool src_less_than_tgt>
bool compare(std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair1,
			 std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair2){
	
	if(src_less_than_tgt)
		return pair1.first < pair2.first;
	else 
		return pair1.first > pair2.first;
}	//compare



std::pair<bool, point<2> > are_intersected 	(std::pair<point<2>, point<2> > line1,
											 std::pair<point<2>, point<2> > line2){
	point<2> p;
	bool vertical1 = false;
	bool vertical2 = false;
	double x_intersect, y_intersect;
	
	if(line1.first.x() == line1.second.x()){
		vertical1 = true;
		x_intersect = line1.first.x();
	}
	else y_intersect = line1.first.y();
	
	if(line2.first.x() == line2.second.x()){
		vertical2 = true;
		x_intersect = line2.first.x();
	}
	else y_intersect = line2.first.y();
	
	if (vertical1 + vertical2 == 1){
		p.set({x_intersect, y_intersect});
		return std::make_pair(true,p);
	}
	else{
		return std::make_pair(false, p);
	}
}	//are_intersected


template<typename Graph>
void refine_graph(Graph & G, 
				  typename std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > const & vect, 
				  int frac_number,
                  typename boost::graph_traits<Graph>::vertex_descriptor src,
                  typename boost::graph_traits<Graph>::vertex_descriptor tgt){
	
	typedef typename std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> >::const_iterator Vector_iter;
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
	typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
	
	Vector_iter vec_it = vect.begin();
	Vector_iter vec_end = vect.end();	
	Vertex_desc v_new, v_old;
	Vertex_desc u, v; // this are the vertices connected by the edge which will be substitueted by two new edges 		
	Edge_desc e, e_to_be_removed, e_new;	
	int frac_num_old;
	
	v_old = src;
	// we start connecting source and first point of intersection
	for(; vec_it != vec_end; ++vec_it){
		v_new = boost::add_vertex(G);	
		G[v_new].coord = (*vec_it).first;
		G[v_new].is_external = false;
		
		// Connecting old and new vertex:
		e = boost::add_edge(v_old, v_new, G).first; // we are not interested in the bool value
		G[e].frac_num = frac_number;
		
		// Storing informations of the edge we will remove
		e_to_be_removed = (*vec_it).second;
		frac_num_old = G[e_to_be_removed].frac_num;  // we store this value because we have to assign it to the new created edges
		
		// Storing source and target of the vertex we will remove, to create the new edges after
		u = boost::source(e_to_be_removed, G);
		v = boost::target(e_to_be_removed, G);
		
		// we remove the old edge which has to be replaced
		boost::remove_edge(e_to_be_removed, G);
		
		// connect u and v to the intersection point (so we broke the old edge into two new ones)
		e_new = boost::add_edge(u, v_new, G).first;
		G[e_new].frac_num = frac_num_old;		
		e_new = boost::add_edge(v_new, v, G).first;
		G[e_new].frac_num = frac_num_old;
		
		// we move v_new into v_old
		v_old = v_new;
	}
	
	// last thing: connect the last intersection point with tgt of the current edge
	e_new = boost::add_edge(v_old, tgt, G).first;
	G[e_new].frac_num = frac_number;
} // refine_graph

#endif
