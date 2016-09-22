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
* \date Sep 14, 2016
* \brief 
*/

#ifndef HH_READER_BASE_CLASS_HH
#define HH_READER_BASE_CLASS_HH

template <typename Graph>
class reader_base_class{
	protected:
		Graph G;		//maybe pointer???
		unsigned int num_dummy_lines;
		
	public:
	
		//! Default constructor
		reader_base_class(): G(0), num_dummy_lines(0) {};
		
		//! Constructor: assign only num_dummy_lines, empty graph
		reader_base_class(unsigned int _num_dummy_lines):  G(0), num_dummy_lines(_num_dummy_lines) {};
		
		//! Default copy constructor
		reader_base_class(reader_base_class const&) = default;
		
		//! Assignement operator
		reader_base_class & operator=(reader_base_class const&) = default;
		
		//! Destructor (needed?)
		virtual ~reader_base_class(){};
		
		//! Getting the graph (once built)
		Graph get_graph(){ return G; };		//così però lo passo per copia.. se è grosso?
		
		// la metto qui? Perché fa sempre la stessa cosa, cambia solo la num_dummy_line
		virtual void ignore_dummy_lines(std::ifstream const& file){
			std::string dummy;
			for(std::size_t i = 0; i < num_dummy_lines; ++i)
				getline(file, dummy);
		};
		
		virtual void read_data_from_line(std::istringstream temp) = 0;
		
		virtual void create_vertex() = 0;
		
		virtual void give_vertex_properties() = 0;
		
		virtual void create_edge() = 0;
		
		virtual void give_edge_property() = 0;

};

#endif	//HH_READER_BASE_CLASS_HH
