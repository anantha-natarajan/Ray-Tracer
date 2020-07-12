#ifndef PLATONIC_SHAPES_H
#define PLATONIC_SHAPES_H

#include "ray.h"
#include "vector.h"
#include "hitable.h"
#include "triangle.h"

class platonic_shapes : public hitable {
	public:
		vec3 **vertices;
		hitable_list *ptr;
		int nVertices;
		platonic_shapes(vec3 **v, int n, material *mat);
		material *my_mat;
		virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};

platonic_shapes::platonic_shapes(vec3 **v, int n, material *mat) {
	vertices = v;
	nVertices = n;
	my_mat = mat;

	hitable **triangleList = new hitable*[n];
//	for (int i = 0; i < n; i++) {
//		cout << vertices[i]->x() << "\t" << vertices[i]->y() << "\t" << vertices[i]->z() << endl;
//	}

	material *s1 = new lambertian(new constant_texture(vec3(1.0, 0.0, 0.0)), 1.0, 1.0, 1.0);
	material *s2 = new lambertian(new constant_texture(vec3(0.0, 1.0, 0.0)), 1.0, 1.0, 1.0);
	material *s3 = new lambertian(new constant_texture(vec3(0.0, 0.0, 1.0)), 1.0, 1.0, 1.0);
	material *s4 = new lambertian(new constant_texture(vec3(0.2, 0.6, 0.3)), 1.0, 1.0, 1.0);
	vec3 tmpNormals(0, 0, 0);
	triangleList[0] = new triangle(v[0], v[1], v[2], tmpNormals, tmpNormals, tmpNormals, my_mat);
	triangleList[1] = new triangle(v[0], v[2], v[3], tmpNormals, tmpNormals, tmpNormals, my_mat);
	triangleList[2] = new triangle(v[0], v[3], v[1], tmpNormals, tmpNormals, tmpNormals, my_mat);
	triangleList[3] = new triangle(v[1], v[2], v[3], tmpNormals, tmpNormals, tmpNormals, my_mat);
	ptr = new hitable_list(triangleList, 4);

}

bool platonic_shapes::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	return ptr->hit(r, t_min, t_max, rec);
}


#endif
