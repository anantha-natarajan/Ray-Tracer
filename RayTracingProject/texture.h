#ifndef TEXTURE_H
#define TEXTURE_H

class texture {
public:
	virtual vec3 getValue(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture {
public:
	vec3 color;
	constant_texture(){}
	constant_texture(vec3 c) {
		color = c;
	}
	virtual vec3 getValue(float u, float v, const vec3& p) const {
		return color;
	}
};

class checker_texture : public texture {
public:
	int nx, ny;
	vec3 colorA, colorB;
	checker_texture(vec3 c1, vec3 c2, int A, int B) : nx(A), ny(B), colorA(c1), colorB(c2){}
	virtual vec3 getValue(float u, float v, const vec3& p) const {
		int i = u * nx;
		int j = v * ny;

		if ((i + j) % 2 == 0) {
			return colorA;
		}
		else {
			return colorB;
		}
	}
};

class image_texture : public texture {
public:
	unsigned char *data;
	int nx, ny;
	image_texture(unsigned char* pixels, int x, int y) {
		data = pixels;
		nx = x;
		ny = y;
	}

	virtual vec3 getValue(float u, float v, const vec3& p) const {

		int i = u * nx;
		int j = (1 - v) * ny - 0.001;
		if (i < 0) i = 0;
		if (j < 0) j = 0;
		if (i > nx-1) i = nx-1;
		if (j > ny - 1) j = ny - 1;
		float r = int(data[3 * i + 3 * nx*j]) / 255.0;
		float g = int(data[3 * i + 3 * nx*j + 1]) / 255.0;
		float b = int(data[3 * i + 3 * nx*j + 2]) / 255.0;
		return vec3(r, g, b);

	}

};


class planar_image_texture : public texture {
public:
	unsigned char *data;
	int nx, ny;
	planar_image_texture(unsigned char* pixels, int x, int y) {
		data = pixels;
		nx = x;
		ny = y;
	}

	virtual vec3 getValue(float u, float v, const vec3& p) const {

		int i = u * nx;
		int j = (1 - v) * ny - 0.001;
		if (i < 0) i = -i;
		if (j < 0) j = -j;
		if (i > nx - 1) i = i % (nx - 1);
		if (j > ny - 1) j = j % (ny - 1);
		float r = int(data[3 * i + 3 * nx*j]) / 255.0;
		float g = int(data[3 * i + 3 * nx*j + 1]) / 255.0;
		float b = int(data[3 * i + 3 * nx*j + 2]) / 255.0;
		return vec3(r, g, b);

	}

};



#endif
