/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	writer_vtp.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Base class to write a .vtp file

	@detail
*/

#ifndef HH_WRITER_VTP_HH
#define HH_WRITER_VTP_HH

#include <string>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPointData.h>
// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

#include "data_structure.hpp"

namespace BGLgeom{

using CellArray_ptr = vtkSmartPointer<vtkCellArray>;
using PolyDataWriter_ptr = vtkSmartPointer<vtkXMLPolyDataWriter>;
using PolyData_ptr = vtkSmartPointer<vtkPolyData>;
using Points_ptr = vtkSmartPointer<vtkPoints>;

template<int dim>
inline void insert_point(double const* P, Points_ptr & points){};

// template specializations for dim=2 and dim=3
template<>
inline void insert_point<2>(double const* P, Points_ptr & points){
	// create an array with 3 components: copy the components of v in the first 2 comp and add 0 as z component
	double PP[3];
	PP[0] = P[0];
	PP[1] = P[1];
	PP[2] = 0.0;
	points -> InsertNextPoint(PP);
};

template<>
inline void insert_point<3>(double const* P, Points_ptr & points){
	points -> InsertNextPoint(P);
};

/*!
	@brief	Class to export a .vtp file
	@detail This writer outputs a .vtp file containing the graphical representation of the graph 	
	@pre	The graph that has to be exported is expected to have at least all the
			properties defined in data_structure.hpp
		
	@param Graph The type of the graph
	@param dim The dimension of the space
*/
template <typename Graph, unsigned int dim>
class writer_vtp{
	
	public:
	
		//! Default constructor 
		writer_vtp() {
			  // Write the file
  			  writer = PolyDataWriter_ptr::New();
  			  writer_vertices = PolyDataWriter_ptr::New();
  			  
  			  std::string filename("../data/output_vtk.vtp");
  			  writer -> SetFileName(filename.c_str());
  			  
  			  lines = CellArray_ptr::New();
			  points = Points_ptr::New();
			  vertices = Points_ptr::New();
		
			  if(filename.substr(filename.length()-3, 3) != "vtp")
				 std::cerr << "Warning! The output file does not have 'vtp' extension." << std::endl;
				
			  std::string vertex_string("_vertices.vtp");
			  std::string filename_vertices(filename);
			  filename_vertices.insert(filename_vertices.end()-4, vertex_string.begin(), vertex_string.begin()+10);
				
			  writer_vertices -> SetFileName(filename_vertices.c_str());
		}
		
		
		//! Constructor with std::string
		writer_vtp(std::string _filename) : filename(_filename) {
			  // Write the file
  			  writer = PolyDataWriter_ptr::New();
  			  writer -> SetFileName(filename.c_str());

  			  lines = CellArray_ptr::New();
			  points = Points_ptr::New();
			  vertices = Points_ptr::New();
			  
			  // Check on the extension of _filename
			  if(filename.substr(filename.length()-3, 3) != "vtp")
				 std::cerr << "Warning! The output file does not have 'vtp' extension." << std::endl;
			  
			  // Naming the output file for colored vertices
			  std::string vertex_string("_vertices.vtp");
			  std::string filename_vertices(filename.begin(), filename.end()-4);

			  filename_vertices.append(vertex_string);
			
  			  writer_vertices = PolyDataWriter_ptr::New();			  
			  writer_vertices -> SetFileName(filename_vertices.c_str());	  
		}
		
		// Constructor with char*
		writer_vtp(const char* _filename) : filename(_filename) {
			  // Write the file
  			  writer = PolyDataWriter_ptr::New();
  			  writer -> SetFileName(filename.c_str());

  			  lines = CellArray_ptr::New();
			  points = Points_ptr::New();
			  vertices = Points_ptr::New();
			  
			  // Check on the extension of _filename
			  if(filename.substr(filename.length()-3, 3) != "vtp")
				 std::cerr << "Warning! The output file does not have 'vtp' extension." << std::endl;
			  
			  // Naming the output file for colored vertices
			  std::string vertex_string("_vertices.vtp");
			  std::string filename_vertices(filename.begin(), filename.end()-4);

			  filename_vertices.append(vertex_string);
			
  			  writer_vertices = PolyDataWriter_ptr::New();			  
			  writer_vertices -> SetFileName(filename_vertices.c_str());	  
		}
		
