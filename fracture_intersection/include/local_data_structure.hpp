#ifndef HH_LOCAL_DATA_STRUCTURE_HH
#define HH_LOCAL_DATA_STRUCTURE_HH

#include <boost/graph/adjacency_list.hpp>

#include"data_structure.hpp"
#include "fracture_graph_properties.hpp"

namespace BGLgeom{

using point2 = BGLgeom::point<2>;
using line = BGLgeom::linear_geometry<2>;

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Fracture::Vertex_prop , Fracture::Edge_prop>;
using Edge_it = BGLgeom::Edge_iter<Graph>;
using Vertex_it = BGLgeom::Vertex_iter<Graph>;
using Vertex_d = BGLgeom::Vertex_desc<Graph>;
using Edge_d = BGLgeom::Edge_desc<Graph>;

}

#endif
