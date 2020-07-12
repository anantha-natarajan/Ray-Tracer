#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ray.h"
#include "vector.h"
#include "hitable.h"

class triangle : public hitable {
public:
	vec3 vertexA, vertexB, vertexC;
	vec3 normalA, normalB, normalC;
	material *my_mat;

	triangle(vec3 A, vec3 B, vec3 C, vec3 nA, vec3 nB, vec3 nC, material *mat) {
		vertexA = A;
		vertexB = B;
		vertexC = C;
		normalA = nA;
		normalB = nB;
		normalC = nC;
		my_mat = mat;
	}

	triangle(vec3 *A, vec3 *B, vec3 *C, vec3 nA, vec3 nB, vec3 nC, material *mat) {
		vertexA = vec3(A->x(), A->y(), A->z());
		vertexB = vec3(B->x(), B->y(), B->z());
		vertexC = vec3(C->x(), C->y(), C->z());
		normalA = nA;
		normalB = nB;
		normalC = nC;
		my_mat = mat;
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

};

bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	vec3 ab = vertexB - vertexA;
	vec3 ac = vertexC - vertexA;
	vec3 normal = cross(ab, ac);
	float A = normal.length()/2; //area of triangle
	//normal.make_unit_vector();

	//parallel
	float d = dot(-normal, r.direction());
	if (d > 0.001) { 
		// the normal and ray are not perpendicular => triangle is not parallel to the ray
		vec3 p0l0 = r.origin() - vertexA;
		float t;
		//cout << "value of FLT_MAX " << FLT_MAX << endl;
		t = -dot(p0l0, -normal) / d;
		if (t < t_max && t > t_min) {
			//triangle plane is in front
			vec3 P = r.point_at_parameter(t);

			vec3 C;

			vec3 bc = vertexC - vertexB;
			vec3 bp = P - vertexB;
			C = cross(bc, bp);
			float u = (C.length()) / 2;
			u /= A;

			if (dot(normal, C) < 0) {
				return false;
			}

			vec3 ca = vertexA - vertexC;
			vec3 cp = P - vertexC;
			C = cross(ca, cp);
			float v = (C.length()) / 2;
			v /= A;

			if (dot(normal, C) < 0) {
				return false;
			}

			float w = 1 - u - v;

			if( w > 0 && w < 1){
				// point lies inside the triangle
				rec.t = t;
				rec.p = r.point_at_parameter(t);
				normal.make_unit_vector();
				rec.normal = normal;
				rec.mat = my_mat;
				rec.u = 0;
				rec.v = 0;
				return true;
			}
		}	
	}
	return false;

}

#endif