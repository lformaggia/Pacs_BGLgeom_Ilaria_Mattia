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
	
	The user may not need topological data. He can use this
	empty struct as template parameter in its concrete 
	reader class
*/
struct no_topological_data {};

/*!
	@brief	Abstract class to read a tabular ASCII file and get data from it	
	
	The user will have to define its own concrete reader inheriting from this
	one.	
	Through the function get_data() (where the user has to specify how to read 
	the input file, i.e. he has to specify the sequence of data read by the 
	fstream attribute @a in_file) data from file can be read. This data will 
	be first of all put in some private attributes declared by the user in his 
	own reader class; then, they will be output packed in the structs used as 
	vertex and edge properties in the graph. The user has to take care of 
	specifying how to pack in the right way the data in the right struct; 
	we provide three abstract method to do this: one outputs the struct that 
	will be the source vertex property, one the struct that will be the target 
	vertex property, and one the struct that will be edge property.
	While doing this operation, in some cases it may be useful for the user to 
	take also care of setting up the geometry of the edges before outputting the 
	edge property.
	
	
	@param Vertex_prop The vertex property used for the graph
	@param Edge_prop The edge property used for the graph
	@param Topological_data A struct where the user can put the topological
							information read from the file, such as the 
							indexes of vertices and edges.
*/
template <typename Vertex_prop,
		  typename Edge_prop,
		  typename Topological_data = no_topological_data>
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
			@brief	Gets data form input file
			
			Specify here how to read your input file. Declare as private attribute
			of your derived class each variable you will use to read the data. 
		*/		
		virtual void get_data() = 0;
		
		//! Packs the data in the struct that will be the edge property and outputs it
		virtual Edge_prop get_edge_data() = 0;
		
		//! Packs the data in the struct that will be the source vertex property and outputs it
		virtual Vertex_prop get_source_data() = 0;
		
		//! Packs the data in the struct that will be the target vertex property and outputs it
		virtual Vertex_prop get_target_data() = 0;
		
		/*! 
			@brief	Packs the data in a struct that will be used for topological
					purpouse and outputs it
					
			Leave this method blank, doing nothing, if you are not reading,
			or if you don't need, topological data
		*/
		virtual Topological_data get_topological_data() = 0;
	
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
