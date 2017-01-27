#include<utility>
#include<vector>

#include"local_data_structure.hpp"

#include"helper_functions.hpp"

using namespace boost; 
using namespace BGLgeom;

int main(){

//initialize reader R passing input filename

//create a 2D graph and then pass it by reference to the method R.create_graph
Graph G;
 
point2 p1(0.0,0.0);
point2 p2(0.0,2.0);
point2 p3(2.0,0.0);
point2 p4(1.0,3.0);
point2 p5(4.0,4.0);
point2 p6(3.0,1.0);
point2 p7(3.0,3.0);
point2 p8(3.0,2.0);
point2 p9(2.0,1.0);
point2 p10(2.0,3.0);
point2 p11(2.0,-1.0);
point2 p12(2.0,4.0);
point2 p13(3.3,3.3);
point2 p14(3.6,3.6);
point2 p15(0.5,0.5);
point2 p16(-1.0,2.0);

line line1(p4,p6);
line line2(p2,p3);	
line line3(p1,p5);
line line4(p7,p8);
line line5(p9,p10);
line line6(p11,p12);
line line7(p13,p14);
line line8(p1,p15);
line line9(p1,p15);
line line10(p3,p16);

std::vector<line> line_vect{line1,line2,line3,line4,line5,line6,line7,line8,line9,line10};


create_graph(G,line_vect);
//initialize writer pts/vtk passing output filename

return 0;
}
