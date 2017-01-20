/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	intersections2D.cpp
	@author Luca Formaggia, Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Implementation of classes and functions to compute 
			intersections between two linear edges. 
*/

#include "intersections2D.hpp"
#include <iostream>
#include <iomanip>

using namespace BGLgeom;

namespace BGLgeom{

std::pair<bool, std::array<double,2>> solve 	(std::array<std::array<double,2>,2> const& A,
        										std::array<double,2> const& b,
        										double const& tol){
        										
	auto D = A[0][0]*A[1][1] - A[1][0]*A[0][1];	//determinant
	if (std::abs(D) <= tol)
	    return std::make_pair(false, std::array<double,2>{0,0});
	D=1./D;
	std::array<double,2> res;
	res[0] = D * (A[1][1]*b[0] - A[0][1]*b[1]);
	res[1] = D * (A[0][0]*b[1] - A[1][0]*b[0]);
	return std::make_pair(true, res);
}	//solve


Intersection compute_intersection	(linear_geometry<2> const& edge1,
									linear_geometry<2> const& edge2){
	Intersection out;
	linear_edge_interface S1(edge1);
	linear_edge_interface S2(edge2);
	std::array<std::array<double,2>,2> intersectionPoints;
	auto v1= S1[1]-S1[0];
	auto v2= S2[1]-S2[0];
	auto len1 = norm(v1);
	auto len2 = norm(v2);
	// Tolerance for distances
	double tol_dist=TOL*std::max(len1,len2);
	// I need to scale the tolerance to check parallelism correctly
	auto tol_sys=2.0*TOL*len1*len1*len2*len2;
	// First check if segments meets at the end
	for (unsigned int i=0;i<2;++i){
	    auto const & P1=S1[i];
	    for (unsigned int j=0;j<2;++j){
	        auto const & P2=S2[j];
	        auto dist=norm(P1-P2);
	        if (dist<=tol_dist){
	            if(out.numberOfIntersections>=2){
	                std::cerr << "Something wrong, cannot have more that 2 intersections" << std::endl;
	                out.good=false;
	                out.how = intersection_type::Something_went_wrong;
	                return out;
	            } //if
	            out.intersect=true;
	            intersectionPoints[out.numberOfIntersections++]=P1;
	            //out.intersectionPoint[out.numberOfIntersections++]=P1;
	            out.endPointIsIntersection[0][i]=true;
	            out.endPointIsIntersection[1][j]=true;
	            out.otherEdgePoint[0][i]=j;
	            out.otherEdgePoint[1][j]=i;
	        }//if
	    }//for
	}//for
	
	// Handle the case where the two edges are identical!
	if (out.numberOfIntersections==2u){
	    out.identical=true;
	    out.parallel=true;
	    out.collinear=true;
	    out.how = intersection_type::Identical;
	    out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
	    #ifndef NDEBUG
	    	std::cout<<"identical"<<std::endl;
	    #endif
	    return out;
	}
	// Now solve the linear system that returns the parametric coordinates of the intersection
	std::array<std::array<double,2>,2> A;
	// to make life easier I call A and B the ends of the two segmensts
	auto const & A1=S1[0];
	auto const & B1=S1[1];
	auto const & A2=S2[0];
	auto const & B2=S2[1];
	auto  V1 = B1-A1;
	auto  V2 = B2-A2;
	A[0][0]= dot(V1,V1);
	A[0][1]=-dot(V1,V2);
	A[1][0]= A[0][1];
	A[1][1]= dot(V2,V2);
	std::array<double,2> b;
	b[0]=    dot(A2-A1,V1);
	b[1]=    dot(A1-A2,V2);
	// find parametric coordinate of the intersection
	auto result=solve(A,b,tol_sys);
	// Standard case, the two lines are not parallel
	if(result.first==true){
	#ifndef NDEBUG
	    auto const & t = result.second;
	    // Make a stupid check (only in debugging phase)
	    auto P1 = A1+ t[0]*(B1-A1);
	    auto P2 = A2+ t[1]*(B2-A2);
	    if(norm(P1-P2)>tol_dist)
	        std::cerr<<" Something strange, intersection points not coincident. Distance= "<<norm(P1-P2)<<std::endl;
	#endif        
	    // The two lines intersect.
	    // Check whether we are inside the segments
	    double const & t1=result.second[0];
	    double const & t2=result.second[1];
	    //double tol1=tol/len1;
	    //double tol2=tol/len2;
	    bool inside = (t1>=-0.5*TOL) && (t1<= 1.0+0.5*TOL) && (t2>=-0.5*TOL) && (t2<= 1.0+0.5*TOL);
	    if (!inside){
	        // No intersecion, end here
	        out.how = intersection_type::No_intersection;
	        return out;
	    } else {
	        out.intersect=true;
	        // I could have used the mean
	        if (std::abs(t1    )<= TOL){
	            if(out.endPointIsIntersection[0][0]){
	                // already found. End here
	                out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
					compute_intersection_type(out);
	                return out;
	            } else {
	                out.endPointIsIntersection[0][0]=true;
	            }
	        } //if
	        if (std::abs(t1-1.0)<= TOL){
	            if(out.endPointIsIntersection[0][1]){
	                // already found. End here
	                out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
					compute_intersection_type(out);	                
	                return out;
	            } else {
	                out.endPointIsIntersection[0][1]=true;
	            }
	        } //if
	        if (std::abs(t2    )<= TOL){
	            if(out.endPointIsIntersection[1][0]){
	                // already found. End here
	                out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
					compute_intersection_type(out);	                
	                return out;
	            } else {
	                out.endPointIsIntersection[1][0]=true;
	            }
	        } //if
	        if (std::abs(t2-1.0)<= TOL){
	            if(out.endPointIsIntersection[1][1]){
	                // already found. End here
	                out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
	                compute_intersection_type(out);
	                return out;
	            } else {
	                out.endPointIsIntersection[1][1]=true;
	            }
	        } //if
	        intersectionPoints[out.numberOfIntersections++]=A1+ t1*(B1-A1);
	        //out.intersectionPoint[out.numberOfIntersections++]=A1+ t1*(B1-A1);
	        out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
			compute_intersection_type(out);	        
	        return out;
	    } //else
	}//if result.first==true
	else {
	    out.parallel=true;
	    // Compute distance between the two lines
	    /*
	    (x-s1(0,0))(s1(1,1)-s1(0,1))-(s1(1,0)*s1(0,0))*(y-s1(0,1))=0
	    (s1(1,1)-s1(0,1))x+(s1(0,0)*s1(1,0))y+s1(0,1)(s1(1,0)*s1(0,0))-
	    (s1(1,1)-s1(0,1))s1(0,0)=0
	    */
	    double factor=dot(B1-A1,A2-A1)/(len1*len1);
	    auto distVect = (A1-A2) + factor*(B1-A1);
	    double distance = norm(distVect);
	    out.distance=distance;
	    // Maybe here you want to use a different tolerance?
	    if (distance>tol_dist){
	        // The two segments are separated
	        // No intersection.
	        out.how = intersection_type::No_intersection;
	        return out;
	    } else {
	        out.collinear=true;
	        double t;
	        // Segments are collinear! We need to consider all the cases!
	        // IS A2 on S1? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[1][0]){
	            t=dot((A2-A1),(B1-A1))/(len1*len1);
	            if(t>=-0.5*TOL && t<=1+0.5*TOL){
	                out.intersect=true;
	                intersectionPoints[out.numberOfIntersections++]=A2;
	                //out.intersectionPoint[out.numberOfIntersections++]=A2;
	                out.endPointIsIntersection[1][0]=true;
	                if(out.numberOfIntersections==2){
	                	out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
	                	compute_intersection_type(out);					
	                	return out;
	                }
	            }//if
	        }//if
	        // IS B2 on S1? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[1][1]){
	            t=dot((B2-A1),(B1-A1))/(len1*len1);
	            if(t>=-0.5*TOL && t<=1+0.5*TOL){
	                out.intersect=true;
	                intersectionPoints[out.numberOfIntersections++]=B2;
	                //out.intersectionPoint[out.numberOfIntersections++]=B2;
	                out.endPointIsIntersection[1][1]=true;
	                if(out.numberOfIntersections==2){
	                	out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
	                	compute_intersection_type(out);
	                	return out;
	                }
	            }
	        }//if
	        // IS A1 on S2? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[0][0]){
	            t=dot((A1-A2),(B2-A2))/(len2*len2);
	            if(t>=-0.5*TOL && t<=1+0.5*TOL){
	                out.intersect=true;
	                intersectionPoints[out.numberOfIntersections++]=A1;
	                //out.intersectionPoint[out.numberOfIntersections++]=A1;
	                out.endPointIsIntersection[0][0]=true;
	                if(out.numberOfIntersections==2){
	                	out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
	                	compute_intersection_type(out);
	                	return out;
	                }
	            }
	        }//if
	        // IS B1 on S2? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[0][1]){
	            t=dot((B1-A2),(B2-A2))/(len2*len2);
	            if(t>=-0.5*TOL && t<=1+0.5*TOL){
	                out.intersect=true;
	                intersectionPoints[out.numberOfIntersections++]=B1;
	                //out.intersectionPoint[out.numberOfIntersections++]=B1;
	                out.endPointIsIntersection[0][1]=true;
	                if(out.numberOfIntersections==2){
	                	out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
	                	compute_intersection_type(out);
	                	return out;
	                }
	            }
	        }
	    } //else
	} //else

	//if I get here there are 2 cases: no intersection or collinear_common_extreme
	if(out.intersect == false){
		out.how = intersection_type::No_intersection;
		return out;
	}
	else{
		out.intersectionPoint = translate_array_to_eigen(intersectionPoints, out.numberOfIntersections);
		compute_intersection_type(out);
		return out;
	}

} //compute_intersection

	
void compute_intersection_type(Intersection & out){
	// Identical and No_intersection cases already handled in compute_intersection
	// Computing the number of intersection points
	unsigned int numEndPointIntersections = 0;
	for(std::size_t i=0; i<2; ++i){
		for(std::size_t j=0; j<2; ++j){
			if(out.endPointIsIntersection[i][j])
				numEndPointIntersections++;
		}
	}
	// numIntersection == 0 already treated in the compute_intersection function
	if(out.numberOfIntersections == 1){ 
		// X intersection
		if(numEndPointIntersections == 0){
			out.how	= intersection_type::X;
			return;
		}
		//T_new, T_old
		if(numEndPointIntersections == 1){
			// edge 1 intersects with one of its two extremes (0 or 1)
			if(out.endPointIsIntersection[1][0] || out.endPointIsIntersection[1][1]){
				out.how = intersection_type::T_new;
				return;
			}
			// edge 0 intersects with one of its two extremes (0 or 1)
			if(out.endPointIsIntersection[0][0] || out.endPointIsIntersection[0][1]){
				out.how = intersection_type::T_old;
				return;
			}
		}
		//Common_extreme; There are two end points (one for each edge) that meets
		if(numEndPointIntersections == 2){
			out.how = intersection_type::Common_extreme;
			return;
		}
		
	} else {		// 2 intersections, i.e. collinear = true
		//Overlap_outside; edge 0 (old) intersects in both its extremes (0 and 1) edge 1 (new)
		if(numEndPointIntersections == 2 && out.endPointIsIntersection[0][0] && out.endPointIsIntersection[0][1]){
			out.how = intersection_type::Overlap_outside;
			return;
		}
		//Overlap_inside; edge 1 (new) intersects in both its extremes (0 and 1) edge 0 (old)
		if(numEndPointIntersections == 2 && out.endPointIsIntersection[1][0] && out.endPointIsIntersection[1][1]){
			out.how = intersection_type::Overlap_inside;
			return;
		}
		//Overlap;
		if(	numEndPointIntersections == 2 && (
			(out.endPointIsIntersection[0][0] && out.endPointIsIntersection[1][0]) ||
			(out.endPointIsIntersection[0][0] && out.endPointIsIntersection[1][1]) ||
			(out.endPointIsIntersection[0][1] && out.endPointIsIntersection[1][0]) ||
			(out.endPointIsIntersection[0][1] && out.endPointIsIntersection[1][1])	 )
			){
				out.how = intersection_type::Overlap;
				return;
		}
		//Overlap_extreme_... ; if one extreme intersects in another, we have 3 endPointIntersections
		if(numEndPointIntersections == 3){
			//Overlap_extreme_inside; both extremes of edge 1 must be intersections
			if(out.endPointIsIntersection[1][0] && out.endPointIsIntersection[1][1]){ 
					out.how = intersection_type::Overlap_extreme_inside;
					return;	
			}			
			//Overlap_extreme_outside; both extremes of edge 0 must be intersections
			if(out.endPointIsIntersection[0][0] && out.endPointIsIntersection[0][1]){ 
					out.how = intersection_type::Overlap_extreme_outside;
					return;	
			}			
		}	//if	
	}	//else
	return;
}	//compute_intersection_type


