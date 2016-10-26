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
template 	<typename Vertex_data_structure,
			//typename Target_data_structure,
			typename Edge_data_structure,
			typename Topological_data_structure = no_topological_data>
class new_reader_class {
	public:
		//! Default constructor
		new_reader_class() : filename(), in_file(), line() {};
		
		//! Constructor
		new_reader_class(std::string _filename) : filename(_filename), in_file(), line() {
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
		new_reader_class & operator=(new_reader_class const&) = default;
		
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
		
		/*! 
			@brief Reads one line and put the data read from the istringstream in the variables defined
					in the attributes of the derived class defined by the user
		*/		
		virtual void get_data_from_line() = 0;
		/*{
			std::istringstream iss_line(line);		
			
			std::getline(in_file, line);
				
			if(iss_line.fail()){
				std::cerr << "Error while transferring the line read into istringstream." << std::endl;
				exit(EXIT_FAILURE);
			}
			
			//char a_caso;
			this->read_line(iss_line);	//non mi piace tanto... 
			//in_file.get(a_caso);
		}*/
		
		//! To know outside the class if we have reached the end of file
		virtual bool is_eof(){
			char find_eof;		
			in_file.get(find_eof);	//nell'ultima riga c'è end of line e poi eof, quindi devo leggere ogni volta due caratteri per sapere se sono in fondo
			if(in_file.peek() == std::ifstream::traits_type::eof()){
				in_file.close();
				return true;
			} else {
				in_file.seekg(-1, in_file.cur); 
				return false;
			}
		}
		
		/*!
			@brief Reads the data from one single line. It has to be specified by the user
			@detail It reads data from the istringstream iss_line that is created and
					initialize in the method get_data_from_line() every time that method is called.
		*/
		//virtual void read_line(std::istringstream & iss_line) = 0;
		
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
		//! String in which the data read from a line are put
		std::string line;
		//! Data put in line are converted in istringstream to be got by the user
		//std::istringstream iss_line;
		
};	//new_reader_class

#endif //HH_NEW_READER_CLASS_HH
