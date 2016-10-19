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

//! Forward declaration of the struct that will contain the data read from a line
struct data_from_line{};

/*!
	@brief Abstract class that implements the functionality to read a file
*/
class new_reader_class{
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
		};
		
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
					of the class and it is updated after every call of read_line(). \n
					The user must use the iss_line to read data from, when defining his own reader
					class
		*/
		virtual void get_data_from_line(data_from_line & D) = 0;
	
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
