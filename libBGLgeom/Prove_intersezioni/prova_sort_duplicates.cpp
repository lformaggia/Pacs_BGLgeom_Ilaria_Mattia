#include<algorithm> 
#include<functional>
#include"IntObj.hpp"

bool asc_order_pts(const point p1, const point p2){
	if(p1(0)<p2(0)) return true;
	else if(p1(0) > p2(0)) return false;
	else return (p1(1) < p2(1));
}

bool asc_order(IntObj & I1, IntObj &I2){
	//ordina I1 nel caso ci siano due elementi
	if(I1.int_pts.size()==2 && !asc_order_pts(I1.int_pts[0], I1.int_pts[1])){
		std::swap(I1.int_pts[0],I1.int_pts[1]);
	}
	//ordina I2 nel caso ci siano due elementi
	if(I2.int_pts.size()==2 && !asc_order_pts(I2.int_pts[0], I2.int_pts[1])){
		std::swap(I2.int_pts[0],I2.int_pts[1]);
	}	
	
	return(asc_order_pts(I1.int_pts[0],I2.int_pts[0]));
}

bool desc_order_pts(const point p1, const point p2){return !asc_order_pts(p1,p2);};

bool desc_order(IntObj & I1, IntObj &I2){
	//ordina I1 nel caso ci siano due elementi
	if(I1.int_pts.size()==2 && !desc_order_pts(I1.int_pts[0], I1.int_pts[1])){
		std::swap(I1.int_pts[0],I1.int_pts[1]);
	}
	//ordina I2 nel caso ci siano due elementi
	if(I2.int_pts.size()==2 && !desc_order_pts(I2.int_pts[0], I2.int_pts[1])){
		std::swap(I2.int_pts[0],I2.int_pts[1]);
	}	
	return desc_order_pts(I1.int_pts[0], I2.int_pts[0]);
}

bool is_duplicate(const IntObj & I1, const IntObj & I2){
	//check if the first of the 2 elements is of type Common_extreme. If so check if the second one shares with it one intersection point
	if(I1.int_type == Intersection_type::Common_extreme){
		for(const point & P2: I2.int_pts)
			if(I1.int_pts[0] == P2) return true;
	}
	// Do the same as before, inverting I1 and I2
	if(I2.int_type == Intersection_type::Common_extreme){
		for(const point & P1: I1.int_pts)
			if(I2.int_pts[0] == P1) return true;
	}
	return false;
}

int main(){
    point src(0.0,0.0);
    point p1(2.0,2.0);
    point p2(1.0,1.0);
    point p3(3.0,3.0);
    point tgt(4.0,4.0);
	IntObj intobj1(true, p3, p1, Intersection_type::Overlap_extreme_inside);
	IntObj intobj2(true, p3, Intersection_type::Common_extreme);
	IntObj intobj3(true, p2, Intersection_type::Common_extreme);
	IntObj intobj4(true, p2, Intersection_type::Common_extreme);
	IntObj intobj5(true, p1, Intersection_type::Common_extreme);
	
	std::vector<IntObj> intvect;
	intvect.push_back(intobj1);
	intvect.push_back(intobj2);
	intvect.push_back(intobj3);
	intvect.push_back(intobj4);
	intvect.push_back(intobj5);
	
	if(asc_order_pts(src,tgt)) 	
		std::sort(intvect.begin(), intvect.end(), asc_order);
	else 
		std::sort(intvect.begin(), intvect.end(), desc_order);

	std::cout<<"Ordered"<<std::endl;
	for(const IntObj & obj: intvect)
		std::cout<< obj<<std::endl;
	 //ok funziona correttamente
	
	auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
	intvect.erase(last, intvect.end());
	
	std::cout<<"Without duplicates"<<std::endl;
	for(const IntObj & obj: intvect)
		std::cout<< obj<<std::endl;

return 0;	
}



