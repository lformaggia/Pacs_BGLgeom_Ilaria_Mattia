/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file intersections2D.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Classes and functions to compute intersections between two linear edges. Implementation
	
	@detail
*/

#include "intersections2D.hpp"
#include <iostream>
#include <iomanip>

using namespace BGLgeom;
namespace BGLgeom{
Intersection compute_intersection	(linear_edge_geometry_srctgt<2> const& edge1,
									linear_edge_geometry_srctgt<2> const& edge2,
	                           		double tol){
	Intersection out;
	linear_edge_interface S1(edge1);
	linear_edge_interface S2(edge2);
	auto v1= S1[1]-S1[0];
	auto v2= S2[1]-S2[0];
	auto len1 = norm(v1);
	auto len2 = norm(v2);
	// Tolerance for distances
	double tol_dist=tol*std::max(len1,len2);
	// I need to scale the tolerance to check parallelism correctly
	auto tol_sys=2.0*tol*len1*len1*len2*len2;
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
	            }
	            out.intersect=true;
	            out.intersectionPoint[out.numberOfIntersections++]=P1;
	            out.endPointIsIntersection[0][i]=true;
	            out.endPointIsIntersection[1][j]=true;
	            out.otherEdgePoint[0][i]=j;
	            out.otherEdgePoint[1][j]=i;
	        }
	    }
	}
	// Handle the case where the two edges are identical!
	if (out.numberOfIntersections==2u){
	    out.identical=true;
	    out.parallel=true;
	    out.collinear=true;
	    out.how = intersection_type::Identical;
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
	    auto const & t = result.second;
	    // Make a stupid check (only in debugging phase)
	#ifndef NDEBUG
	    auto P1 = A1+ t[0]*(B1-A1);
	    auto P2 = A2+ t[1]*(B2-A2);
	    if(norm(P1-P2)>tol_dist)
	        std::cerr<<" Something strange, intersection points not coincident. Distance= "<<norm(P1-P2);
	#endif        
	    // The two lines intersect.
	    // Check whether we are inside the segments
	    double const & t1=result.second[0];
	    double const & t2=result.second[1];
	    //double tol1=tol/len1;
	    //double tol2=tol/len2;
	    bool inside = (t1>=-0.5*tol) && (t1<= 1.0+0.5*tol) && (t2>=-0.5*tol) && (t2<= 1.0+0.5*tol);
	    if (!inside){
	        // No intersecion, end here
	        out.how = intersection_type::No_intersection;
	        return out;
	    } else {
	        out.intersect=true;
	        // I coud have used the mean
	        if (std::abs(t1    )<= tol){
	            if(out.endPointIsIntersection[0][0]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[0][0]=true;
	            }
	        }
	        if (std::abs(t1-1.0)<= tol){
	            if(out.endPointIsIntersection[0][1]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[0][1]=true;
	            }
	        }
	        if (std::abs(t2    )<= tol){
	            if(out.endPointIsIntersection[1][0]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[1][0]=true;
	            }
	        }
	        if (std::abs(t2-1.0)<= tol){
	            if(out.endPointIsIntersection[1][1]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[1][1]=true;
	            }
	        }
	        out.intersectionPoint[out.numberOfIntersections++]=A1+ t1*(B1-A1);
	        return out;
	    }
	} else {
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
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=A2;
	                out.endPointIsIntersection[1][0]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	        // IS B2 on S1? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[1][1]){
	            t=dot((B2-A1),(B1-A1))/(len1*len1);
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=B2;
	                out.endPointIsIntersection[1][1]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	        // IS A1 on S2? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[0][0]){
	            t=dot((A1-A2),(B2-A2))/(len2*len2);
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=A1;
	                out.endPointIsIntersection[0][0]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	        // IS B1 on S2? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[0][1]){
	            t=dot((B1-A2),(B2-A2))/(len2*len2);
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=B1;
	                out.endPointIsIntersection[0][1]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	    }
	}
	// ========= COMPUTING THE INTERSECTION SITUATION ============
	// Identical and No_intersection cases already handled
	//Poi devo togliere tutti i return prima
	
	//calcolo il numero di intersezioni endPoint
	unsigned int numEndPointIntersections = 0;
	for(std::size_t i=0; i<2; ++i){
		for(std::size_t j=0; j<2; ++j){
			if(out.endPointIsIntersection[i][j])
				numEndPointIntersections++;
		}
	}
	//Comincio a distinguere i casi. numIntersection == 0 already treated in the previous code
	if(out.numberOfIntersections == 1){
		//with only one intersection, of course edge are not collinear
		// X intersection
		if(numEndPointIntersections == 0){
			out.how	= intersection_type::X;
			return out;
		}
		//T_new, T_old
		if(numEndPointIntersections == 1){
			//edge 1 interseca con uno dei suoi due estremi (0 o 1)
			if(out.endPointIsIntersection[1][0] || out.endPointIsIntersection[1][1]){
				out.how = intersection_type::T_new;
				return out;
			}
			//edge 0 interseca con uno dei suoi due estremi (0 o 1)
			if(out.endPointIsIntersection[0][0] || out.endPointIsIntersection[0][1]){
				out.how = intersection_type::T_old;
				return out;
			}
		}
		//Common_extreme; There are two end points (one for each edge) that meets
		if(numEndPointIntersections == 2){
			out.how = intersection_type::Common_extreme;
			return out;
		}
	} else {		//2 intersezioni, ovvero collinear = true
		//Overlap_outside; edge 0 (old) intersects in both its extremes (0 and 1) edge 1 (new)
		if(out.endPointIsIntersection[0][0] && out.endPointIsIntersection[0][1]){
			out.how = intersection_type::Overlap_outside;
			return out;
		}
		//Overlap_inside; edge 1 (new) intersects in both its extremes (0 and 1) edge 0 (old)
		if(out.endPointIsIntersection[1][0] && out.endPointIsIntersection[1][1]){
			out.how = intersection_type::Overlap_inside;
			return out;
		}
		//Overlap; Con solo due intersezioni segnalate sono sicuro di essere in questo caso
		if(	numEndPointIntersections == 2 && (
			(out.endPointIsIntersection[0][0] && out.endPointIsIntersection[1][0]) ||
			(out.endPointIsIntersection[0][0] && out.endPointIsIntersection[1][1]) ||
			(out.endPointIsIntersection[0][1] && out.endPointIsIntersection[1][0]) ||
			(out.endPointIsIntersection[0][1] && out.endPointIsIntersection[1][1])	 )
			){
				out.how = intersection_type::Overlap;
				return out;
		}
		//Overlap_extreme_...	; se si sovrappone un estremo, mi segnala tre intersezioni
		if(numEndPointIntersections == 3){
			//Overlap_extreme_inside; devono essere intersezioni tutti e due gli extremes dello stesso edge
			if( (out.endPointIsIntersection[0][0] && out.endPointIsIntersection[0][1]) ||
				(out.endPointIsIntersection[1][0] && out.endPointIsIntersection[1][1])	){
					out.how = intersection_type::Overlap_extreme_inside;
					return out;	
			}
			/*Overlap_extreme_outside; AHHHHHH, non si può distinguere da overlap_extreme_inside!
			Provare per credere: mettere nel test di SegmentIntersect gli edge:
			edge1 : 0,3 ; 0,0  ------ edge2: 0,1 ; 0,0
			edge1 : 0,3 ; 0,0 ------- edge2: 0,4 ; 0,0
			Mi dà lo stesso identico output, a parte le coordinate delle intersezioni!
			O fare un match appunto sulla coordinate delle intersezioni, o cercare fuori
			di gestire i due casi circa alla stessa maniera
			*/
			
		}
		
	}
	
	return out;
}	//compute_intersection



