/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file reader_base_class.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Base abstract class to read input file and creating the graph

	@detail It contains all the variables needed to read an input file and to
			store a graph. It allows to specify how to read the imput file 
			through the abrstract methods.
*/

#ifndef HH_READER_BASE_CLASS_HH
#define HH_READER_BASE_CLASS_HH

#include <string>
#include <fstream>
#include <sstream>

#include <boost/graph/graph_traits.hpp>

template <typename Graph>
class reader_base_class{
	public:
	
		typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
		typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
	
		//! Default constructor (we need however to initialize the reference)
		reader_base_class(Graph & _G):	G(_G),
										file_name(),
										num_dummy_lines(0),
										line(),
										new_source(),
										new_target(),
										new_edge(),
										edge_inserted() {};
		
		//! Constructor: assign only num_dummy_lines, empty graph
		reader_base_class	(Graph & _G,
							 std::string _file_name,
							 unsigned int _num_dummy_lines):	G(_G),
																file_name(_file_name),
																num_dummy_lines(_num_dummy_lines),
																line(),
																new_source(),
																new_target(),
																new_edge(),
																edge_inserted() {};
		
		//! Default copy constructor
		reader_base_class(reader_base_class const&) = default;
		
		//! Assignement operator
		reader_base_class & operator=(reader_base_class const&) = default;
		
		//! Destructor (needed?)
		virtual ~reader_base_class(){};
		
		//! It allows to set the input file
		virtual void set_input_file(std::string _file_name){
			file_name = _file_name;
		};
		
		//! It deletes the whole graph. This in order to free memory after the graph has been read.
		virtual void release_graph(){
			G.clear();
		};
		
		//! Getting the graph (once built)
		virtual Graph get_graph(){ return G; };		//così però lo passo per copia.. se è grosso?
		
		//! Read the input file
		virtual void read_input_file();
		
		//! It ignores the first n lines, that are headers, in the input file
		virtual void ignore_dummy_lines(std::ifstream & file){
			std::string dummy;
			for(std::size_t i = 0; i < num_dummy_lines; ++i)
				std::getline(file, dummy);
		};
		
		//! It describes how to read each line in the input file, and in which variables to store the data
		virtual void read_data_from_line(std::istringstream & temp) = 0;
		
		//! It build the graph one edge at a time, called many times from an external loop
		virtual void build_graph() = 0;
		
		//! It assign properties to new vertices in the rigth way. It has to be called in build_graph()!
		virtual void give_vertex_properties() = 0;
		
		//! It assign properties to the new edge in the rigth way. It has to be called in build_graph()!
		virtual void give_edge_properties() = 0;
		
	protected:
		/*!
			@brief A reference is used to represent the Graph.
			@detail Using a reference allows us not to copy the whole graph outside
					 the class once finished to read data and to build the graph.
					 In this way we build the pre-existent graph (created in the main)
					 while reading the input file. We do not use extra memory.
		*/
		Graph & G;
		//! The string in which is stored the name of the input file to read.
		std::string file_name;
		//! The numbers of initial lines (headers) that the reader has to skip to read useful data
		unsigned int num_dummy_lines;
		//! This will contain one line of the input file, to parse
		std::string line;
		//! The vertex descriptor for the source of the new edge added
		Vertex_desc new_source;
		//! The vertex descriptor for the target of the new edge added
		Vertex_desc new_target;
		//! The edge descriptor for the new edge
		Edge_desc new_edge;
		/*! 
			@brief Bool returned in a pair with an edge descriptor by add_edge function.
			@detail It is true if the edge was succesfully inserted, false otherwise 
		*/
		bool edge_inserted;
		//bool vertices_index_already_present		//va inizializzato!		
};

template <typename Graph>
void reader_base_class<Graph>::read_input_file(){

	std::ifstream file(file_name.c_str());
	
	//controllo che apertura file sia andata a buon fine
	
	this->ignore_dummy_lines(file);
	
	while(!file.fail() && !file.eof()){
		std::getline(file, line);
		if(line.empty())
			continue;
		std::istringstream temp(line);
		this->read_data_from_line(temp);
		if(!temp.fail()){
			this->build_graph();		//le proprietà magari non riesco a darle di fila così lineari, vedi intersezioni di Formaggia.
			//this->give_vertex_properties();
			//this->give_edge_properties();
		}	//if
	}	//while
};

#endif	//HH_READER_BASE_CLASS_HH
