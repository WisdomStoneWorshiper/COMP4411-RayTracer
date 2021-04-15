#include <cmath>

#include "light.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    vec3f d = getDirection(P);
	ray objectToSource(P,d);
	vec3f att = vec3f(1.0,1.0,1.0);
	isect i;
	while (scene->intersect(objectToSource,i)) {
		std::cout << "direction" << std::endl;
		const Material &m = i.getMaterial();
		att = prod(att,m.kt);
		vec3f Q = objectToSource.at(i.t);
		objectToSource = ray(Q,d);
	}
    return att;
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	double dis=(P-position).length();
	double brightness=1.0/(getScene()->getAttConst()+getScene()->getAttLinear()*dis+getScene()->getAttQuad()*dis*dis);
	brightness=std::min(1.0,brightness);
	return brightness;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	vec3f d = (position - P).normalize();
	ray objectToSource(P,d);
	double T_light = (position - P).length();
	vec3f att = vec3f(1.0,1.0,1.0);
	isect i;
	while (scene->intersect(objectToSource,i)) {
		if (i.t < T_light) {
			std::cout << "point 1" << std::endl;
			const Material &m = i.getMaterial();
			att = prod(att,m.kt);
			vec3f Q = objectToSource.at(i.t);
			objectToSource = ray(Q,d);
			T_light -= i.t;
		} else {
			std::cout << "point 2" << std::endl;
			return att;
		}
	}
    return att;
}
