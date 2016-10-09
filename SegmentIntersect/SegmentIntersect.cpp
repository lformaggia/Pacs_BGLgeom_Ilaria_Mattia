#include "SegmentIntersect.hpp"
#include <iostream>
#include <iomanip>

std::ostream & operator << (std::ostream & out, Geometry::Intersection const & i)
{
  out<<"*Segment intersections:"<<std::endl;
  out<<"\tSegment intersects     :"<<std::boolalpha<<i.intersect<<std::endl;
  if (!i.intersect) return out;
  out<<"\tNumber of intersections:"<<i.numberOfIntersections<<std::endl;
  for (auto j=0u;j<i.numberOfIntersections;++j)
    {
      out<<"\t x["<<j<<"]="<<i.intersectionPoint[j][0];
      out<<"\t y["<<j<<"]="<<i.intersectionPoint[j][1];
      out<<std::endl;
    }
  out<<"\t Segments are identical:"<<std::boolalpha<<i.identical<<std::endl;
  out<<"\t Segments are parallel :"<<std::boolalpha<<i.parallel<<std::endl;
  out<<"\t Segments are collinear:"<<std::boolalpha<<i.collinear<<std::endl;
  for (unsigned int j=0u;j<2;++j)
    {
      for (unsigned int k=0u;k<2;++k)
        {
          if(i.endPointIsIntersection[j][k])
            {
              out<<"\t EndPoint "<<k<<" of segment "<<j<<" is intersection"<<std::endl;
              if(i.otherEdgePoint[j][k]!=-1)
                out<<"\t\t and it is joined to EdgePoint "<<i.otherEdgePoint[j][k]
                   <<" of segment "<<j+1 %2<<std::endl;
            }
        }
      
    }
  return out;
}
