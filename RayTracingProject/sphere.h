#pragma once
#include "hitable.h"

class sphere : public hitable {
	public:
		vec3 center;
		float radius;
		sphere(){}
		sphere(vec3 cent, float r) {
			center = cent;
			radius = r;
		}
		virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float determinant = b * b - a*c;
	if (determinant > 0) {
		float temp = (-b - sqrt(determinant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}

		temp = (-b + sqrt(determinant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}
