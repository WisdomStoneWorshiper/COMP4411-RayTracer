#include <cmath>

#include "light.h"
 #define M_PI 3.14159265358979323846 // pi

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
			const Material &m = i.getMaterial();
			att = prod(att,m.kt);
			vec3f Q = objectToSource.at(i.t);
			objectToSource = ray(Q,d);
			T_light -= i.t;
		} else {
			return att;
		}
	}
    return att;
}


double SpotLight::distanceAttenuation( const vec3f& P ) const
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

vec3f SpotLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f SpotLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f SpotLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	vec3f Lp=-1*getDirection(P);
	double I = Lp.dot(orientation.normalize());
	double range = cos(angle/180.0*M_PI);
	bool in_range=false; 
	if(I>range){
		in_range=true;
	}
	vec3f d = (position - P).normalize();
	ray objectToSource(P,d);
	double T_light = (position - P).length();
	vec3f att = vec3f(1.0,1.0,1.0);
	isect i;
	while (scene->intersect(objectToSource,i)) {
		if (i.t < T_light) {
			const Material &m = i.getMaterial();
			att = prod(att,m.kt);
			vec3f Q = objectToSource.at(i.t);
			objectToSource = ray(Q,d);
			T_light -= i.t;
		} else {
			return att;
		}
	}
	if (!in_range){
		return att*0;
	}
    return att;
}
