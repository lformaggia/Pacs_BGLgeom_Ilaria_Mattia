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

#include "data_structure.hpp"

//The struct that will be used as property for vertices in the declaration of the adjacency list
//It will have the same types of source_data or target_data, but with generic names
//struct Zunino_vertex_data{
//	BGLgeom::point<3> COORD;
//};
/*
struct Zunino_source_data{
	BGLgeom::point<3> SRC;
};

struct Zunino_target_data{
	BGLgeom::point<3> TGT;
};
*/

//since I'm deriving from the base data strcuture provided by BGLgeom libreary, the new class Zunino_edge_data is no more an aggragate, and then it can't be initialized with an initializer list. It is needed to define a constructor, a copy constructor, or a move constructor within this class.
template <unsigned int dim>
struct Zunino_edge_data : public BGLgeom::BGLgeom_edge_property<dim>{
	double capacity;
	double length;
	
	Zunino_edge_data() : BGLgeom::BGLgeom_edge_property<dim>(), capacity(.0), length(.0) {};
	
	Zunino_edge_data(double _capacity, double _length) : BGLgeom::BGLgeom_edge_property<dim>(), capacity(_capacity), length(_length) {};
	
	Zunino_edge_data & operator=(Zunino_edge_data const&) = default;
};

struct Zunino_topological_data{
	unsigned int src, tgt;
};

template 	<//typename Vertex_data,
			//typename Zunino_target_data,
			typename Edge_data,
			typename Topological_data>
class Zunino_reader : public BGLgeom::new_reader_class<BGLgeom::BGLgeom_vertex_property<3>, Edge_data, Topological_data> {
	private:
		BGLgeom::BGLgeom_vertex_property<3>::point_t SRC,TGT; 				// they will store vertices coordinates	  
		unsigned int src, tgt; 											// they will read source and target of each edge
		unsigned int edge_num;											// dummy variable;
		double capacity, length;										// they will store diameter and length of the edge
		
	public:
		Zunino_reader(std::string _filename) : BGLgeom::new_reader_class<BGLgeom::BGLgeom_vertex_property<3>, Edge_data, Topological_data>(_filename),
												SRC(), TGT(), src(), tgt(), edge_num(), capacity(), length() {};		
	
		virtual void get_data_from_line(){
			this->in_file /*iss_line*/ >> edge_num >> src >> tgt >> capacity >> length >> SRC >> TGT;
		}
		
		virtual BGLgeom::BGLgeom_vertex_property<3> get_source_data(){
			return BGLgeom::BGLgeom_vertex_property<3>{SRC};
		}
		
		virtual BGLgeom::BGLgeom_vertex_property<3> get_target_data(){
			return BGLgeom::BGLgeom_vertex_property<3>{TGT};
		}
		
		virtual Edge_data get_edge_data(){
			return Edge_data(capacity, length);
		}
		
		virtual Topological_data get_topological_data(){
			return Topological_data{src, tgt};
		}
		
};	//Zunino_reader

#endif
