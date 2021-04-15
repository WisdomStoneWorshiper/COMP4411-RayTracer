#include "ray.h"
#include "material.h"
#include "light.h"

int Material::n_materials = 1;

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	/*My reference:

	https://drive.google.com/file/d/1bld4cdxs3Avg3kA8qnYW-VQutQrwswL-/view?usp=sharing
	https://drive.google.com/file/d/1szSlWa1AGnnGkxT0flss_nHTI7E879UZ/view?usp=sharing around page 135
	Leture notes 09
	*/
	vec3f Isum=ke+(prod(ka,scene->getAmbient()));
	vec3f P=r.at(i.t);
	vec3f n=i.N;
	vec3f v=r.getDirection();
	for (std::list<Light*>::const_iterator L = scene->beginLights(); L != scene->endLights(); ++L){
		Light* curr_light=*L;
		vec3f l=curr_light->getDirection(P);
		vec3f I = curr_light->getColor(P);

		vec3f Ld=prod(kd,I)*std::max(0.0,n.dot(l));
		vec3f r=-2*(l.dot(n))*n+l;
		vec3f Ls=prod(ks,I)*pow(std::max(0.0,v.dot(r)),shininess*128);
		vec3f atten=curr_light->distanceAttenuation(P)*curr_light->shadowAttenuation(P);

		Isum=Isum+prod(atten,Ld+Ls);
	}

	return Isum.clamp();
}
