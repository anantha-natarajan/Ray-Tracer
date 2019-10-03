#include<iostream>
#include<fstream>
#include<math.h>
#include"vector.h"
#include"ray.h"
#include"sphere.h"
#include"hitable_list.h"
#include"float.h"
#include"camera.h"


using namespace std;

/*float hit_sphere(const vec3& centre, float radius, ray& r) {
	vec3 oc = r.origin() - centre;
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float determinant = b * b - 4 * a*c;
	if (determinant < 0) {
		return -1;
	}
	else {
		return (-b - sqrt(determinant)) / (2 * a);
	}
}

vec3 color(ray &r) {
	float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	if (t > 0) {
		vec3 N = r.point_at_parameter(t) - vec3(0, 0, -1);
		N.make_unit_vector();
		return 0.5*vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}		
	vec3 directionVector= r.direction();
	directionVector.make_unit_vector();
	t = (directionVector.y() + 1)*0.5;
	return((1 - t)*vec3(1, 1, 1) + t * vec3(0.5, 0.7,1));
}*/

vec3 color(ray &r, hitable_list *world) {
	hit_record rec;
	if (world->hit(r, 0, FLT_MAX, rec)) {
		return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else {
		vec3 directionVector = r.direction();
		directionVector.make_unit_vector();
		float t = (directionVector.y() + 1)*0.5;
		return((1 - t)*vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1));
	}
}

int main() {

	fstream file("output9.ppm" , ios::out);
	int nx = 800;
	int ny = 400;
	int ns = 100;
	

	if (file.is_open()) {	
		file << "P3" << endl << nx << " " << ny << endl << "255" << endl;
		camera cam;
		hitable *list[2];
		list[0] = new sphere(vec3(0, 0, -1), 0.5);
		list[1] = new sphere(vec3(0, -100.5, -1), 100);
		hitable_list *world = new hitable_list(list, 2);

		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				vec3 col(0, 0, 0);
				for (int k = 0; k < ns; k++) {
					float r = ((double)rand() / (RAND_MAX));
					float u = float(i + r) / float(nx);
					r = ((double)rand() / (RAND_MAX));
					float v = float(j + r) / float(ny);
					ray R = cam.get_ray(u, v);
					col += color(R, world);
				}
				col /= float(ns);
				int r = int(255.99*col[0]);
				int g = int(255.99*col[1]);
				int b = int(255.99*col[2]);

				file << r << " " << g << " " << b << " ";
			}
			file << endl;
		}

	}

	vec3 v1(1, 2, 3), v2(4,5,6), v3;
	float t = 2;

	v3 = v1 + v2 * t;

//	cin >> v3;
	cout << v3 << endl;

	system("pause");

	return 0;

}