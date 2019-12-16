#include <iostream>
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include <float.h>
#include "random.h"

// Determine the color of the pixel
vec3 color(const ray& r, hittable *world)
{
	hit_record rec;
	// If the ray hits the sphere, find the normal to the sphere
	// and assign a color according to its direction
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
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
	// Create two spheres and place them in the world
	hittable *list[2];
	list[0] = new sphere(vec3(0,0,-1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hittable *world = new hittable_list(list, 2);
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
                col += color(r, world);
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