#ifndef HH_LOCAL_DATA_STRUCTURE_HH
#define HH_LOCAL_DATA_STRUCTURE_HH

#include <boost/graph/adjacency_list.hpp>

#include "data_structure.hpp"
#include "linear_edge.hpp"

namespace BGLgeom{

using Graph = boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS,BGLgeom::Vertex_base_property<2>,BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2> >;
using point2 = BGLgeom::point<2>;
using line = BGLgeom::linear_edge<2>;

using Edge_it = BGLgeom::Edge_iter<Graph>;
using Vertex_it = BGLgeom::Vertex_iter<Graph>;
using Vertex_d = BGLgeom::Vertex_desc<Graph>;
using Edge_d = BGLgeom::Edge_desc<Graph>;
}

#endif
