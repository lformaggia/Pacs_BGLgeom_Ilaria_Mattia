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
#include "intersector_base_class.hpp"
#include "generic_point.hpp"

// Per accere a tutti gli attributi e metodi di reader_base_class e intersector_base_class dovrò mettere this-> davanti

template <typename Graph>
class reader_Formaggia final: public reader_base_class<Graph>, public intersector_base_class<Graph> {
	public:
		//Vanno fatti così i typedef. Non sono inutili.
		typedef typename reader_base_class<Graph>::Vertex_desc Vertex_desc;
		typedef typename reader_base_class<Graph>::Edge_desc Edge_desc;
		typedef typename intersector_base_class<Graph>::Edge_iter Edge_iter;
		typedef typename intersector_base_class<Graph>::Intersections_type Intersections_type;
	
		//! Default constructor (we need however to initialize the reference to the graph)
		reader_Formaggia(Graph & _G) :	reader_base_class<Graph>(_G),
										intersector_base_class<Graph>(),
										frac_number(0),
										SRC(),
										TGT(),
										frac_number_old(0),
										src_temp(),
										tgt_temp(),
										e_to_be_removed(),
										split_edge(),
										intersection_new(),
										intersection_old(),
										vec_it(),
										vec_end() {};
		
		//! Constructor
		reader_Formaggia	(Graph & _G,
							std::string _file_name,
							unsigned int _num_dummy_lines) : 	reader_base_class<Graph>(_G, _file_name, _num_dummy_lines),
																intersector_base_class<Graph>(),
																frac_number(0),
																SRC(),
																TGT(),
																frac_number_old(0),
																src_temp(),
																tgt_temp(),
																e_to_be_removed(),
																split_edge(),
																intersection_new(),
																intersection_old(),
																vec_it(),
																vec_end() {};
		
		//! Default copy constructor
		reader_Formaggia(reader_Formaggia const&) = default;
		
		//! Default assignment operator
		reader_Formaggia & operator=(reader_Formaggia const&) = default;
		
		//! Destructor
		virtual ~reader_Formaggia(){};
		
		//=================== ATTRIBUTES SETTING METHODS ====================
		//Really needed? Perché son tutte variabili che non userò e non vedrò mai fuori..
		
		//! It allows to set e_to_be_removed
		void set_e_to_be_removed(Edge_desc const& _e_to_be_removed){
			e_to_be_removed = _e_to_be_removed;
		};
		
		//! It allows to set split_edge
		void set_split_edge(Edge_desc const& _split_edge){
			split_edge = _split_edge;
		};
		
		//! It allows to set intersection_new
		void set_intersection_new(Vertex_desc const& _intersection_new){
			intersection_new = _intersection_new;
		};
		
		//! It allows to set intersection_old
		void set_intersection_old(Vertex_desc const& _intersection_old){
			intersection_old = _intersection_old;
		};
		
		//============= OVERRIDING OF reader_base_class METHODS ================
		
		//! This is the way to interpret the data form Formaggia data file
		virtual void read_data_from_line(std::istringstream & temp){
			temp >> frac_number >> SRC >> TGT;
		};
		
		//! It assigns properties to new_source in the right way
		virtual void give_new_source_properties(){
			this->G[this->new_source].coord = SRC;
			this->G[this->new_source].is_external = true;
		};
		
		//! It assigns properties to new_target in the right way
		virtual void give_new_target_properties(){
			this->G[this->new_target].coord = TGT;
			this->G[this->new_target].is_external = true;
		};
		
		//! Overriding of the abstract method. It assigns properties to new_edge in the right way
		virtual void give_new_edge_properties(){
			this->G[this->new_edge].frac_num = frac_number;
		};
				
		//! The set of instruction for one single step in the building of the graph
		virtual void build_graph();
		
		//========= EXTRA reader_Formaggia METHODS (NOT OVERRIDDEN) ===============
		
		//! Overriding of the abstrac method. It assigns properties to a new intersection point in the right way
		void give_new_intersection_properties(){
			this->G[intersection_new].coord = (*vec_it).first;
			this->G[intersection_new].is_external = false;
		};
		
		//! It assigns properties to split_edge in the right way
		 void give_split_edge_properties(){
			this->G[split_edge].frac_num = frac_number_old;
		};
				
		//============ OVERRIDING OF intersector_base_class METHODS ==============
		
		//! It checks if edges are intersected (only vertical or horizontal)
		virtual bool are_intersected();
		
		//! Boh
		virtual void refine_graph();
		
		//! Bohboh
		virtual void order_intersections();
		
	private:
	//================== ATTRIBUTES TO STORE INPUT DATA =====================
		//! It stores the fracture number of the new edge
		unsigned int frac_number;
		//! The coordinates of the extremes of the new edge
		point<2> SRC, TGT;
		
	//=============== ATTRRIBUTES NEEDED TO BUILD THE GRAPH ===============	//Meglio qui o creare e distruggere variabili ogni volta nei metodi?
		//! Fracture number of the edge (Edge2) that will be refined
		unsigned int frac_number_old;	
		//! Two vertex_descriptor that helps constructiong the graph
		Vertex_desc	src_temp, tgt_temp;
		//! Edge descriptor of the Edge2 that will be removed and broken into two new edges
		Edge_desc e_to_be_removed;
		/*! 
			@brief Edge descriptor used to store step by step the pieces of e_to_be_removed
			@detail split_edge will be the descriptor of the pieces of Edge2/e_to_be_removed.
					this->new_edge will be the descriptor of the pieces of Edge1.
		*/
		Edge_desc split_edge;
		//! Vertex descriptor for intersection vertices. They are two beacause of the algorithm
		Vertex_desc intersection_new, intersection_old;	
		//!	Iterators to go through the vector intersections
		typename Intersections_type::const_iterator vec_it, vec_end;
				
};		//reader_Formaggia


