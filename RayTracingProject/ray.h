#ifndef RAY_H
#define RAY_H

#include"vector.h"
class ray {
	vec3 A, B;
	float timeStamp;

public:

	ray() {};
	ray(const vec3& v1, const vec3& v2, float ti =0.0) {
		A = v1;
		B = v2;
		timeStamp = ti;
	}
	vec3 origin() const {
		return A;
	}
	vec3 direction() const {
		return B;
	}

	float time() const {
		return timeStamp;
	}
	vec3 point_at_parameter(float t) const {
		vec3 point;
		point = A + B * t;
		return point;
	}
};

#endif // !RAY_H