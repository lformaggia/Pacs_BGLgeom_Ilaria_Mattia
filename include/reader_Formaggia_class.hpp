/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file reader_Formaggia_class.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Implementation of the reader_base_class for the Formaggia file format
*/

#ifndef HH_READER_FORMAGGIA_CLASS_HH
#define HH_READER_FORMAGGIA_CLASS_HH

#include "reader_base_class.hpp"
#include "intersector_class.hpp"
#include "generic_point.hpp"

template <typename Graph>
class reader_Formaggia final: public reader_base_class<Graph> {
	public:
		//credo inutile perché c'è già nel public di reader_base_class, e stiamo ereditando da lì
		//typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
	
		//! Default constructor (we need however to initialize the reference to the graph)
		reader_Formaggia(Graph & _G) : reader_base_class<Graph>(_G), frac_number(0), SRC(), TGT(), src_temp(), tgt_temp(), I(_G) {};
		
		//! Constructor
		reader_Formaggia	(Graph & _G,
							std::string _file_name,
							unsigned int _num_dummy_lines) : 	reader_base_class<Graph>(_G, _file_name, _num_dummy_lines),
																frac_number(0),
																SRC(),
																TGT(),
																src_temp(),
																tgt_temp(),
																I(_G) {};
		
		//! Default copy constructor
		reader_Formaggia(reader_Formaggia const&) = default;
		
		//! Default assignment operator
		reader_Formaggia & operator=(reader_Formaggia const&) = default;
		
		//! Destructor
		virtual ~reader_Formaggia(){};
		
		//! This is the way to interpret the data form Formaggia data file
		virtual void read_data_from_line(std::istringstream & temp){
			temp >> frac_number >> SRC >> TGT;
		};
		
		//! Assigns properties to the vertices of the just read edge
		virtual void give_vertex_properties(){
			this->G[this->new_source].coord = SRC;
			this->G[this->new_source].is_external = true;
			this->G[this->new_target].coord = TGT;
			this->G[this->new_target].is-external = true;
		};
		
		//! 
		virtual void give_edge_properties(){
		
		};
		
		//!
		virtual void build_graph(){
			//per ora facciamo senza controllo su vertici molto vicini. Inserisco e basta assumendo che siano vertici diversi.
			this->new_source = boost::add_vertex(G);
			this->new_target = boost::add_vertex(G);
			this->give_vertex_properties();
			
			//riesco a racchiudere tutta sta roba in un unico metodo di una classe intersector abrstract?
			
			//costruisco la linea per l'arco corrente:
			I.set_current_edge(SRC, TGT);
			//inizializzo il numero della frattura corrente
			I.set_current_frac_number(frac_number);
			//calcolo tutte le intersezioni:
			I.compute_intersections();
			//ordiniamo i punti di intersezione per collegare bene i nuovi archi
			I.order_intersections();
			// raffino il grafo date le intersezioni che ho appena calcolato
			I.refine_graph();
			
			/*		//compreso in I.compute_intersections()
			for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it){
				src_temp = boost::source(*e_it, G);
				tgt_temp = boost::target(*e_it, G);
				I.set_intersection_edge(G[src_temp].coord, G[tgt_temp].coord);
				
				
			}	// for
			*/
			
		};
		
	private:
		//! It stores the fracture number of the new edge
		unsigned int frac_number;
		//! The coordinates of the extremes of the new edge
		point<2> SRC, TGT;
		//! Two vertex_descriptor that helps constructiong the graph
		Vertex_desc	src_temp, tgt_temp;
		//! The class that manages intersection while constructing the graph
		intersector I;
		
		
		
		
};		//reader_Formaggia


#endif	//HH_READER_FORMAGGIA_CLASS_HH
