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
#include "generic_point.hpp"

template <typename Graph>
class reader_Formaggia final: public reader_base_class<Graph> {
	private:
		//! It stores the fracture number of the new edge
		unsigned int frac_number;
		//! The coordinates of the extremes of the new edge
		point<2> SRC, TGT;
		
	public:
		//! Default constructor (we need however to initialize the reference to the graph)
		reader_Formaggia(Graph & _G) : reader_base_class<Graph>(_G), frac_number(0), SRC(), TGT() {};
		
		//! Constructor
		reader_Formaggia	(Graph & _G,
							std::string _file_name,
							unsigned int _num_dummy_lines) : 	reader_base_class<Graph>(_G, _file_name, _num_dummy_lines),
																frac_number(0),
																SRC(),
																TGT() {};
		
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
			
			
		};
		
		
		
};		//reader_Formaggia


#endif	//HH_READER_FORMAGGIA_CLASS_HH