std::ostream & operator<<(std::ostream & out, Intersection const& I){
	out<<"*Segment intersections:"<<std::endl;
	out<<"\tSegment intersects     :"<<std::boolalpha<<I.intersect<<std::endl;
	if (!I.intersect) return out;
	out<<"\tNumber of intersections:"<<I.numberOfIntersections<<std::endl;
	for (auto j=0u;j<I.numberOfIntersections;++j){
	    out<<"\t x["<<j<<"]="<<I.intersectionPoint[j](0,0);
	    out<<"\t y["<<j<<"]="<<I.intersectionPoint[j](0,1);
	    out<<std::endl;
	}
	out<<"\tSegments are identical:"<<std::boolalpha<<I.identical<<std::endl;
	out<<"\tSegments are parallel :"<<std::boolalpha<<I.parallel<<std::endl;
	out<<"\tSegments are collinear:"<<std::boolalpha<<I.collinear<<std::endl;
	for (unsigned int j=0u;j<2;++j){
	    for (unsigned int k=0u;k<2;++k){
	        if(I.endPointIsIntersection[j][k]){
	            out<<"\tEndPoint "<<k<<" of segment "<<j<<" is intersection"<<std::endl;
	            if(I.otherEdgePoint[j][k]!=-1)
	            out<<"\t\tand it is joined to EdgePoint "<<I.otherEdgePoint[j][k]
	                <<" of segment "<<(j+1)%2<<std::endl;
	        }
	    }	    
	}
	// Output of the type of intersection
	out << "\tType: ";
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
	else if(I.how == BGLgeom::intersection_type::Something_went_wrong)
		out << "Something went wrong";	
	out << std::endl;
	return out;
}	//operator<<

std::array<BGLgeom::point<2>,2>
translate_array_to_eigen(std::array<std::array<double,2>,2> const& array, unsigned int const& numberOfIntersection){
	std::array<BGLgeom::point<2>,2> P;
	for(std::size_t i=0; i<numberOfIntersection; ++i)
		for(std::size_t j=0; j<2; ++j)
			P[i](0,j) = array[i][j];
	return P;
}	//translate_array_to_eigen

}	//BGLgeom
