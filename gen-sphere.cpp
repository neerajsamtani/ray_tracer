#include <iostream>
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include <float.h>
#include "random.h"
#include "material.h"

// Determine the color of the pixel
vec3 color(const ray& r, hittable *world, int depth)
{
	hit_record rec;
	// If the ray hits an object, reflect the ray in a random direction
	// and recursively determine what happens to that ray.
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation*color(scattered, world, depth+1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	// Calculate a blue-white linear interpolation (gradient)
	// Color the background based on this lerp
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}

// Print PPM to stdout
// Pipe to a .ppm file in the terminal to save and view the output
int main()
{
	// Image is 200x100 pixels
	int nx = 200;
	int ny = 100;
	// Number of samples
	int ns = 100;
	// Standard formatting of a .ppm file
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	// Create four spheres and place them in the world
	hittable *list[5];
	// Two lambertains (perfect mattes) which were in our initial image
	list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	// One metal sphere and one dielectric inside another dielectric
	list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
	hittable *world = new hittable_list(list, 5);
	// Create a camera
	camera cam;
	// Color x axis from left to right using index i
	// Color y axis from top to bottom using index j
	for (int j = ny-1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			// Add samples in a single pixel
			// for antialiasing
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			// Average all the samples
			col /= float(ns);
			col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}
