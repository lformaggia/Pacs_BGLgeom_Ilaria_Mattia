/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	test_writer_ASCII.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Definition of the concrete class that inherits from writer_ASCII
	
	We define a test edge property for the graph. Then the concrete 
	writer for that type of graph is defined.
*/

#ifndef HH_TEST_WRITER_ASCII_HH
#define HH_TEST_WRITER_ASCII_HH

#include <vector>
#include <string>
#include "data_structure.hpp"
#include "graph_builder.hpp"
#include "writer_ASCII.hpp"
#include "linear_geometry.hpp"

namespace{

//! Edge property we will assign to the graph
struct Edge_prop_ASCII : public BGLgeom::Edge_base_property<BGLgeom::linear_geometry<2>,2> {
	double foo_double;
	std::vector<int> foo_vector;
	std::string foo_string;
	
	//! Default constructor
	Edge_prop_ASCII() : BGLgeom::Edge_base_property<BGLgeom::linear_geometry<2>,2>(),
						foo_double(),
						foo_vector(),
						foo_string() {};
};	//Edge_prop_ASCII

//! Concrete writer_ASCII for the test, and specific for the properties we will have on the graph
template <typename Graph>
class test_writer_ASCII : public BGLgeom::writer_ASCII<Graph> {
	public:
		//! Constructor
		test_writer_ASCII(std::string _filename) : BGLgeom::writer_ASCII<Graph>(_filename){};
		
		//! Defining abstract method
		void
		write_data(BGLgeom::Edge_desc<Graph> const& e, Graph const& G){
			BGLgeom::Vertex_desc<Graph> src = boost::source(e,G);
			BGLgeom::Vertex_desc<Graph> tgt = boost::target(e,G);
			this->out_file << G[e].index << "\t\t" << G[src].coordinates  << "\t\t" << G[tgt].coordinates
							<< "\t\t" << G[e].foo_double << "\t\t" << G[e].foo_vector[3]
							<< "\t\t" << G[e].foo_string << std::endl;
		}	
};	//test_writer_ASCII

}	//unnamed namespace

#endif	//HH_TEST_WRITER_ASCII_HH
