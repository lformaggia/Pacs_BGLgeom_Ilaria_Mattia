/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file new_reader_class.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Base abstract class to read input file

	@detail This abstract class provides the user some methods and functionality
			to read data form the input file and check errors.
*/

#ifndef HH_NEW_READER_CLASS_HH
#define HH_NEW_READER_CLASS_HH

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <exception>

/*! 
	@brief An empty struct to handle the case the user do not need to store topological data
	@detail Inside this the user may put data as vertex and edge descriptor for the connettivity of the graph
*/
struct no_topological_data {};

/*!
	@brief Abstract class that implements the functionality to read a file and get data from it	
	@detail The users has to specify, in the derived class, all variables he need in
			order to store information read from the input file. Then, through the definition
			of Edge_data_structure and Vertex_data_structure, he can get separately all the
			information to put on edges and vertices
	
	@param Edge_data_structure A struct where the user has to define type and name
								of the variables he needs to append to vertices as
								vertex bundled property
	@param Vertex_data_structure A struct where the user has to define type and name
								of the variables he needs to append to edge as
								edge bundled property
	@pre It may be useful to declare a friend operator>> to help the reader read the data
*/
template 	<typename Edge_data_structure,
			typename Vertex_data_structure,
			typename Topological_data_structure = no_topological_data>
class new_reader_class {
	public:
		//! Default constructor
		new_reader_class() : filename(), in_file(), file_is_opened(false) {};
		
		//! Constructor
		new_reader_class(std::string _filename) : filename(_filename), file_is_opened(true) {
			try{
				in_file.open(filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening input file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		//! Copy constructor
		new_reader_class(new_reader_class const&) = default;
		
		//! Assignment operator
		new_reader_class & (new_reader_class const&) = default;
		
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
				std::string dummy;
				for(std::size_t i = 0; i < n; ++i)
					std::getline(in_file, dummy);
		}
		
		//! Reads one line and put it into a istringstream
		virtual void read_line(){
			std::getline(in_file, line);
			iss_line.str(line);
			if(iss_line.fail()){
				std::cerr << "Error while transferring the line read into istringstream." << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/*!
			@brief Reads the data from one single line. It has to be specified by the user
			@detail It reads data form the istringstream iss_line that is defined as an attribute
					of the class and it is updated after every call of read_line().
		*/
		virtual void get_data_from_line() = 0;
		
		//! A method to get the right data to append to an edge
		virtual Edge_data_structure get_edge_data() = 0;
		
		//! A method to get the right data to append to a vertex
		virtual Vertex_data_structure get_vertex_data() = 0;
		
		//! A method to get the right topological data from a line
		virtual Topological_data_structure get_topological_data() = 0;
	
	protected:
		//! The name of the file to be read
		std::string filename;
		//! File stream to handle the input file
		std::ifstream in_file;
		//! String in which the data read from a line are put
		std::string line;
		//! Data put in line are converted in istringstream to be got by the user
		std::istringstream iss_line;
		
};	//new_reader_class

#endif //HH_NEW_READER_CLASS_HH
