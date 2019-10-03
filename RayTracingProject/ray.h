#pragma once
#include"vector.h"
class ray {
	vec3 A, B;

	public:
		
		ray();
		ray(const vec3& v1, const vec3& v2) {
			A = v1;
			B = v2;
		}
		vec3 origin() const {
			return A;
		}
		vec3 direction() const {
			return B;
		}
		vec3 point_at_parameter(float t) const {
			vec3 point;
			point = A + B * t;
			return point;
		}
};