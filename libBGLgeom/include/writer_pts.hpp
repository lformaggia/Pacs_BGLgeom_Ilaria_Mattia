/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	writer_pts.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Base class to write a .pts file
*/

#ifndef HH_WRITER_PTS_HH
#define HH_WRITER_PTS_HH

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>

#include "graph_builder.hpp"
#include "point.hpp"
#include "mesh.hpp"

namespace BGLgeom{

//! Helper function to write a point in a pts file format
template <unsigned int dim>
void
write_point_pts(std::ostream & out, BGLgeom::point<dim> const& P){
	out << std::setw(16);
	for(std::size_t i=0; i<dim-1; ++i)
		out << std::fixed << std::setprecision(8) << P(0,i) << std::setw(16);
	out << P(0,dim-1);
}


/*!
	@brief	Classe to export infos about the graph on a .pts file
	In particular, this writer wants to write the mesh genarated on the
	edges of the graph, in order to be properly used by getFEM code
	@note	If the number of boundary conditions attached to each vertex through 
			the vertex properties (using class boundary_condition of this library)
			is larger than one, this writer will produce a separate pts file for
			each boundary condition. The file will be named "out_file"BC1.pts,
			"out_file"BC2.pts, etc. If there is only one boundary condition the
			output file name is left as the original.
	
	@pre The graph that has to be exported is expected to have at least all the
		properties defined in data_structure.hpp
		
	@param Graph The type of the graph
	@param dim Dimension of the space
	@param num_bc Number of boundary conditions
*/
template <typename Graph, unsigned int dim, unsigned int num_bc = 1>
class writer_pts{
	public:
		//! Default constructor with std::string
		writer_pts(std::string & _filename) : out_file(), filename(_filename) {
			if(filename.substr(filename.length()-3, 3) != "pts")
				std::cerr << "Warning! The output file does not have 'pts' extension." << std::endl;
		}
		
		//! Default constructor with char*
		writer_pts(const char* _filename) : out_file(), filename(_filename) {
			if(filename.substr(filename.length()-3, 3) != "pts")
				std::cerr << "Warning! The output file does not have 'pts' extension." << std::endl;
		}
		
		//! Destructor
		virtual ~writer_pts() {}
		
		//! Changing the filename to open a different ofstream
		void set_filename(std::string const& _filename){
			out_file.close();	//close the previous one
			out_file.open(_filename.c_str());
		}
		
		/*! 
			@brief It exports the mesh and the info contained in the graph in an pts file
			If the parameter add_geometric_info is passed as true, this will print
			in a separate file, named "filename"_moreinfo.pts, the evaluation of the
			first derivative, of the second derivative and of the curvature in 
			corrispondence of the points of the mesh
			@pre	When the second parameter is set true, a parametric mesh is required 
					to be present on the edge. If it is empty, the writer will produce 
					an output evaluating the geometric quantities only in correspondence 
					of source and target
		*/
		void
		export_pts(Graph const& G, bool add_geometric_info = false){
			BGLgeom::Edge_iter<Graph> e_it, e_end;
			BGLgeom::Vertex_desc<Graph> src, tgt;
			for(std::size_t i = 0; i < num_bc; ++i){
				std::string temp_filename(filename);
				// Changing filename if needed
				if(num_bc > 1){
					std::ostringstream temp;
					temp << "_BC" << i+1;
					temp_filename.insert(filename.length()-4, temp.str());
				}
				// Opening file
				try{
					out_file.open(temp_filename.c_str());
				} catch(std::exception & e) {
					std::cerr << "Writer_pts: error while opening output file. In particular: " << e.what() << std::endl;
					exit(EXIT_FAILURE);
				}
				// Writing on file
				std::cout << "Writing pts file ..." << std::endl;
				out_file << "BEGIN_LIST" << std::endl;
				for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
					this->export_edge(G, *e_it, src, tgt, i);
				}
				out_file << "END_LIST";
				out_file.close();
			}	//for
			// Adding geometrical information
			if(add_geometric_info){
				// Changing filename
				std::string temp_filename(filename);
				std::ostringstream temp;
				temp << "_moreinfo";
				temp_filename.insert(filename.length()-4, temp.str());
				// Opening file
				try{
					out_file.open(temp_filename.c_str());
				} catch(std::exception & e) {
					std::cerr << "Writer_pts: error while opening output file. In particular: " << e.what() << std::endl;
					exit(EXIT_FAILURE);
				}
				// Writing on file
				out_file << "BEGIN_LIST" << std::endl;
				for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
					this->export_info(G, *e_it);
				}
				out_file << "END_LIST";
				out_file.close();				
			}	//if				
		}	//export_pts
		
	private:
		//! The stream associated to the output file
		std::ofstream out_file;
		//! The name of the ouput file
		std::string filename;
	
		/*!
			@brief Inner method to output a single edge
			
			@param G	The Graph
			@param e	Edge_descriptor for the current edge
			@param src	Vertex_descriptor for the current source
			@param tgt	Vertex_descriptor for the current target
			@param i	Iteration on the external for loop, needed to print out
						the correct boundary condition on the rigth file
		*/
		void
		export_edge	(Graph const& G,
					 BGLgeom::Edge_desc<Graph> const& e,
					 BGLgeom::Vertex_desc<Graph> & src,
					 BGLgeom::Vertex_desc<Graph> & tgt,
					 unsigned int const& i){
			out_file << "BEGIN_ARC" << std::endl;
			src = boost::source(e, G);
			tgt = boost::target(e, G);
			out_file << G[src].BC[i] << std::endl;
			out_file << G[tgt].BC[i] << std::endl;
			out_file<< std::setw(8) << std::setprecision(2) << G[e].index;
			write_point_pts<dim>(out_file, G[src].coordinates);
			out_file << std::setw(10) << "start" << std::endl;
			out_file << std::setw(8) << std::setprecision(2) << G[e].index;
			write_point_pts<dim>(out_file, G[tgt].coordinates);
			out_file << std::setw(8) << "end" << std::endl;
			// Writing the mesh (without source and target)
			if(G[e].mesh.real.size() > 0){	// A mesh is present on the edge
				for(std::size_t i = 1; i < G[e].mesh.real.size()-1; ++i){
					out_file << std::setw(8) << std::setprecision(2) << G[e].index;
					write_point_pts<dim>(out_file, G[e].mesh.real[i]);
					out_file << std::setw(10) << "point" << std::endl;
				}	//for
			}	//if
			out_file << "END_ARC" << std::endl;
		}	//export_edge
		
		/*!
			@brief	Exporting to a separate pts file additional geometric infos
			
			It exports for each edge this kind of info: \n
			- the three component of the first derivative; \n
			- the three component of the second derivative; \n
			- the evaluation of the curvature. \n
			All this info are computed in correspondence of the points of the
			mesh present on the edge
			@note	If a parametric mesh is not present on the edge, this will 
					evaluate the geometric properties only in corrispondence of 
					source and target
		*/
		void
		export_info(Graph const& G,
					BGLgeom::Edge_desc<Graph> const& e){
			out_file << "BEGIN_ARC" << std::endl;
			if(G[e].mesh.parametric.size() < 2){	// There isn't a parametric mesh on the edge
				// Evaluation only in source (t=0) and target (t=1)
				for(int t = 0; t < 2; ++t){
					out_file << std::setw(8) << std::setprecision(2) << G[e].index;
					write_point_pts<dim>(out_file, G[e].geometry.first_der(t));
					write_point_pts<dim>(out_file, G[e].geometry.second_der(t));
					out_file << std::setw(16) << std::setprecision(8) << G[e].geometry.curvature(t);
					out_file << std::endl;
				}	//for
			} else {
				for(std::size_t i = 0; i < G[e].mesh.parametric.size(); ++i){
					out_file << std::setw(8) << std::setprecision(2) << G[e].index;
					write_point_pts<dim>(out_file, G[e].geometry.first_der(G[e].mesh.parametric[i]) );
					write_point_pts<dim>(out_file, G[e].geometry.second_der(G[e].mesh.parametric[i]) );
					out_file << std::setw(16) << std::setprecision(8) << G[e].geometry.curvature(G[e].mesh.parametric[i]);
					out_file << std::endl;
				}	//for
			}	//else
			out_file << "END_ARC" << std::endl;		
		}	//export_info
};	//writer_pts

}	//BGLgeom

#endif	//HH_WRITER_PTS_HH
