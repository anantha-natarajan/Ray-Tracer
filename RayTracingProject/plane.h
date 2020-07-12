#ifndef PLANE_H
#define PLANE_H

#include "ray.h"
#include "vector.h"
#include "hitable.h"

class plane : public hitable {
public:
	vec3 normal;
	vec3 pointOnPlane;
	material *my_mat;
	plane() {}
	plane(vec3 n, vec3 point, material *mat) {
		normal = n;
		pointOnPlane = point;
		my_mat = mat;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};


void get_uv_plane(const vec3& ph, float& u, float& v, vec3 n, vec3 pp) {


	vec3 a = cross(n, vec3(1, 0, 0));
	vec3 b = cross(n, vec3(0, 1, 0));

	vec3 max_ab = dot(a, a) < dot(b, b) ? b : a;
	vec3 c = cross(n, vec3(0, 0, 1));
	
	vec3 maxVec = dot(max_ab, max_ab) < dot(c, c) ? c : max_ab;
	maxVec.make_unit_vector();

	vec3 U = maxVec;
	vec3 V = cross(n, U);

	u = dot(U, ph - pp);
	v = dot(V, ph - pp);





	return;
	/*vec3 n0(1, 0, 0);
	vec3 n1(0, 0, -1);
	float s0 = 5, s1 = 5;
	u = dot(n0, ph - pp)/s0;
	v = dot(n1, ph - pp) / s1;
	if (u < 0) {
		u = -u;
	}
	if (v < 0) {
		v = -v;
	}
	
	if (u > 1 || u < 0) {
		u = (int)u % 1;		
		cout << u << endl;
	}
	if (v > 1 || v < 0) {
		v = (int)v % 1;
		u = (int)u % 1;
		cout << v << endl;
	}
	//u = p.x() ;
	//v = p.z() ;*/
}

bool plane :: hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	
	float denominator = dot(r.direction(), normal);
	if (denominator > 0.0001) {
		vec3 p0l0 = r.origin() - pointOnPlane;
		float t;
		//cout << "value of FLT_MAX " << FLT_MAX << endl;
		t = -dot(p0l0, normal) / denominator;
		//cout << t << endl;
		if (t < t_max && t > t_min) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = -normal;
			rec.mat = my_mat;
			get_uv_plane(rec.p, rec.u, rec.v, -normal, pointOnPlane);
			//rec.u = 0;
			//rec.v = 0;
			return true;
		}
	}
	return false;

}


#endif // ! PLANE_H

