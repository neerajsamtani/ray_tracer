#ifndef MATERIALH
#define MATERIALH

#include "hittable.h"
#include "random.h"
#include "ray.h"

struct hit_record;

// Calculate the vector of the reflected ray
vec3 reflect(const vec3& v, const vec3& n) 
{
    return v - 2*dot(v,n)*n;
}

// Abstract class for materials
class material  
{
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation,
            ray& scattered) const = 0;
};

// Lambertian is a perfect matte material
// which scatters light in all directions equally
class lambertian : public material 
{
    public:
        lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec,
                             vec3& attenuation, ray& scattered) const {
        	// Calculate target point
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            // Calculate next ray
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

// Metal is a reflective material
class metal : public material 
{
    public:
        metal(const vec3& a, float f) : albedo(a) {
        	// Maximum fuzziness of 1
        	if (f < 1.0)
        		fuzz = f;
        	else
        		fuzz = 1.0;
        }
        virtual bool scatter(const ray& r_in, const hit_record& rec,
                             vec3& attenuation, ray& scattered) const {
        	// Calculate the target point of reflection
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            // Calculate reflected ray, with fuzziness if needed
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        vec3 albedo;
        float fuzz;
};

#endif