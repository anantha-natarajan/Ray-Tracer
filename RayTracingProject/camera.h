#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vector.h"
#include <math.h>
#define M_PI 3.141592

vec3 randomPointInCircle() {
	vec3 p;
	do {
		float x = ((float)rand() / (RAND_MAX));
		float y = ((float)rand() / (RAND_MAX));
		float z = 0;
		p = 2 * vec3(x, y, z) - vec3(1, 1, 0);
	} while (p.square_length() >= 1.0);
	return p;
}

class camera {
public:
	vec3 origin, horizontal, vertical, lower_left_corner;
	float lens_radius, time0, time1;
	vec3 right, finalUp, forward;

	camera(vec3 cameraPos, vec3 LookAt, vec3 up ,float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) {
		lens_radius = aperture / 2;
		forward = cameraPos - LookAt;
		forward.make_unit_vector();
		right = cross(up, forward);
		right.make_unit_vector();
		finalUp = cross(forward, right);
		finalUp.make_unit_vector();
		origin = cameraPos;

		float theta = vfov * M_PI / 180;
		float height_half = tan(theta / 2);
		float width_half = aspect * height_half;

		lower_left_corner = vec3(-width_half, -height_half, -1);
		lower_left_corner = origin - width_half *focus_dist* right - height_half * finalUp * focus_dist - forward * focus_dist;
		horizontal = 2 * width_half * right * focus_dist;
		vertical = 2 * height_half * finalUp * focus_dist;

		time0 = t0;
		time1 = t1;
		
	}

	ray get_ray(float u, float v) {
		float trand = ((float)rand() / (RAND_MAX));
		float randTime = time0 + trand * (time1 - time0);
		vec3 rd = lens_radius * randomPointInCircle();
		vec3 offset = right * rd.x() + finalUp * rd.y();
		offset = vec3(0, 0, 0);
		return ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset, randTime);
	}

};

#endif