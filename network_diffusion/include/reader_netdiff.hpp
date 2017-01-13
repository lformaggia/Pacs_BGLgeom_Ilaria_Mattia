/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file reader_netdiff.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Class to read from input for application on network diffusion
*/

#ifndef HH_READER_NETDIFF_HH
#define HH_READER_NETDIFF_HH

#include <string>
#include "reader_ASCII.hpp"
#include "netdiff_graph_properties.hpp"

namespace NetDiff{

// To allow the compiler find input operator for points
using BGLgeom::operator>>;

//! The concrete class to read ASCII input file for the network diffusion example
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
												
		//! Reading form input
		void get_data(){
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
		
		/*!
			@brief Returning data on the edge
			
			We handle here, instead of in the main, the setting of the linear 
			geometry of the edge
		*/
		NetDiff::Edge_prop get_edge_data(){
			NetDiff::Edge_prop E(diam);
			E.geometry.set_source(SRC);
			E.geometry.set_target(TGT);
			return E;
		}
		
		//! Returning topological data
		NetDiff::Topological_prop get_topological_data(){
			return NetDiff::Topological_prop(src,tgt);
		}
		
	private:
		//! Coordinates of source and target
		NetDiff::Vertex_prop::point_t SRC, TGT;
		/*! 
		@brief Vertex_descriptor for the source
		@remark We use an unsigned int as vertex descriptor since we know 
				(from BGL) that the type of adjacency_list we choose to 
				represent the graph uses unsigned int as vertex descriptor
	*/
		unsigned int src, tgt;
		//! Diameter
		double diam;
		//! Dummy variable to discard unused values
		double discard1, discard2;

};	//reader_netdiff



}	//NetDiff

#endif	//HH_READER_NETDIFF_HH
