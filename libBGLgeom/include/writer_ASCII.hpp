/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	writer_ASCII.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Base abstract class to write infos about the graph on tabular files.

	This abstract class provides the user some methods and functionality
	to write data on file from the graph
*/
#ifndef HH_WRITER_ASCII_HH
#define HH_WRITER_ASCII_HH

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <initializer_list>
#include "data_structure.hpp"

/* Not used here, but the user may need to use boost::source or boost::target,
so in this way he doesn't have to remember to put the header when he derives
the writer */
#include <boost/graph/adjacency_list.hpp>

namespace BGLgeom{

/*!
	@brief	Class handling output operations for a tabular type file
	The user is asked to derived its own writer class from this abstract
	one and to define its own "write_data" method as he wants. This allows
	him to choose the format that suites the best for him, and to choose
	which properties (only him knows which they are, if the are not the
	base ones provided by this library!) to write in the output file
*/
template <typename Graph>
class writer_ASCII{
	public:		
		//! Default Constructor
		writer_ASCII(std::string _filename) : out_file(){
			try{
				out_file.open(_filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening output file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		//! Destructor
		virtual ~writer_ASCII(){};
		
		//! Set the output file
		virtual void set_output(std::string _filename){
			out_file.close();
			try{
				out_file.open(_filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening input file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		//! Closing current file
		virtual void close(){
			out_file.close();
		}
		
		/*!
			@brief It writes headers of columns
			Implemented using initializer_list, so it can accept any number of arguments.
			it puts a tab as a separator of the different columns
		*/
		virtual void put_headers(std::initializer_list<std::string> args){
			typename std::initializer_list<std::string>::iterator init_list_it, init_list_end;
			init_list_it = args.begin();
			init_list_end = args.end();
			for( ; init_list_it != init_list_end; ++init_list_it){
				out_file << *init_list_it << "\t";
				if(init_list_it != init_list_end)
					out_file << "\t";
			}
			out_file << std::endl;
		}
		
		/*!
			@brief	It writes data
			Abstract method. The user has to define how to write the data contained
			in the graph
			@note	The method asks for an edge descriptor: the user can recover the information
					about source and target by using the function boost::source(e,G) and
					boost::target(e,G) of the BGL, and then accessing the vertex properties
					through the vertex descriptor just obtained
					
			@param G The graph
			@param e The edge descriptor of the edge whose information will be written on the file
			
		*/
		virtual void write_data(BGLgeom::Edge_desc<Graph> const& e, Graph const& G) = 0;
		
	protected:
		//! The stream associated to the output file
		std::ofstream out_file;
};	//writer_ASCII

}	//BGLgeom

#endif	//HH_WRITER_ASCII_HH
