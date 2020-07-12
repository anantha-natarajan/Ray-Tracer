
#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
using namespace std;

class vec3 {
public:
	float e[3];

	//constructors
	vec3() {}
	vec3(float a, float b, float c) {
		e[0] = a;
		e[1] = b;
		e[2] = c;
	}

	//functions
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	//overload operators declare & define
	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline float operator[] (int i) const { return e[i]; }
	//inline float& operator[] (int i) { return e[i]; }

	//overloading unary operations on Vector
	inline vec3& operator+=(const vec3 &v2) {
		e[0] = e[0] + v2.e[0];
		e[1] = e[1] + v2.e[1];
		e[2] = e[2] + v2.e[2];
		return *this;
	}
	inline vec3& operator-=(const vec3 &v2) {
		e[0] = e[0] - v2.e[0];
		e[1] = e[1] - v2.e[1];
		e[2] = e[2] - v2.e[2];
		return *this;
	}
	inline vec3& operator/=(const vec3 &v2) {
		e[0] = e[0] / v2.e[0];
		e[1] = e[1] / v2.e[1];
		e[2] = e[2] / v2.e[2];
		return *this;
	}
	inline vec3& operator*=(const vec3 &v2) {
		e[0] = e[0] * v2.e[0];
		e[1] = e[1] * v2.e[1];
		e[2] = e[2] * v2.e[2];
		return *this;
	}
	inline vec3& operator*=(float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	inline vec3& operator/=(float t) {
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}

	//overloading binary operations on Vector
	// if I overloaded operators as member functions, I was not able to call those overloaded operator member function from another class
	//hence im commenting these and overloading operators as non member functions
	/*inline vec3& operator+(const vec3 &v1) {
		e[0] += v1.e[0];
		e[1] += v1.e[1];
		e[2] += v1.e[2];
		return *this;
	}
	inline vec3& operator-(const vec3 &v1) {
		e[0] -= v1.e[0];
		e[1] -= v1.e[1];
		e[2] -= v1.e[2];
		return *this;
	}
	inline vec3& operator*(const vec3 &v1) {
		e[0] *= v1.e[0];
		e[1] *= v1.e[1];
		e[2] *= v1.e[2];
		return *this;
	}
	inline vec3& operator/(const vec3 &v1) {
		e[0] /= v1.e[0];
		e[1] /= v1.e[1];
		e[2] /= v1.e[2];
		return *this;
	}
	inline vec3& operator*(float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	inline vec3& operator/(float t) {
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}
	*/
	
	//overloadng input and output stream classes

	//functions of class
	inline float length() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float square_length() const {
		return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline void make_unit_vector() {
		float mag = length();
		e[0] /= mag;
		e[1] /= mag;
		e[2] /= mag;
	}

};

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] +v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, float t) {
	return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator*(float t, const vec3 &v1) {
	return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator/(const vec3 &v1, float t) {
	return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}


inline float dot(const vec3 &v1, const vec3 &v2) {
	float f = v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
	return f;
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
	vec3 v3;
	v3.e[0] = v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1];
	v3.e[1] = v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2];
	v3.e[2] = v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0];
	return v3;
}

inline ostream& operator<<(ostream& os,const vec3 &t) {
	os << t.e[0] << endl << t.e[1] << endl << t.e[2] << endl;
	return os;
}

inline istream& operator>>(istream& is, vec3 &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}


inline float distanceVectors(const vec3 &v1, const vec3 &v2) {
	float xComp = (v2.e[0] - v1.e[0])*(v2.e[0] - v1.e[0]);
	float yComp = (v2.e[1] - v1.e[1])*(v2.e[1] - v1.e[1]);
	float zComp = (v2.e[2] - v1.e[2])*(v2.e[2] - v1.e[2]);
	float d = sqrt(xComp + yComp + zComp);
	return d;
}

#endif