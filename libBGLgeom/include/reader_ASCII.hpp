/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	reader_ASCII.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Base abstract class to read input file with tabular ASCII 
			format
*/

#ifndef HH_READER_ASCII_HH
#define HH_READER_ASCII_HH

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <exception>

namespace BGLgeom{

/*! 
	@brief	Empty struct for topological data
	@detail The user may not need topological data. He can use this
			empty struct as template parameter in its concrete 
			reader class
*/
struct no_topological_data {};

/*!
	@brief	Abstract class that implements the functionality to read a file and get data from it	
	@detail The users has to specify, in the derived class, all variables he need in
			order to store information read from the input file. Then, through the definition
			of Edge_data_structure and Vertex_data_structure, he can get separately all the
			information to put on edges and vertices
	
	@param Vertex_data_structure A struct where the user has to define type and name
								of the variables he needs to append to the edges as
								edge bundled property
	@param Edge_data_structure A struct where the user has to define type and name
								of the variables he needs to append to the vertices as
								vertex bundled property
	@param Topological_data_structure A struct where the user has to define type and name
										of the variables he needs to manage the topology
										of the graph while building it. Defaulted to an
										empty struct
*/
template 	<typename Vertex_data_structure,
			typename Edge_data_structure,
			typename Topological_data_structure = no_topological_data>
class reader_ASCII {
	public:
		//! Default constructor
		reader_ASCII() : filename(), in_file(), find_eof() {};
		
		//! Constructor
		reader_ASCII(std::string _filename) : filename(_filename), in_file(), find_eof() {
			try{
				in_file.open(filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening input file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		//! Copy constructor
		reader_ASCII(reader_ASCII const&) = default;
		
		//! Assignment operator
		reader_ASCII & operator=(reader_ASCII const&) = default;
		
		//! Destructor
		virtual ~reader_ASCII() = default;
		
		//! Set the input file to read
		virtual void set_input(std::string _filename){
			filename = _filename;
			try{
				in_file.open(filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening input file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/*! 
			@brief Ignore n lines of the input code that the user knows he has not to read
			@remark It sets the file stream n lines after the previous position
		*/
		virtual void ignore_dummy_lines(unsigned int const& n){
			if(in_file.is_open()){
				std::string dummy;
				for(std::size_t i = 0; i < n; ++i)
					std::getline(in_file, dummy);
			} else{
				std::cerr << "Error: file not open!" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		//! To know outside the class if we have reached the end of file
		virtual bool is_eof(){
			// In the last row there are end of line and then eof, so we need to read each time two characters to know if we reached eof
			in_file.get(find_eof);
			if(find_eof == std::ifstream::traits_type::eof() || in_file.peek() == std::ifstream::traits_type::eof()){
				in_file.close();
				return true;
			} else {
				in_file.seekg(-1, in_file.cur); 
				return false;
			}
		}
		
		/*! 
			@brief	Reads one line and put the data read from the istringstream in the variables defined
					in the attributes of the derived class defined by the user
		*/		
		virtual void get_data() = 0;
		
		//! A method to get the right data to append to an edge
		virtual Edge_data_structure get_edge_data() = 0;
		
		//! A method to get the right data to append to the source
		virtual Vertex_data_structure get_source_data() = 0;
		
		//! A method to get the right data to append to the target
		virtual Vertex_data_structure get_target_data() = 0;
		
		//! A method to get the right topological data from a line
		virtual Topological_data_structure get_topological_data() = 0;
	
	protected:
		//! The name of the file to be read
		std::string filename;
		//! File stream to handle the input file
		std::ifstream in_file;
		//! Helper variable to make the class read properly the end of file
		char find_eof;
};	//reader_ASCII

}	//BGLgeom

#endif //HH_reader_ASCII_HH
