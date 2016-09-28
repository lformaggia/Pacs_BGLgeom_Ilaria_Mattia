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

template <typename Graph>
class reader_Zunino final: public reader_base_class<Graph> {
	private:
		point<3> SRC,TGT; 												// they will store vertices coordinates	  
		unsigned int src, tgt; 											// they will read source and target of each edge
		unsigned int edge_num;											// dummy variable;
		double diam, length;											// they will store diameter and length of the edge		
		
	public:

		typedef typename reader_base_class<Graph>::Vertex_desc Vertex_desc;
		typedef typename reader_base_class<Graph>::Edge_desc Edge_desc;
		
		//! Default constructor (we need however to initialize the reference to the graph)
		reader_Zunino(Graph & _G): reader_base_class<Graph>(_G), SRC(), TGT(), src(0), tgt(0), edge_num(0), diam(0.0), length(0.0) {};
		
		//! Constructor: it assigns value only to the variables in reader_base_class, the others in reader_Zunino are defaulted
		reader_Zunino	(Graph & _G,
						std::string _file_name,
						unsigned int _num_dummy_lines): reader_base_class<Graph>(_G, _file_name, _num_dummy_lines),
														SRC(),
														TGT(),
														src(0),
														tgt(0),
														edge_num(0),
														diam(0.0),
														length(0.0) {};
		//! Default copy constructor
		reader_Zunino(reader_Zunino const&) = default;
		
		//! Assignment operator
		reader_Zunino & operator=(reader_Zunino const&) = default;
		
		//! Destructor
		virtual ~reader_Zunino(){};
		
		//! This is the way we read and interpret a file from a Zunino input file format
		virtual void read_data_from_line(std::istringstream & temp){
			temp >> edge_num >> src >> tgt >> diam >> length >> SRC >> TGT;
		};
		
		//! It build the graph one edge at a time
		virtual void build_graph(){
			//if(vertices_index_already_present) ----> ????
			//devo automatizzare in qualche modo le due linee sotto!
			
			// devo usare this->new_source perché new_source non è definita in reader_Zunino, quindi chimando solo new_source lui non vede niente. Ma siccome eredita da reader_base, new source è in quello, che è compreso qui in reader_Zunino, e quindi c'è e lo posso vedere chiamando this->new_source
			this->new_source = src; //solo se scelgo vecS vecS nell'adjacency list
			this->new_target = tgt; //idem come sopra
			std::tie(this->new_edge, this->edge_inserted) = boost::add_edge(this->new_source, this->new_target, this->G);
			this->if_edge_not_inserted();		//controllo se l'edge è stato inserito o meno
			this->give_vertex_properties();		//non c'è bisogno del this, ma magari meglio lasciarlo per chiarezza. non so.
			this->give_edge_properties();
		};
		
		//! It assign the right properties to the vertices just added
		virtual void give_vertex_properties(){
			this->G[this->new_source] = SRC;
			this->G[this->new_target] = TGT;
		};
		
		//! It assign the properties to the edge just added
		virtual void give_edge_properties(){
			this->G[this->new_edge].capacity = diam;
			this->G[this->new_edge].length = length;
		};
		
};	//reader_Zunino

#endif 	//HH_READER_ZUNINO_CLASS_HH
