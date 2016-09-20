/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
* \file io_graph.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sep 14, 2016
* \brief Declaration of functions related to input and output of the graph 
*/

#ifndef HH_IO_GRAPH_HH
#define HH_IO_GRAPH_HH

#include "generic_point.hpp"

template <typename Graph, typename Point>
void read_zunino_old_format(Graph & G, std::string file_name);

template <typename Graph>
void read_Formaggia_format(Graph & G, std::string file_name);

template <typename Graph>
typename boost::graph_traits<Graph>::vertex_descriptor     //return type
vertex_insertion_or_identification(Graph & G, point<2> const& P);

template <typename Graph>
void check_for_intersections(std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > & v,
							 point<2> const & SRC,
							 point<2> const & TGT, 
							 Graph const & G);

std::pair<bool, point<2> > are_intersected (std::pair<point<2>, point<2> > line1, std::pair<point<2>, point<2> > line2){
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
}

template<typename Graph, bool src_less_than_tgt>
bool compare(std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair1,
			 std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair2);

template<typename Graph>
void refine_graph(Graph & G, 
				  typename std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > const & vect, 
                  int frac_number,
                  typename boost::graph_traits<Graph>::vertex_descriptor src,
                  typename boost::graph_traits<Graph>::vertex_descriptor tgt);

#include "io_graph_imp.hpp"	

#endif 
