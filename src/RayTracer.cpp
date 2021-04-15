// The main ray tracer.

#include "RayTracer.h"

#include <Fl/fl_ask.h>

#include "fileio/parse.h"
#include "fileio/read.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace(Scene *scene, double x, double y) {
	ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
	scene->getCamera()->rayThrough(x, y, r);
	stack<Material> stack;
	return traceRay(scene, r, vec3f(1.0, 1.0, 1.0), 0, stack).clamp(); //is air at start
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay(Scene *scene, const ray &r, const vec3f &thresh, int depth, stack<Material> material_stack) {
	isect i;
	if (max_depth<depth) return vec3f(0.0, 0.0, 0.0);

	if (scene->intersect(r, i)) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.

		/*
		My reference:
		https://drive.google.com/file/d/17wMyM9TLaF0b76zvX_dhBjITrlUaNiLP/view?usp=sharing
		
		*/
		// std::cout  << "pos " << i.t << " depth " << depth << " isAir " << isAir << std::endl;
		const Material &m = i.getMaterial();
		vec3f phong=m.shade(scene, r, i);
		if (phong.length() < getScene()->getThreshold())		//adaptive termination
			return phong;
		phong = prod(phong, vec3f(1, 1, 1) - m.kt);
		//reflection
		vec3f P=r.at(i.t);
		vec3f R=-2*(r.getDirection().dot(i.N))*i.N+r.getDirection();
		R=R.normalize();
		ray reflection=ray(P,R);
		vec3f reflectedColor=traceRay(scene,reflection,thresh,depth+1,material_stack);
		reflectedColor=prod(m.kr,reflectedColor);
		//reflection end

		//refraction
		vec3f refractedColor = vec3f(0.0, 0.0, 0.0);
		if (m.kt.length() < 0.000000001)	//if kt is very small, then material cannot transmit rays -> no refraction
			return phong + reflectedColor;
		
		vec3f N;
		double ni,nt;
		if (material_stack.empty()) {
			ni = 1.0;
			N = i.N;
			nt = m.index;
			material_stack.push(m);
		} else {
			if (material_stack.top().identity == m.identity) {
				ni = m.index;
				material_stack.pop();
				nt = material_stack.empty() ? 1.0 : material_stack.top().index;
				N = -i.N;
			} else {
				ni = material_stack.top().index;
				nt = m.index;
				N = i.N;
				material_stack.push(m);
			}
		}

		double Nr = ni/nt;
		vec3f V = -r.getDirection();
		double cos_theta_t = 1-Nr*Nr*(  1 - ( N.dot(V) ) * ( N.dot(V) )  );
		if (cos_theta_t < 0)				// if there is total internal reflection, no refraction happens
			return phong+reflectedColor;
		vec3f PP = r.at(i.t); 
		vec3f T = ( Nr*(N.dot(V)) - sqrt(cos_theta_t) )*N - Nr*V;
		T = T.normalize();
		ray refracted_ray = ray(PP,T);
		refractedColor = traceRay(scene,refracted_ray,thresh,depth+1, material_stack);
		refractedColor = prod(m.kt,refractedColor);
		// refraction end
		
		
		return phong + reflectedColor + refractedColor;

	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.

		return vec3f(0.0, 0.0, 0.0);
	}
}

RayTracer::RayTracer() {
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
}

RayTracer::~RayTracer() {
	delete[] buffer;
	delete scene;
}

void RayTracer::getBuffer(unsigned char *&buf, int &w, int &h) {
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio() { return scene ? scene->getCamera()->getAspectRatio() : 1; }

bool RayTracer::sceneLoaded() { return m_bSceneLoaded; }

bool RayTracer::loadScene(char *fn) {
	try {
		scene = readScene(fn);
	} catch (ParseError pe) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		fl_alert("ParseError: %s\n", pe);
#endif
		return false;
	}

	if (!scene) return false;

	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[bufferSize];

	// separate objects into bounded and unbounded
	scene->initScene();

	// Add any specialized scene loading code here

	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup(int w, int h) {
	if (buffer_width != w || buffer_height != h) {
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete[] buffer;
		buffer = new unsigned char[bufferSize];
	}
	memset(buffer, 0, w * h * 3);
}

void RayTracer::traceLines(int start, int stop) {
	vec3f col;
	if (!scene) return;

	if (stop > buffer_height) stop = buffer_height;

	for (int j = start; j < stop; ++j)
		for (int i = 0; i < buffer_width; ++i) tracePixel(i, j);
}

void RayTracer::tracePixel(int i, int j) {
	vec3f col;

	if (!scene) return;

	double x = double(i) / double(buffer_width);
	double y = double(j) / double(buffer_height);

	col = trace(scene, x, y);

	unsigned char *pixel = buffer + (i + j * buffer_width) * 3;

	pixel[0] = (int)(255.0 * col[0]);
	pixel[1] = (int)(255.0 * col[1]);
	pixel[2] = (int)(255.0 * col[2]);
}

void RayTracer::setDepth(int d){max_depth=d;}

Scene *  RayTracer::getScene(){
	return scene;
}