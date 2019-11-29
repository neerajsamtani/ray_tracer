#include <iostream>
#include "vec3.h"
#include "ray.h"

// Find where the ray hit the sphere
float hit_sphere(const vec3& center, float radius, const ray& r)
{
	// Form a quadratic equation to check if the ray satisfies
	// x*x + y*y + z*z = R*R
	// which in vector mathematics is
	// dot((p(t)-c),(p(t)-c))=R*R
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - 4*a*c;
	// Return the intersection of the ray and the sphere
	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-b - sqrt(discriminant)) / (2.0*a);
	}
}

// Determine the color of the pixel
vec3 color(const ray& r)
{
	// If the ray hits the sphere, find the normal to the sphere
	// and assign a color according to its direction
	float t = hit_sphere(vec3(0,0,-1), 0.5, r);
	if (t > 0.0)
	{
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0,0,-1));
		return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
	}
	// Calculate a blue-white linear interpolation (gradient)
	// Color the background based on this lerp
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5*(unit_direction.y() + 1.0);
	return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
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
	// Color x axis from left to right using index i
	// Color y axis from top to bottom using index j
	for (int j = ny-1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = color(r);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}