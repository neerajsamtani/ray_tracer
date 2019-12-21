#include <iostream>
#include "sphere.h"
#include "moving_sphere.h"
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

// Generate lots of random spheres
hittable *random_scene() {
	// 40 objects in the scene, plus a ground sphere and 3 large spheres
	// Start n at 50000 to ensure we get 40 randomly generated spheres at
	// a distance of atleast 0.9 away from vec3(4,0.2,0)
	int n = 50000;
	hittable **list = new hittable*[n+1];
	// Create ground sphere
	list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float choose_mat = random_double();
			vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());
			if ((center-vec3(4,0.2,0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new moving_sphere(center,
						center+vec3(0, 0.5*random_double(), 0),
						0.0, 1.0, 0.2,
						new lambertian(vec3(random_double()*random_double(),
											random_double()*random_double(),
											random_double()*random_double())
						)
					);
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
							new metal(vec3(0.5*(1 + random_double()),
										   0.5*(1 + random_double()),
										   0.5*(1 + random_double())),
									  0.5*random_double()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hittable_list(list,i);
}

// Print PPM to stdout
// Pipe to a .ppm file in the terminal to save and view the output
int main()
{
	// Image is 200x100 pixels
	int nx = 800;
	int ny = 400;
	// Number of samples
	int ns = 100;
	// Standard formatting of a .ppm file
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	// Generate a random scene
	hittable *world = random_scene();
	// Create a camera
	vec3 lookfrom(13,2,3);
	vec3 lookat(0,0,0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	camera cam(lookfrom, lookat, vec3(0,1,0), 20,
				float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
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