		/*! 
			@brief	It exports the graph in .vtp format (compatible with Paraview)
			@detail	If the edge isn't linear we assume there's a mesh defined on it.
					If there isn't, a simple straight line will be displayed for 
					that edge, independently of its geometry
		*/
		virtual void export_vtp(Graph const& G){
			std::cout << "Writing vtp file ..." << std::endl;
			BGLgeom::Edge_iter<Graph> e_it, e_end;
			unsigned int n_vertices = 0;
			for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it)
				add_line(*e_it, G, n_vertices);							
			generate_output(n_vertices);
		}	
		
	protected:
		//! The file name
		std::string filename;
		//! The streams associated to the output files
		PolyDataWriter_ptr writer;
		PolyDataWriter_ptr writer_vertices;
		//! Containers with all the points
		Points_ptr points;
		Points_ptr vertices;
		//! The array containing all the lines corresponding to the edges
		CellArray_ptr lines;		
		
		//! Helper function to ...
		void add_line(BGLgeom::Edge_desc<Graph> const& e, Graph const& G, unsigned int & count_vertices){

			BGLgeom::Vertex_desc<Graph> src = boost::source(e,G);
			BGLgeom::Vertex_desc<Graph> tgt = boost::target(e,G);
			
			//store point-coordinates in an array
			const double *SRC = G[src].coordinates.data();
			const double *TGT = G[tgt].coordinates.data();
			
			// all the points of all the edges are stored in points, so I need to keep track of the ID of my current point
			const int ID_start = points->GetNumberOfPoints(); 
			
			// insert src and tgt in "vertices"
			insert_point<dim>(SRC,vertices);
			insert_point<dim>(TGT,vertices);
			count_vertices += 2;				
			
			if(G[e].mesh.real.empty()){	// If the mesh is empty, create only source and target
				insert_point<dim>(SRC,points);
				insert_point<dim>(TGT,points);		
			} else {	// if the Mesh is defined, create the points of the mesh, included source and target
				double const *P; //it will contain the point coordinates;
				for(const BGLgeom::point<dim>& point: G[e].mesh.real){
					P = point.data();
					insert_point<dim>(P,points);			
 				}
			}	//else
			
			const unsigned int num_points = points->GetNumberOfPoints() - ID_start; // we count how many points we have in this line
			
			// create a new PolyLine
			vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
  			polyLine ->GetPointIds()->SetNumberOfIds(num_points); // set the PolyLine to contain num_points elements
  			
  			for(unsigned int i = 0; i < num_points; i++)
    			polyLine->GetPointIds()->SetId(i,i+ID_start);
 
   			lines->InsertNextCell(polyLine); // insert the new created line to the container of all lines
		}; // add_line
		
		//! Helper function to ...
		void generate_output(const unsigned int & n_vertices){
			/* Now all the lines have been stored in lines: we create a PolyData object
			containing points and line containers, and it will be the writer input argument */
			PolyData_ptr polyData = PolyData_ptr::New();
			PolyData_ptr polyData_vertices = PolyData_ptr::New();
 
			// Add the points to the dataset
  			polyData->SetPoints(points);
 
  			// Add the lines to the dataset
  			polyData->SetLines(lines);
  			
  			// Add the vertices to polyData_vertices
  			polyData_vertices -> SetPoints(vertices);
  			
  			// Color vertices
 	   		vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
			#if VTK_MAJOR_VERSION <= 5
	  		vertexFilter->SetInputConnection(polyData_vertices ->GetProducerPort());
			#else
	  		vertexFilter->SetInputData(polyData_vertices);
			#endif
	  		vertexFilter->Update();
	 
	  		PolyData_ptr polyData_color = vtkSmartPointer<vtkPolyData>::New();	  	
	  		polyData_color->ShallowCopy(vertexFilter->GetOutput());
	 
	  		// Setup colors
	  		unsigned char red[3] = {255, 0, 0};
	  		//unsigned char green[3] = {0, 255, 0};
	  		//unsigned char blue[3] = {0, 0, 255};
	 
	  		vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
	  		colors->SetNumberOfComponents(n_vertices);
	  		colors->SetName ("Colors");
	  		
	  		for (std::size_t i=0; i<n_vertices; ++i)
	  			colors->InsertNextTupleValue(red);
	 
	  		polyData_color->GetPointData()->SetScalars(colors);	
  
  			// Set the writers and write the output files
			#if VTK_MAJOR_VERSION <= 5
  			writer->SetInput(polyData);
  			writer_vertices -> SetInput(polyData_color);
			#else
  			writer->SetInputData(polyData);	
    		writer_vertices -> SetInputData(polyData_color)
  			#endif
  			
  			writer -> Write();
  			writer_vertices -> Write();
		}; // generate_output
		
};	//writer_vtp

}	//BGLgeom

#endif	//HH_WRITER_PTS_HH
