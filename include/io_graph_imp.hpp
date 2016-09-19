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
#include <cmath>
#include <algorithm>

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
			// this function returns a vertex descriptor: a new one if not present in G, or the respctive and already existing vertex
			u = vertex_insertion_or_identification(G, SRC); // o le mettiamo void e lo passiamo per reference?
			v = vertex_insertion_or_identification(G, TGT); 
			
			// find intersections with other edges
			std::vector<std::pair<Point, Edge_desc> > intersections;
			check_for_intersections(intersections, SRC, TGT, G); // checks for intersections with every edge in the graph and re-orders them
			
			refine_graph(G, intersections);
			
		}	//if		
	}	//while
};	//read_Formaggia_format


template <typename Graph, typename Point>
typename boost::graph_traits<Graph>::vertex_descriptor // return type
vertex_insertion_or_identification(Graph const& G, Point const& P){

	typename boost::graph_traits<Graph>::vertex_iterator v_it, v_end;
	typename boost::graph_traits<Graph>::vertex_descriptor v;
	
	for(std::tie(v_it, v_end) = boost::vertices(G); v_it != v_end; ++v_it){
		double diff_x = std::abs(P.x() - G[*v_it].coord.x()); // absolute difference between x coordinates
		double diff_y;
		double toll = 0.001; // the threshold under which two points have two be considered the same point. From GetPot???
		if(diff_x < toll){
			diff_y = std::abs(P.y() - G[*v_it].coord.y()); 
			if(diff_y < toll){
				return *v_it;
			}
		}
		else{
			v = add_vertex(G);
			G[v].coord = P;  // assign the property
			G[v].is_external = true;
			return v;
		}
	}
	
} // vertex_insertion_or_identification


template <typename G, typename Point>
void check_for_intersections(std::vector<std::pair<Point, typename boost::graph_traits<Graph>::edge_descriptor> > & vect,
							 typename Point const & SRC,
							 typename Point const & TGT, 
							 typename Graph const & G){

	typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
	typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter;	
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
	
	Edge_iter e_it, e_end;
	Vertex_desc u,v;
	std::pair<Point, Point> line1, line2;
	line1 = std::make_pair(SRC, TGT);
	
	for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; e_it++){
		u = boost::source(*e_it);
		v = boost::target(*e_it);
		line2 = std::make_pair(G[u].coord, G[v].coord);
		
		bool ok_intersection;
		Point intersection_point;		
		//Qua ci va la funzione del prof che calcola le intersezioni
		std::tie(ok_intersection, new_Point) = are_intersected(line1, line2);		//prototipo di definizione di quella funzione.
		
		if(ok_intersection)		//se intersecano, inserisco il punto
			vect.push_back(std::make_pair(intersection_point, *e_it));
	}	//for
	
	
	
	// Re-ordering
	enum {decreasing, increasing};
	enum {xx,yy};
	
	if(SRC.x() < TGT.x())
		std::sort(vect.begin(), vect.end(), compare<increasing,xx>);	//ordinare il vettore in modo crescente secondo le x dei punti
	else if (SRC.x() > TGT.x())
		std::sort(vect.begin(), vect.end(), compare<decreasing,xx>);	//ordinare il vettore in modo decrescente secondo le x dei punti
	else if(SRC.y() < TGT.y())
		std::sort(vect.begin(), vect.end(), compare<increasing,yy>);
	else
		std::sort(vect.begin(), vect.end(), compare<decreasing,yy>);	//(retta verticale) ordina per y decrescenti
	
	
}	//check_for_intersection


template<typename Graph, typename Point>
void refine_graph(Graph & G, std::vector<std::pair<Point, typename boost::graph_traits<Graph>::edge_descriptor> > const & vect){
	
	typedef std::vector<std::pair<Point, typename boost::graph_traits<Graph>::edge_descriptor> >::iterator Vector_iter;
	
	Vector_iter v_it = vect.begin();
	Vector_iter v_end = vect.end();
	
	
	
}

#endif
