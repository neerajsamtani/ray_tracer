#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include <limits>

// Determine the color of the pixel
vec3 color(const ray& r, hitable *world)
{
	hit_record rec;
	// If the ray hits the sphere, find the normal to the sphere
	// and assign a color according to its direction
	if (world->hit(r, 0.0, std::numeric_limits<float>::max(), rec))
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
	// Standard formatting of a .ppm file
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	// Start calculations from lower_left_corner
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	// Directional vectors
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	// Create two spheres and place them in the world
	hitable *list[2];
	list[0] = new sphere(vec3(0,0,-1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	// Color x axis from left to right using index i
	// Color y axis from top to bottom using index j
	for (int j = ny-1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 p = r.point_at_parameter(2.0); // Why?
			vec3 col = color(r, world);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}