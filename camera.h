#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include "random.h"

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0*vec3(random_double(),random_double(),0) - vec3(1,1,0);
	} while (dot(p,p) >= 1.0);
	return p;
}

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
	   float aperture, float focus_dist, float t0, float t1) {

	// Set shutter open and close time
	// Used for motion blur
	time0 = t0;
	time1 = t1;
	// Set lens radius
	// Used for defocus blur
	lens_radius = aperture / 2;
	// vfov is top to bottom in degrees. Convert it to radians
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
	lower_left_corner = origin - 
						half_width * u * focus_dist - 
						half_height * v * focus_dist - 
						w * focus_dist;
	horizontal = 2 * half_width * focus_dist * u;
	vertical = 2 * half_height * focus_dist * v;
}

	ray get_ray(float s, float t) {
			// rd and offset are used for defocus blur
			vec3 rd = lens_radius*random_in_unit_disk();
			vec3 offset = u * rd.x() + v * rd.y();
			// Send out the ray at a random time between time0 and time1
			float time = time0 + random_double()*(time1-time0);
			return ray(origin + offset,
						lower_left_corner + s*horizontal + t*vertical
						- origin - offset,
						time);
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float time0, time1;
	float lens_radius;
};

#endif