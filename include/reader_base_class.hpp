/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
* \file reader_base_class.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sept, 2016
* \brief Base abstract class to read input file and creating the graph
*/

#ifndef HH_READER_BASE_CLASS_HH
#define HH_READER_BASE_CLASS_HH

#include <string>
#include <ifstream>
#include <istringstream>

#include <boost/graph/graph_traits.hpp>

template <typename Graph>
class reader_base_class{
	public:
	
		typedef typename graph_traits<Graph>::vertex_descriptor Vertex_desc;
		typedef typename graph_traits<Graph>::edge_descriptor Edge_desc;
	
		//! Default constructor
		reader_base_class(): G(),
							 file_name(),
							 num_dummy_lines(0),
							 line(),
							 new_source(),
							 new_target(),
							 new_edge(),
							 edge_inserted() {};
		
		//! Constructor: assign only num_dummy_lines, empty graph
		reader_base_class(std::string _file_name, unsigned int _num_dummy_lines):	G(),
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
		
		//! Getting the graph (once built)
		Graph get_graph(){ return G; };		//così però lo passo per copia.. se è grosso?
		
		//! Read the input file
		void read_input_file();
		
		//! It ignores the first n lines, that are headers, in the input file
		// la metto qui? Perché fa sempre la stessa cosa, cambia solo la num_dummy_line
		virtual void ignore_dummy_lines(std::ifstream const& file){
			std::string dummy;
			for(std::size_t i = 0; i < num_dummy_lines; ++i)
				getline(file, dummy);
		};
		
		//! It describes how to read each line in the input file, and in which variables to store the data
		virtual void read_data_from_line(std::istringstream const& temp) = 0;
		
		//! It adds a new vertex in the graph and returns the corrispondent vertex_descriptor by reference
		//virtual void create_vertex(Vertex_desc & v) = 0;
		
		//! It build the graph one edge at a time, called many times from an external loop
		virtual void build_graph() = 0;
		
		virtual void give_vertex_properties(Vertex_desc const& new_source, Vertex_desc const& new_target) = 0;
		
		//virtual void create_edge() = 0;
		
		virtual void give_edge_property(Edge_desc const& e) = 0;
		
	protected:
		Graph G;		//maybe pointer???
		std::string file_name;
		unsigned int num_dummy_lines;
		std::string line;
		Vertex_desc new_source;
		Vertex_desc new_target;
		Edge_desc new_edge;
		bool edge_inserted;
		//bool vertices_index_already_present		//va inizializzato!		
};

template <typename Graph>
void reader_base_class<Graph>::read_input_file(){

	std::ifstream file(file_name.c_str());
	
	//controllo che apertura file sia andata a buon fine
	
	this->ignore_dummy_lines(file, num_dummy_lines);
	
	while(!file.fail() && !file.eof()){
		std::getline(file, line);
		if(line.empty())
			continue;
		std::istringstream temp(s);
		this->read_data_from_line(temp);
		if(!temp.fail())
			//aggiungo nodi e arco corrente
			this->build_graph();
		
			//creo i nodi
			//this->create_vertex(source);
			
			this->give_vertex_properties(new_source, new_target);
			this->give_edge_properties(e);
	}
};

#endif	//HH_READER_BASE_CLASS_HH
