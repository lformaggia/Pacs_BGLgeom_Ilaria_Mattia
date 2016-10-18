#include "SegmentIntersect.hpp"

int main()
{
  using namespace Geometry;
  std::array<double,2> p1={0.0,-1.0};
  std::array<double,2> p2={0.0,2.0};
  std::array<double,2> p3={0.,2.0};
  std::array<double,2> p4={0.0,0.0};
  
  EdgeGeo<> a(p1,p2);
  EdgeGeo<> b(p3,p4);
  auto intersection=segmentIntersect(a,b);
  std::cout<<intersection;
}