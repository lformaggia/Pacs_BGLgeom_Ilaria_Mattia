#include "bspline_geometry.hpp"
#include "graph_builder.hpp"
#include "data_structure.hpp"
#include "mesh.hpp"
#include "point.hpp"
#include <vector>
#include <iostream>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>


using namespace BGLgeom;
using namespace boost;

int main(){


	std::vector<point<2>> control_pts;
	control_pts.push_back(point<2>(0,0));
	control_pts.push_back(point<2>(0,1));
	control_pts.push_back(point<2>(1,1));
	control_pts.push_back(point<2>(1,0));
	
	std::cout << "================== BSPLINE GEOMETRY ======================" << std::endl << std::endl;
	bspline_geometry<2,2> B(control_pts);
	std::cout << B << std::endl << std::endl;
	
	std::cout << "Evaluation: " << std::endl;
	std::cout << "\tt=0   : " << B(0) << std::endl;
	std::cout << "\tt=0.5 : " << B(0.5) << std::endl;
	std::cout << "\tt=1   : " << B(1) << std::endl;
	
	std::cout << "First derivative: " << std::endl;
	std::cout << "\tt=0   : " << B.first_der(0) << std::endl;
	std::cout << "\tt=0.5 : " << B.first_der(0.5) << std::endl;
	std::cout << "\tt=1   : " << B.first_der(1) << std::endl;
	
	std::cout << "Second derivative: " << std::endl;
	std::cout << "\tt=0   : " << B.second_der(0) << std::endl;
	std::cout << "\tt=0.5 : " << B.second_der(0.5) << std::endl;
	std::cout << "\tt=1   : " << B.second_der(1) << std::endl;
//	std::cout << "\tt=10  : " << B.second_der(10) << std::endl;
	
	std::cout << "Curvilinear abscissa: " << std::endl;
	std::cout << "\tt=0   : " << B.curv_abs(0) << std::endl;
	std::cout << "\tt=0.5 : " << B.curv_abs(0.5) << std::endl;
	std::cout << "\tt=1   : " << B.curv_abs(1) << std::endl;
	
	std::cout << "Curvature: " << std::endl;
	std::cout << "\tt=0   : " << B.curvature(0) << std::endl;
	std::cout << "\tt=0.5 : " << B.curvature(0.5) << std::endl;
	std::cout << "\tt=1   : " << B.curvature(1) << std::endl;
	
	// Now evaluation with vectors
	std::cout << std::endl << "----------- Now evaluation with vector ------------" << std::endl << std::endl;
	std::vector<double> t{0,0.5,1};
	
	std::cout << "Evaluation:" << std::endl;
	std::vector<point<2>> Eval = B(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << Eval[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "First derivative:" << std::endl;
	std::vector<point<2>> F = B.first_der(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << F[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Second derivative:" << std::endl;
	std::vector<point<2>> S = B.second_der(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << S[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Curvilinear abscissa:" << std::endl;
	std::vector<double> A = B.curv_abs(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << A[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Curvature:" << std::endl;
	std::vector<double> C = B.curvature(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << C[i] << std::endl;
	std::cout << std::endl;
	
	// The example on De Falco demo
	std::cout << std::endl << "==================== ANOTHER BSPLINE =====================" << std::endl;
	std::cout << "Now a more difficult example" << std::endl << std::endl;
	
	std::vector<point<3>> CPs = {point<3>(0,      0,   0),	// 1st c.p.
							     point<3>(2./3.,  1,   0),	// 2nd c.p.
							     point<3>(2,      2, 8.0),	// 3rd c.p.
							     point<3>(10./3., 4,   0),	// 4th c.p.
							     point<3>(11./3., 4,   0),	// 5th c.p.
							     point<3>(4,      8,   0)};	// 6th c.p.
	// Bspline edge defaulted at dim=3, deg=3
	bspline_geometry<> B2;
	B2.set_bspline(CPs);
	mesh<3> M;
	M.uniform_mesh(20, B2);
	std::vector<point<3>> value = M.real;
	std::vector<point<3>> firstD = B2.first_der(M.parametric);
	std::vector<point<3>> secondD = B2.second_der(M.parametric);
	
	std::cout << std::setw(6) << "x" << std::setw(7) << "y" << std::setw(7) << "z"
			  << std::setw(14) << "dx/dt" << std::setw(7) << "dy/dt" << std::setw(7) << "dz/dt"
			  << std::setw(13) << "d2x/dt2" << std::setw(8) << "d2y/dt2" << std::setw(8) << "d2z/dt2" << std::endl;
	for(std::size_t i = 0; i < value.size(); ++i){
		std::cout << std::setprecision(4) << std::fixed << std::setw(8)
				  << value[i] << std::setw(12) << firstD[i] << std::setw(12) << secondD[i] << std::endl;
	}
	std::cout << std::endl;
	
	// Now we try to build a graph with one bspline edge
	std::cout << "==================== ON GRAPH ======================" << std::endl;
	std::cout <<  "Creating a graph" << std::endl << std::endl;
	using Graph = adjacency_list<vecS, vecS, directedS, Vertex_base_property<3>, Edge_base_property_static<bspline_geometry<>,3> >;
	Graph G;
	
	Vertex_desc<Graph> src, tgt;
	src = add_vertex(G);
	tgt = add_vertex(G);
	Edge_desc<Graph> e;
	e = new_bspline_edge<Graph,3>(src, tgt, G, CPs);
	
	std::cout << "\tt=0   : " << G[e].geometry(0) << std::endl;
	std::cout << "\tt=0.34: " << G[e].geometry.second_der(0.34) << std::endl;
	
	return 0;
}
