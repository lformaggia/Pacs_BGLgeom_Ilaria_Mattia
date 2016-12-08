#ifndef HH_INTERSECTION_LAYER_HH
#define HH_INTERSECTION_LAYER_HH

#include<vector>

#include"local_data_structure.hpp"
#include"intersections2D.hpp"
#include"linear_edge.hpp"



namespace BGLgeom{
	struct Int_layer{
		bool intersect;
		BGLgeom::intersection_type how;
		Edge_d int_edge;
		std::vector<point2> int_pts;
		
		
		Int_layer(const BGLgeom::Intersection & I, const Edge_d & e): intersect(I.intersect), how(I.how), int_edge(e){
			for(int i=0; i<I.numberOfIntersections; ++i){
				point2 P(I.intersectionPoint[i][0], I.intersectionPoint[i][1]);
				int_pts.push_back(P);
			}
		
		};
		
		friend std::ostream & operator << (std::ostream & out, Int_layer const& I) {
			out<<"Intersection point(s): "<<std::endl;
			for(const point2 & P: I.int_pts){
				out<<P<<std::endl;
			}

			out<<"Intersection type: ";
			if(I.how == BGLgeom::intersection_type::X)
				out << "X";
			else if(I.how == BGLgeom::intersection_type::T_new)
				out << "T_new";
			else if(I.how == BGLgeom::intersection_type::T_old)
				out << "T_old";
			else if(I.how == BGLgeom::intersection_type::Overlap_outside)
				out << "Overlap_outside";
			else if(I.how == BGLgeom::intersection_type::Overlap_inside)
				out << "Overlap_inside";
			else if(I.how == BGLgeom::intersection_type::Overlap)
				out << "Overlap";
			else if(I.how == BGLgeom::intersection_type::Overlap_extreme_inside)
				out << "Overlap_extreme_inside";
			else if(I.how == BGLgeom::intersection_type::Overlap_extreme_outside)
				out << "Overlap_extreme_outside";
			else if(I.how == BGLgeom::intersection_type::Identical)
				out << "Identical";
			else if(I.how == BGLgeom::intersection_type::Common_extreme)
				out << "Common_extreme";
			else 
				out << "Something went wrong";	
			out << std::endl;	
		}; //overload <<
	};
} //namespace

#endif
