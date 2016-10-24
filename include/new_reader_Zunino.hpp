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

//The struct that will be used as property for vertices in the declaration of the adjacency list
//It will have the same types of source_data or target_data, but with generic names
struct Zunino_vertex_data{
	BGLgeom::point<3> COORD;
};
/*
struct Zunino_source_data{
	BGLgeom::point<3> SRC;
};

struct Zunino_target_data{
	BGLgeom::point<3> TGT;
};
*/

struct Zunino_edge_data{
	double capacity;
	double length;
};

struct Zunino_topological_data{
	unsigned int src, tgt;
};

template 	<typename Zunino_vertex_data,
			//typename Zunino_target_data,
			typename Zunino_edge_data,
			typename Zunino_topological_data>
class Zunino_reader : public new_reader_class<Zunino_vertex_data, Zunino_edge_data, Zunino_topological_data> {
	private:
		BGLgeom::point<3> SRC,TGT; 												// they will store vertices coordinates	  
		unsigned int src, tgt; 											// they will read source and target of each edge
		unsigned int edge_num;											// dummy variable;
		double capacity, length;											// they will store diameter and length of the edge
		
	public:
		Zunino_reader(std::string _filename) : new_reader_class<Zunino_vertex_data, Zunino_edge_data, Zunino_topological_data>(_filename),
												SRC(), TGT(), src(), tgt(), edge_num(), capacity(), length() {};
		
	
		virtual void read_line(std::istringstream & iss_line){
			iss_line >> edge_num >> src >> tgt >> capacity >> length >> SRC >> TGT;
		}
		
		virtual Zunino_vertex_data get_source_data(){
			return Zunino_vertex_data{SRC};
		}
		
		virtual Zunino_vertex_data get_target_data(){
			return Zunino_vertex_data{TGT};
		}
		
		virtual Zunino_edge_data get_edge_data(){
			return Zunino_edge_data{capacity, length};
		}
		
		virtual Zunino_topological_data get_topological_data(){
			return Zunino_topological_data{src, tgt};
		}
		
};	//Zunino_reader

#endif
