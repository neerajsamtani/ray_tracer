#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

struct hit_record 
{
	float t;
	vec3 p;
	vec3 normal;
};

// Abstract class for all surfaces/volumes that can be hit
class hittable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif