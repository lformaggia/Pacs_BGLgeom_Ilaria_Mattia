/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
* \file reader_Zunino_class.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sept, 2016
* \brief Implementation of the reader for Zunino file format
*/

//Devo includere l'overloading di tutti i metodi = 0 in reader_base_class, anche se alcuni non li uso.


#ifndef HH_READER_ZUNINO_CLASS_HH
#define HH_READER_ZUNINO_CLASS_HH

#include <tuple>

#include "reader_base_class.hpp"
#include "generic_point.hpp"
#include "Zunino_edge_property.hpp"		//va inclusa o funziona anche senza?

class reader_Zunino final: public reader_base_class {
	private:
		point<3> SRC,TGT; 												// they will store vertex coordinates	  
		unsigned int src, tgt; 													// they will read source and target of each edge
		unsigned int edge_num;													// dummy variable;
		double diam, length;													// they will store diameter and length of the edge		
		
	public:
		//! Default constructor:
		reader_Zunino(): reader_base_class(), SRC(), TGT(), src(0), tgt(0), edge_num(0), diam(0.0), length(0.0) {};
		
		//! COnstructor: it assigns value only to the variables in reader_base_class, the others in reader_Zunino are defaulted
		reader_Zunino	(std::string _file_name,
						unsigned int _num_dummy_lines): reader_base_class(_file_name, _num_dummy_lines),
														SRC(),
														TGT(),
														src(0),
														tgt(0),
														edge_num(0),
														diam(0.0),
														lenght(0.0) {};
		//! Default copy constructor
		reader_Zunino(reader_Zunino const&) = default;
		
		//! Assignment operator
		reader_Zunino & operator=(reader_Zunino const&) = default;
		
		//! Destructor
		virtual ~reader_Zunino(){};
		
		//! This is the way we read and interpret a file from a Zunino input file format
		virtual void read_data_from_line(std::istringstream const& temp){
			temp >> edge_num >> src >> tgt >> diam >> length >> SRC >> TGT;
		};
		
		/*
		//! It adds a new vertex in the graph
		virtual void create_vertex(Vertex_desc & v){
			v = 
		};
		*/
		
		//! It build the graph one edge at a time
		virtual void build_graph(){
			//if(vertices_index_already_present) ----> ????
			//devo automatizzare in qualche modo le due linee sotto!
			new_source = src; //solo se scelgo vecS vecS nell'adjacency list
			new_target = tgt; //idem come sopra
			std::tie(new_edge, edge_inserted) = boost::add_edge(new_source, new_target, G);		//add_edge(new_source, new_target, G)
			if(!edge_inserted){
				//succede qualcosa
			}
		};
		
		//! It assign the right properties to the vertices just added
		virtual void give_vertex_properties(Vertex_desc const& new_source, Vertex_desc const& new_target){
			G[new_source] = SRC;
			G[new_target] = TGT;
		};
		
		//! It assign the properties to the edge just added
		virtual void give_edge_properties(Edge_desc const& e){
			G[e].capacity = diam;
			G[e].length = length;
		};
};

#endif 	//HH_READER_ZUNINO_CLASS_HH
