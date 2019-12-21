#ifndef MATERIALH
#define MATERIALH

#include "hittable.h"
#include "random.h"
#include "ray.h"

// TODO- Motion blur for metals and glass
// Add `scattered = ray(rec.p, target-rec.p, r_in.time());` to these materials
// TODO - Fix bug: reflections don't show motion blur. This might be fixed by
// adding the above statement.

struct hit_record;

// Calculate the vector of the reflected ray
vec3 reflect(const vec3& v, const vec3& n) 
{
	// v is incident ray
	// n is surface normal
	return v - 2*dot(v,n)*n;
}

// Check if there is a refracted ray
// If so, store the refracted ray in vec3& refracted
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
	// Only refract if there is a positive solution
	// Otherwise light rays undergo total internal reflection
	if (discriminant > 0) {
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else
		return false;
}

// Reflectivity of glass varies with angle.
// Schlick's approximation finds the contribution of the Fresnel 
// factor in the specular reflection of light.
float schlick(float cosine, float ref_idx) {
	float r0 = (1-ref_idx) / (1+ref_idx);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1 - cosine),5);
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
			scattered = ray(rec.p, target-rec.p, r_in.time());
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

// Dielectrics include plastic and glass
class dielectric : public material {
	public:
		dielectric(float ri) : ref_idx(ri) {}
		// Light undergoes reflection and refraction when
		// interacting with dielectric materials
		virtual bool scatter(const ray& r_in, const hit_record& rec,
							 vec3& attenuation, ray& scattered) const {
			vec3 outward_normal;
			vec3 reflected = reflect(r_in.direction(), rec.normal);
			float ni_over_nt;
			attenuation = vec3(1.0, 1.0, 1.0);
			vec3 refracted;

			float reflect_prob;
			float cosine;

			// Set up vectors for refraction
			if (dot(r_in.direction(), rec.normal) > 0) {
				 outward_normal = -rec.normal;
				 ni_over_nt = ref_idx;
				 cosine = ref_idx * dot(r_in.direction(), rec.normal)
						/ r_in.direction().length();
			}
			else {
				 outward_normal = rec.normal;
				 ni_over_nt = 1.0 / ref_idx;
				 cosine = -dot(r_in.direction(), rec.normal)
						/ r_in.direction().length();
			}

			// Set up proabability of reflection based on schlick's approximation
			if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			   reflect_prob = schlick(cosine, ref_idx);
			}
			else {
			   reflect_prob = 1.0;
			}

			// Reflect or refract the ray r_in
			// and store result in scattered
			if (random_double() < reflect_prob) {
			   scattered = ray(rec.p, reflected);
			}
			else {
			   scattered = ray(rec.p, refracted);
			}

			return true;
		}

		float ref_idx;
};

#endif