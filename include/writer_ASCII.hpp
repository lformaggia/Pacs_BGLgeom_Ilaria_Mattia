/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file writer_ASCII.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Base abstract class to write infos about the graph on tabular files.

	@detail This abstract class provides the user some methods and functionality
			to write data on file from the graph
*/
#ifndef HH_WRITER_ASCII_HH
#define HH_WRITER_ASCII_HH

#include <string>
#include <fstream>
#include <cstdlib>
#include <initializer_list>

namespace BGLgeom{

/*!
	@brief Class handling output operations for a tabular type file
*/
template <>
class writer_ASCII{
	public:
		//! Default constructor
		writer_ASCII() : filename(), out_file(){};
		
		//! Constructor
		writer_ASCII(std::string _filename) : filename(_filename), out_file(){
			try{
				out_file.open(filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening output file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		//Ha senso?
		//! Copy constructor
		
		//Ha senso?
		//! Assignment operator
		
		//! Set the output file
		virtual void set_output(std::string _filename){
			filename = _filename;
			try{
				out_file.open(filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening input file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/*!
			@brief It writes headers of columns
			@detail Implemented using initializer_list, so it can accept any number of arguments
		*/
		virtual void put_headers(std::initializer_list<std::string> args){
		
		}
		
		//! It writes a line of data
		virtual void write_line() = 0;
		
		//! It writes all data from the edge and vertex properties
		//Ha senso?(Se ci sono output strani delle cose base è un casino, o vuol dire che devo preimpostare tutto in quest'ottica). Come si fa? 
		virtual void write_properties() = 0;	
		
	protected:
		//! The name of the output file
		std::string filename;	//Non serve a niente avere il filename dentro in realtà, gli si può passare sempre diretto così sono anche sicuro che apro sempre e che ha un nome.
		//! The stream associated to the output file
		std::ofstream out_file;

};	//writer_ASCII

}	//BGLgeom

#endif	//HH_WRITER_ASCII_HH
