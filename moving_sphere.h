#ifndef MOVINGSPHERE_H
#define MOVINGSPHERE_H

#include "rtweekend.h"

#include "hittable.h"

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
            vec3 cen0, vec3 cen1, double t0, double t1, double r, shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m)
        {};

        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

        vec3 center(double time) const;

    public:
        vec3 center0, center1;
        // Start and end time of motion
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};

// Find where the center of the sphere is at `time`
vec3 moving_sphere::center(double time) const{
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

// Find the positive solution of the interection of the ray and the sphere
// The position of the center of the sphere depends on the ray's time
bool moving_sphere::hit(
    const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        auto temp = (-half_b - root)/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

// CODE FOR BVH
// Create a static AABB which always bounds the sphere
// at any point in its motion
// bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const {
// 	aabb box0(center(t0) - vec3(radius, radius, radius),
// 			  center(t0) + vec3(radius, radius, radius));
// 	aabb box1(center(t1) - vec3(radius, radius, radius),
// 			  center(t1) + vec3(radius, radius, radius));
// 	box = surrounding_box(box0, box1);
// 	return true;
// }

#endif