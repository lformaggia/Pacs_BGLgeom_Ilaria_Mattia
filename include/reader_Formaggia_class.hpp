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

#include <algorithm>		//per il sort in order_intersection

#include "reader_base_class.hpp"
#include "intersector_class.hpp"
#include "generic_point.hpp"

// Per accere a tutti gli attributi e metodi di reader_base_class e intersector_base_class dovrò mettere this-> davanti

template <typename Graph>
class reader_Formaggia final: public reader_base_class<Graph>, public intersector_base_class<Graph> {
	public:
		//credo inutile perché c'è già nel public di reader_base_class, e stiamo ereditando da lì
		//typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
	
		//! Default constructor (we need however to initialize the reference to the graph)
		reader_Formaggia(Graph & _G) :	reader_base_class<Graph>(_G),
										intersector_base_class<Graph>(_G),
										frac_number(0),
										SRC(),
										TGT(),
										src_temp(),
										tgt_temp() {};
		
		//! Constructor
		reader_Formaggia	(Graph & _G,
							std::string _file_name,
							unsigned int _num_dummy_lines) : 	reader_base_class<Graph>(_G, _file_name, _num_dummy_lines),
																intersector_base_class<Graph>(_G),
																frac_number(0),
																SRC(),
																TGT(),
																src_temp(),
																tgt_temp() {};
		
		//! Default copy constructor
		reader_Formaggia(reader_Formaggia const&) = default;
		
		//! Default assignment operator
		reader_Formaggia & operator=(reader_Formaggia const&) = default;
		
		//! Destructor
		virtual ~reader_Formaggia(){};
		
		//============= OVERRIDING OF reader_base_class METHODS ================
		
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
			this->set_Edge1(SRC, TGT);
			//inizializzo il numero della frattura corrente
			this->set_current_frac_number(frac_number);
			//calcolo tutte le intersezioni:
			this->compute_intersections();///!!!!!!NON C'é PIù!!!!
			//ordiniamo i punti di intersezione per collegare bene i nuovi archi
			this->order_intersections();
			// raffino il grafo date le intersezioni che ho appena calcolato
			this->refine_graph();
			
		};
		
		
		//============ OVERRIDING OF intersector_base_class METHODS ==============
		
		//! It checks if edges are intersected (only vertical or horizontal)
		bool are_intersected();
		
		//! Boh
		void refine_graph();
		
		//! Bohboh
		void order_intersections();
		
		//! BOH
		bool src_less_than_tgt	(std::pair<point<2>, Edge_desc> intersection_vector_elem1,
					 		 	 std::pair<point<2>, Edge_desc> intersection_vector_elem2);
					 		 	 
		//! BOOOOHHH
		bool src_greater_than_tgt	(std::pair<point<2>, Edge_desc> intersection_vector_elem1,
			 		 	 	 		 std::pair<point<2>, Edge_desc> intersection_vector_elem2)
		
		
	private:
		//! It stores the fracture number of the new edge
		unsigned int frac_number;
		//! The coordinates of the extremes of the new edge
		point<2> SRC, TGT;
		//! Two vertex_descriptor that helps constructiong the graph
		Vertex_desc	src_temp, tgt_temp;
				
};		//reader_Formaggia


tempalte <typename Graph>
bool reader_Formaggia::are_intersected(){
	bool vertical1 = false;
	bool vertical2 = false;
	double x_intersect, y_intersect;
	
	if(this->Edge1.first.x() == this->Edge1.second.x()){
		vertical1 = true;
		x_intersect = this->Edge1.first.x();
	}
	else
		y_intersect = this->Edge1.first.y();
	
	if(this->Edge2.first.x() == this->Edge2.second.x()){
		vertical2 = true;
		x_intersect = this->Edge2.first.x();
	}
	else
		y_intersect = this->Edge2.first.y();
	
	if (vertical1 + vertical2 == 1){
		this->intersection_point.set({x_intersect, y_intersect});
		return true;
	}
	else
		return false;
};	//are_intersected


template <typename Graph>
void reader_Formaggia::refine_graph(){

};	//refine_graph


template <typename Graph>
void reader_Formaggia::order_intersections(){
	if(this->Edge1.first < this->Edge1.second)
		std::sort(this->intersections.begin(), this->intersections.end(), this->src_less_than_tgt);
	else
		std::sort(this->intersections.begin(), this->intersections.end(), this->src_greater_than_tgt);
};	//order_intersections


template <typename Graph>
bool reader_Formaggia::src_less_than_tgt	(std::pair<point<2>, Edge_desc> intersection_vector_elem1,
					 		 	 			 std::pair<point<2>, Edge_desc> intersection_vector_elem2){
	return intersection_vector_elem1.first < intersection_vector_elem2.first;
};	//src_less_than_tgt


template <typename Graph>
bool reader_Formaggia::src_greater_than_tgt	(std::pair<point<2>, Edge_desc> intersection_vector_elem1,
					 		 	 	 		 std::pair<point<2>, Edge_desc> intersection_vector_elem2){
	return intersection_vector_elem1.first > intersection_vector_elem2.first;
};	//src_greater_than_tgt

#endif	//HH_READER_FORMAGGIA_CLASS_HH
