#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"
#include "aabb.h"

class sphere: public hittable
{
public:
	sphere() {}
	sphere(vec3 cen, float r, material *m) 
	{
		center = cen;
		radius = r;
		mat_ptr = m;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center;
	float radius;
	material *mat_ptr;
};

// Find the positive solution of the interection of the ray and the sphere
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	// Note that a few 2's have been ommitted since they eliminate each other
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(b*b-a*c))/a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(b*b-a*c))/a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

// Create an AABB for the sphere
// Since this sphere doesn't move, t0 and t1 aren't required
// They are just part of the function definition
bool sphere::bounding_box(float t0, float t1, aabb& box) const {
	box = aabb(center - vec3(radius, radius, radius),
			   center + vec3(radius, radius, radius));
	return true;
}

#endif