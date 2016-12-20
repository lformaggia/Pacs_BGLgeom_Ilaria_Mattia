/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file writer_pts.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Base class to write a .pts file

	@detail
*/

#ifndef HH_WRITER_PTS_HH
#define HH_WRITER_PTS_HH

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>

#include "data_structure.hpp"

namespace BGLgeom{

/*!
	@brief	Classe to export infos about the graph on a .pts file
	@detail In particular, this writer wants to write the mesh genarated on the
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
		//! Default constructor
		writer_pts(std::string & _filename) : out_file(), filename(_filename) {
			if(_filename.substr(_filename.length()-3, 3) != "pts")
				std::cerr << "Warning! The output file does not have 'pts' extension." << std::endl;
			/*else
				*/
		}
		
		//! Destructor
		virtual ~writer_pts(){
			out_file.close();	//necessary? Really not
		}
		
		//! Changing the filename to open a different ofstream
		void set_filename(std::string const& _filename){
			out_file.close();	//close the previous one
			out_file.open(_filename.c_str());
		}
		
		//! It exports the mesh and the info contained in the graph in an pts file
		void
		export_pts(Graph const& G){
			BGLgeom::Edge_iter<Graph> e_it, e_end;
			BGLgeom::Vertex_desc<Graph> src, tgt;
			for(std::size_t i = 0; i < num_bc; ++i){
				std::string temp_filename(filename);
				// Changing filename if needed
				if(num_bc > 1){
					// Renaming filename
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
				out_file << "BEGIN_LIST" << std::endl;
				for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
					this->export_edge(G, *e_it, src, tgt, i);
				}
				out_file << "END_LIST";
				out_file.close();
			}	//for			
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
			BGLgeom::write_point_pts<dim>(out_file, G[src].coordinates);
			out_file << std::setw(10) << "start" << std::endl;
			BGLgeom::write_point_pts<dim>(out_file, G[tgt].coordinates);
			out_file << std::setw(8) << "end" << std::endl;
			write_mesh_pts<dim>(out_file, G[e].mesh);	//with overload of operator<< for mesh
			out_file << "END_ARC" << std::endl;
		}	//export_edge
};	//writer_pts

}	//BGLgeom

#endif	//HH_WRITER_PTS_HH
