#ifndef	OBJ_FILE_H
#define	OBJ_FILE_H

#include "ray.h"
#include "vector.h"
#include "hitable.h"
#include "hitable_list.h"
#include "triangle.h"
#include<string>
#include<iostream>
#include<fstream>

class obj_file : public hitable {
private:
	vec3 *vertices;
	vec3 *faces;
	int nFaces, nVertices;
public:
	hitable_list *ptr;
	material *my_mat;
	string filename;
	obj_file(string file, int vn, int fn, material *mat);
	void read_obj(string filename, int vn, int fn);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};

void obj_file::read_obj(string filename, int vn, int fn) {
	string token, line;
	string fname = filename;
	string v = "v";
	ifstream objfile(fname, ios::binary);
	if (objfile.is_open()) {
		do {
			getline(objfile, line);
			objfile >> token;
		} while (token.compare(v));
		cout << line << endl;
		//cout << typeid(token).name()<< endl;
		//cout << typeid(v).name() << endl;
		vertices = new vec3[vn];
		vec3 tmp;
		int vCount = 0;
		while (token.compare(v) == 0) {
			float x, y, z;
			objfile >> token;
			x = stof(token);
			objfile >> token;
			y = stof(token);
			objfile >> token;
			z = stof(token);
			vertices[vCount] = vec3(x, y, z);
			vCount++;
			objfile >> token;
		}

		//iterate until faces
		v = "f";
		do {
			getline(objfile, line);
			objfile >> token;
		} while (token.compare(v));

		faces = new vec3[fn];
		int fCount = 0;
		int ptr;
		string val;
		while (token.compare(v) == 0) {
			float x, y, z;
			objfile >> token;
			ptr = token.find("/");
			val = token.substr(0, ptr);
			x = stof(val) - 1;

			objfile >> token;
			ptr = token.find("/");
			val = token.substr(0, ptr);
			y = stof(val) - 1;

			objfile >> token;
			ptr = token.find("/");
			val = token.substr(0, ptr);
			z = stof(val) - 1;
			faces[fCount] = vec3(x, y, z);
			fCount++;

			objfile >> token;
		}

		cout << line << endl;
	}
	objfile.close();

}

obj_file::obj_file(string file, int vn, int fn,  material *mat) {
	my_mat = mat;
	filename = file;
	read_obj(file, vn, fn);
	hitable **triangleList = new hitable*[fn];
	vec3 tmpNormals(0, 0, 0);
	for (int i = 0; i < fn; i++) {
		triangleList[i] = new triangle(vertices[ (int)faces[i].x() ] , vertices[(int)faces[i].y()], vertices[(int)faces[i].z()], tmpNormals, tmpNormals, tmpNormals, my_mat);
		cout << i << endl;
	}

	//triangleList[0] = new triangle(vertices[(int)faces[0].x()], vertices[(int)faces[0].y()], vertices[(int)faces[0].z()], tmpNormals, tmpNormals, tmpNormals, mat);
	
	ptr = new hitable_list(triangleList, fn);
}

bool obj_file::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	return ptr->hit(r, t_min, t_max, rec);
}

#endif