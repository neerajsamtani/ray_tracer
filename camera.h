#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
		// vfov is top to bottom in degrees
		vec3 u, v, w;
		// Convert to radians
		float theta = vfov*M_PI/180;
		// Calculate half height and width
		float half_height = tan(theta/2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		// Find orthonormal basis for the camera plane
		// Camera points at -w
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		// Calculate the vectors bordering the image
		lower_left_corner = origin - half_width*u - half_height*v - w;
		horizontal = 2*half_width*u;
		vertical = 2*half_height*v;
	}

	ray get_ray(float s, float t) {
		return ray(origin,
				   lower_left_corner + s*horizontal + t*vertical - origin);
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};
#endif