template <typename Graph>
void reader_Formaggia<Graph>::build_graph(){
	//per ora facciamo senza controllo su vertici molto vicini. Inserisco e basta assumendo che siano vertici diversi.
	this->new_source = boost::add_vertex(this->G);
	this->give_new_source_properties();
	this->new_target = boost::add_vertex(this->G);
	this->give_new_target_properties();
	
//riesco a racchiudere tutto in un unico metodo di una classe intersector abstract? Meglio di no e lasciare le singole operazioni spacchettate
	
	//costruisco la linea per l'arco corrente:
	this->set_Edge1(SRC, TGT);
	//calcolo tutte le intersezioni:
	Edge_iter e_it, e_end;
	for(std::tie(e_it, e_end) = edges(this->G); e_it != e_end; ++e_it){
		src_temp = boost::source(*e_it, this->G);
		tgt_temp = boost::target(*e_it, this->G);
		this->set_Edge2(this->G[src_temp].coord, this->G[tgt_temp].coord);
		if(this->are_intersected()){
			this->set_Edge2_descriptor(*e_it);
			this->store_intersection();
		}	//if
	}	//for
	//ordiniamo i punti di intersezione per collegare bene i nuovi archi
	this->order_intersections();
	// raffino il grafo date le intersezioni che ho appena calcolato
	this->refine_graph();
	// svuotiamo il vettore intersections
	this->clear_intersections();			
};	//build_graph


template <typename Graph>
bool reader_Formaggia<Graph>::are_intersected(){
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
void reader_Formaggia<Graph>::refine_graph(){
	
	typedef typename reader_base_class<Graph>::Edge_desc Edge_desc;		//brutto, da 
	typedef typename std::vector<std::pair<point<2>, Edge_desc> >::const_iterator Vector_iter;

	vec_it = this->intersections.begin();
	vec_end = this->intersections.end();
	
	intersection_old = this->new_source;	// to connect in the right way new_source to the first intersection vertex	
	for(; vec_it != vec_end; ++vec_it){
		intersection_new = boost::add_vertex(this->G);
		this->give_new_intersection_properties();		
		
		// Connecting intersection_old and intersection_new:
		std::tie(this->new_edge, this->edge_inserted) = boost::add_edge(intersection_old, intersection_new, this->G);
		this->if_edge_not_inserted();
		this->give_new_edge_properties();		//Qui assegna la fracture number ddi Edge1
		
		// Storing properties of Edge2 which we will remove
		e_to_be_removed = (*vec_it).second;
		frac_number_old = this->G[e_to_be_removed].frac_num;  // we store this value because we have to assign it to the new created edges
		
		// Storing source and target of Edge2 that we will remove, to create the new edges after
		src_temp = boost::source(e_to_be_removed, this->G);
		tgt_temp = boost::target(e_to_be_removed, this->G);
		
		// we remove Edge2 which has to be replaced
		boost::remove_edge(e_to_be_removed, this->G);
		
		// we connect src_temp and tgt_temp to the intersection point intersection_new (so we broke the old edge into two new ones)
		std::tie(this->split_edge, this->edge_inserted) = boost::add_edge(src_temp, intersection_new, this->G);
		this->if_edge_not_inserted();
		this->give_split_edge_properties();				
		std::tie(this->split_edge, this->edge_inserted) = boost::add_edge(intersection_new, tgt_temp, this->G);
		this->if_edge_not_inserted();
		this->give_split_edge_properties();
		
		// we move the new into old to do next loop step
		intersection_old = intersection_new;
	}
	
	// last thing: connect the last intersection point with new_target of Edge1
	// and this is also the add of Edge1 in the graph if there aren't intersections with other edges
	std::tie(this->new_edge, this->edge_inserted) = boost::add_edge(intersection_old, this->new_target, this->G);
	this->give_new_edge_properties();	//the fracture number of Edge1
};	//refine_graph


// la funzione compare dev'essere un qualcosa che ha un call operator, o una funzione esterna a caso. Altrimenti una lambda function va bene.
template <typename Graph>
void reader_Formaggia<Graph>::order_intersections(){
	if(this->Edge1.first < this->Edge1.second)
		std::sort	(this->intersections.begin(),
					this->intersections.end(),
					//lambda function before named "src_less_than_tgt"
					[] (std::pair<point<2>, Edge_desc> intersection_vector_elem1,
						std::pair<point<2>, Edge_desc> intersection_vector_elem2)
						{return intersection_vector_elem1.first < intersection_vector_elem2.first;}
						);
	else
		std::sort	(this->intersections.begin(),
					this->intersections.end(),
					//lambda function before named "src_greater_than_tgt"
					[] (std::pair<point<2>, Edge_desc> intersection_vector_elem1,
						std::pair<point<2>, Edge_desc> intersection_vector_elem2)
						{return intersection_vector_elem1.first > intersection_vector_elem2.first;}
						);
};	//order_intersections

#endif	//HH_READER_FORMAGGIA_CLASS_HH
