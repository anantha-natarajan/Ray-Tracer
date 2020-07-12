
#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "vector.h"
#include "hitable.h"

class sphere : public hitable {
public:
	vec3 center;
	float radius;
	material *my_mat;
	sphere() {}
	sphere(vec3 cent, float r, material *mat) {
		center = cent;
		radius = r;
		my_mat = mat;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};

void get_uv_sphere(const vec3& p, float& u, float& v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float determinant = b * b - a * c;
	if (determinant > 0) {
		float temp = (-b - sqrt(determinant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			rec.mat = my_mat;
			get_uv_sphere((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}

		temp = (-b + sqrt(determinant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			rec.mat = my_mat;
			get_uv_sphere((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
	}
	return false;
}


class movingSphere : public hitable {
public:
	vec3 center0, center1;
	float radius, time0, time1;
	material *my_mat;
	movingSphere() {}
	movingSphere(vec3 cent0, vec3 cent1, float t0, float t1, float r, material *mat) {
		center0 = cent0;
		center1 = cent1;
		time0 = t0;
		time1 = t1;
		radius = r;
		my_mat = mat;
	}
	vec3 center(float time) const;
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};

vec3 movingSphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}


bool movingSphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float determinant = b * b - a * c;
	if (determinant > 0) {
		float temp = (-b - sqrt(determinant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat = my_mat;
			get_uv_sphere((rec.p - center(r.time())) / radius, rec.u, rec.v);
			return true;
		}

		temp = (-b + sqrt(determinant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat = my_mat;
			get_uv_sphere((rec.p - center(r.time())) / radius, rec.u, rec.v);
			return true;
		}
	}
	return false;
}



#endif // !SPHERE_H
