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
#include <iostream>

//! Forward declaration of the struct that will contain the data readed from a line
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
			in_file.open(filename);
		};
		
		//! Copy constructor
		new_reader_class(new_reader_class const&) = default;
		
		//! Assignment operator
		new_reader_class & (new_reader_class const&) = default;
		
		//! Set the input file to read
		virtual void set_input(std::string _filename){
			filename = _filename;
			in_file.open(filename);
			file_is_opened = true;
		}
		
		/*! 
			@brief Ignore n lines of the input code that the user knows he has not to read
			@remark It sets the file stream n lines after the previous position
		*/
		virtual void ignore_dummy_lines(unsigned int const& n){
			if(!file_is_opened){
				std::cerr << "Error: input file not opened for reading" << std::endl;
				return;
			}
			std::string dummy;
			for(std::size_t i = 0; i < n; ++i)
				std::getline(file, dummy);
		}
		
		//! Reads the data from one single line. It has to be specified by the user
		virtual data_from_line read_line() = 0;
	
	protected:
		std::string filename;
		std::ifstream in_file;
		bool file_is_opened;
};	//new_reader_class

#endif //HH_NEW_READER_CLASS_HH
