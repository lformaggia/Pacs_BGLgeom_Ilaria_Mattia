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
	
	@detail Implementation of the example on Zunino's graph.
	
	@remark In this header file the user is asked to implement: \n
			- A struct to handle data which will be put as edge_property \n
			- A struct to handle data which will be put as vertex_property \n
			- A reader class that inherits from new_reader_class, in which the user has to
				put all variables that will be read from input file and override all
				abstract methods
			\n
			Notice that all the data structures have to be provided with at least 
			the default constructor and a constructor, and the copy constructor,
			assignment operator and move constructor if needed.
*/

#ifndef HH_NEW_READER_ZUNINO_HH
#define HH_NEW_READER_ZUNINO_HH

#include <string>

#include "new_reader_class.hpp"
//#include "generic_point.hpp"
#include "data_structure.hpp"
//#include "boundary_conditions.hpp"

/* Since I'm deriving from the base data strcuture provided by BGLgeom library, the new class Zunino_edge_data is no more an aggragate, and then it can't be initialized with an initializer list. It is needed to define a constructor, a copy constructor, or a move constructor within this class. */
template <unsigned int dim>
struct Zunino_edge_data : public BGLgeom::Edge_base_property<dim>{
	double capacity;
	double length;
	
	// Default constructor
	Zunino_edge_data() : BGLgeom::Edge_base_property<dim>(), capacity(.0), length(.0) {};
	
	// Constructor
	Zunino_edge_data(double _capacity, double _length) : BGLgeom::Edge_base_property<dim>(), capacity(_capacity), length(_length) {};
	
	// Assignment operator
	Zunino_edge_data & operator=(Zunino_edge_data const&) = default;
};

struct Zunino_topological_data{
	unsigned int src, tgt;
	
	// Default constructor
	Zunino_topological_data() : src(), tgt() {};
	
	// Constructor
	Zunino_topological_data(unsigned int const& _src, unsigned int const& _tgt) : src(_src), tgt(_tgt) {};
};

template <typename Edge_data, typename Topological_data>
class Zunino_reader : public BGLgeom::new_reader_class<BGLgeom::Vertex_base_property<3>, Edge_data, Topological_data> {
	private:
		BGLgeom::Vertex_base_property<3>::point_t SRC,TGT; 			// they will store vertices coordinates	  
		unsigned int src, tgt; 											// they will read source and target of each edge
		unsigned int edge_num;											// dummy variable;
		double capacity, length;										// they will store diameter and length of the edge
		BGLgeom::Vertex_base_property<3>::bc_t BC_src, BC_tgt;
		
	public:
		Zunino_reader(std::string _filename) : 	BGLgeom::new_reader_class	<BGLgeom::Vertex_base_property<3>,
																			Edge_data,
																			Topological_data>(_filename),
												SRC(),
												TGT(),
												src(),
												tgt(),
												edge_num(),
												capacity(),
												length(),
												BC_src(),
												BC_tgt() {};	
												
		virtual void get_other_data(){};
	
		virtual void get_data_from_line(){
			this->in_file /*iss_line*/ >> edge_num >> src >> tgt >> capacity >> length;// >> SRC >> TGT >> BC_src >> BC_tgt;
			BGLgeom::read_point(this->in_file, SRC);
			BGLgeom::read_point(this->in_file, TGT);
			this->in_file >> BC_src >> BC_tgt;
		}
		
		virtual BGLgeom::Vertex_base_property<3> get_source_data(){
			return BGLgeom::Vertex_base_property<3>(SRC, BC_src);
		}
		
		virtual BGLgeom::Vertex_base_property<3> get_target_data(){
			return BGLgeom::Vertex_base_property<3>(TGT, BC_tgt);
		}
		
		virtual Edge_data get_edge_data(){
			return Edge_data(capacity, length);
		}
		
		virtual Topological_data get_topological_data(){
			return Topological_data(src, tgt);
		}
		
};	//Zunino_reader

#endif