std::ostream & operator<<(std::ostream & out, Intersection const& I){
	out<<"*Segment intersections:"<<std::endl;
	out<<"\tSegment intersects     :"<<std::boolalpha<<I.intersect<<std::endl;
	if (!I.intersect) return out;
	out<<"\tNumber of intersections:"<<I.numberOfIntersections<<std::endl;
	for (auto j=0u;j<I.numberOfIntersections;++j){
	    out<<"\t x["<<j<<"]="<<I.intersectionPoint[j][0];
	    out<<"\t y["<<j<<"]="<<I.intersectionPoint[j][1];
	    out<<std::endl;
	}
	out<<"\t Segments are identical:"<<std::boolalpha<<I.identical<<std::endl;
	out<<"\t Segments are parallel :"<<std::boolalpha<<I.parallel<<std::endl;
	out<<"\t Segments are collinear:"<<std::boolalpha<<I.collinear<<std::endl;
	for (unsigned int j=0u;j<2;++j){
	    for (unsigned int k=0u;k<2;++k){
	        if(I.endPointIsIntersection[j][k]){
	            out<<"\t EndPoint "<<k<<" of segment "<<j<<" is intersection"<<std::endl;
	            if(I.otherEdgePoint[j][k]!=-1)
	            out<<"\t\t and it is joined to EdgePoint "<<I.otherEdgePoint[j][k]
	                <<" of segment "<<(j+1)%2<<std::endl;
	        }
	    }	    
	}
	return out;
}	//operator<<


}
