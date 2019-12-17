#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

class material;

struct hit_record 
{
	// Discriminant of intersection
	float t;
	// Point at parameter
	vec3 p;
	// Normal
	vec3 normal;
	// Material
	material *mat_ptr;
};

// Abstract class for all surfaces/volumes that can be hit
class hittable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif