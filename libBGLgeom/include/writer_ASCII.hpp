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

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <initializer_list>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

namespace geograph{

/*!
	@brief Class handling output operations for a tabular type file
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
		
		//Ha senso?
		//! Copy constructor
		
		//Ha senso?
		//! Assignment operator
		
		//! Set the output file
		virtual void set_output(std::string _filename){
			out_file.close();	//I close the previous file I opened in the constructor
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
			typename std::initializer_list<string>::iterator init_list_it, init_list_end;
			init_list_it = args.begin();
			init_list_end = args.end();
			for( ; init_list_it != init_list_end; ++init_list_it){
				out_file << *init_list_it; //<< "\t";
				if(init_list_it != init_list_end)	//per non mettere uno spazio in fondo. Da migliorare con counter
					out_file << " ";
			}
			out_file << std::endl;
		}
		
		/*!
			@brief It writes a line of data
		*/
		virtual void write_line(Graph const& G, typename boost::graph_traits<Graph>::edge_descriptor const& e){
			typename boost::graph_traits<Graph>::vertex_descriptor src, tgt;
			src = boost::source(*e_it, G);
			tgt = boost::target(*e_it, G);
			out_file << G[src] << " " << G[tgt] << " " << G[*e_it] << std::endl;
		}
		
		/*!
			@brief It writes all data from the edge and vertex properties
			@detail It writes all properties of the graph, one line for each edge, 
					giving vertex properties as source and target properties of
					that edge
			@pre All properties must have an overload of the output operator in which
					all data concerning the property are displayed in one line, according
					to some known and predefined order
		*/
		//Ha senso?(Se ci sono output strani delle cose base è un casino, o vuol dire che devo preimpostare tutto in quest'ottica). Come si fa? 
		virtual void write_properties(Graph const& G){
			//gestire gli header direttamente da qui, o in data_structure?
			typename boost::graph_traits<Graph>::edge_iterator e_it, e_end;
			typename boost::graph_traits<Graph>::vertex_descriptor src, tgt;
			unsigned int n = num_edges(G);
			unsigned int i = 0;
			for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
				++i;
				src = boost::source(*e_it, G);
				tgt = boost::target(*e_it, G);
				// We print, for each edge: properties of source, properties of target, properties of the edge
				out_file << G[src] << " " << G[tgt] << " " << G[*e_it];
				if(i != n)
					out_file << std::endl;
			}	//for
		};	
		
	protected:
		// The name of the output file
		//std::string filename;	//Non serve a niente avere il filename dentro in realtà, gli si può passare sempre diretto così sono anche sicuro che apro sempre e che ha un nome.
		//! The stream associated to the output file
		std::ofstream out_file;

};	//writer_ASCII

}	//BGLgeom

#endif	//HH_WRITER_ASCII_HH
