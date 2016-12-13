/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file reader_netdiff.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Class to read form input for network diffusion
	
	@detail Application concerning a diffusion problem on a network
	
	@remark In this header file the user is asked to implement: a reader class
			that inherits from reader_ASCII, in which the user has to put all 
			variables that will be read from input file and override all
			abstract methods
*/

#ifndef HH_READER_NETDIFF_HH
#define HH_READER_NETDIFF_HH

#include <string>
#include "reader_ASCII.hpp"
#include "netdiff_graph_properties.hpp"

namespace NetDiff{

using BGLgeom::operator>>;	//Per leggere i punti! vedi: http://stackoverflow.com/questions/36286448/no-match-and-cannot-bind-lvalue-errors-while-overloading-operator-with

class reader_netdiff : public BGLgeom::reader_ASCII	<NetDiff::Vertex_prop,
													 NetDiff::Edge_prop,
													 NetDiff::Topological_prop> {
	public:
		//! Constructor
		reader_netdiff(std::string _filename) : BGLgeom::reader_ASCII	<NetDiff::Vertex_prop,
																		 NetDiff::Edge_prop,
																		 NetDiff::Topological_prop>(_filename),
												SRC(),
												TGT(),
												src(),
												tgt(),
												diam(),
												discard1(),
												discard2() {}
												
		// Now the overriding of the abstract method
		//! Nothing to do
		void get_other_data(){};
		
		//! Reading form input
		void get_data_from_line(){
			this->in_file >> discard1 >> src >> tgt >> diam >> discard2 >> SRC >> TGT;
		}
		
		//! Returning data on the source
		NetDiff::Vertex_prop get_source_data(){
			return NetDiff::Vertex_prop(SRC);
		}
		
		//! Returning data on the target
		NetDiff::Vertex_prop get_target_data(){
			return NetDiff::Vertex_prop(TGT);
		}
		
		//! Returning data on the edge
		NetDiff::Edge_prop get_edge_data(){
			return NetDiff::Edge_prop(diam);
		}
		
		//! Returning topological data
		NetDiff::Topological_prop get_topological_data(){
			return NetDiff::Topological_prop(src,tgt);
		}
		
	private:
		//! Coordinates of source and target
		// oppure: BGLgeom::point<3> SRC, TGT; //????
		NetDiff::Vertex_prop::point_t SRC, TGT;
		//! Vertex descriptors for source and target
		unsigned int src, tgt;
		//! Diameter
		double diam;
		//! Dummy variable to discard unused values
		double discard1, discard2;


};	//reader_netdiff



}	//NetDiff

#endif	//HH_READER_NETDIFF_HH
