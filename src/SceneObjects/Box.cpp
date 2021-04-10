#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	BoundingBox bb=ComputeLocalBoundingBox();
	double tnear,tfar;
	if (bb.intersect(r,tnear,tfar)==false){
		return false;
	}else{
		if(tnear<RAY_EPSILON || tfar<RAY_EPSILON) return false;
		i.obj=this;
		i.t=tnear;
		i.N=vec3f(0,0,0);
		vec3f temp=r.at(i.t);
		if(fabs(temp[0]-bb.max[0])<RAY_EPSILON){
			i.N[0]=1;
		}else if(fabs(temp[0]-bb.min[0])<RAY_EPSILON){
			i.N[0]=-1;
		}else if(fabs(temp[1]-bb.max[1])<RAY_EPSILON){
			i.N[1]=1;
		}else if(fabs(temp[1]-bb.min[1])<RAY_EPSILON){
			i.N[1]=-1;
		}else if(fabs(temp[2]-bb.max[2])<RAY_EPSILON){
			i.N[2]=1;
		}else if(fabs(temp[2]-bb.min[2])<RAY_EPSILON){
			i.N[2]=-1;
		}
		return true;

	}


	

	return false;
}
