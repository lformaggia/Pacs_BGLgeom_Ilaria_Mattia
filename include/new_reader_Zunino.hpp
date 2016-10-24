/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file new_reader_Zunino.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Class for reading from Zunino files
	
	@detail In this header file the user has to implement: \n
			- A struct to handle data which will be put as edge_property \n
			- A struct to handle data which will be put as vertex_property \n
			- A reader class that inherits from new_reader_class, in which the user has to
				put all variables that will be read from input file and override all
				abstract methods
*/

#ifndef HH_NEW_READER_ZUNINO_HH
#define HH_NEW_READER_ZUNINO_HH

#include "new_reader_class.hpp"
#include "generic_point.hpp"

struct Zunino_vertex_data{
	BGLgeom::point<3> SRC, TGT;
};

struct Zunino_edge_data{
	double capacity;
	double length;
};

struct Zunino_topological_data{
	unsigned int src, tgt;
};

class Zunino_reader : public new_reader_class<Zunino_vertex_data, Zunino_edge_data, Zunino_topological_data> {
	private:
		BGLgeom::point<3> SRC,TGT; 												// they will store vertices coordinates	  
		unsigned int src, tgt; 											// they will read source and target of each edge
		unsigned int edge_num;											// dummy variable;
		double capacity, length;											// they will store diameter and length of the edge
		
	public:
		virtual void get_data_from_line(){
			this->iss_line >> edge_num >> src >> tgt >> capacity >> length >> SRC >> TGT;
		}
		
		virtual Zunino_vertex_data get_vertex_data(){
			return Zunino_vertex_data{SRC, TGT};
		}
		
		virtual Zunino_edge_data get_edge_data(){
			return Zunino_edge_data{capacity, length};
		}
		
		virtual Zunino_topological_data get_topologica_data(){
			return Zunino_topological_data{src, tgt};
		}
		
};	//Zunino_reader

#endif
