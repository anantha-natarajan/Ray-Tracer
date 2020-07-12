#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "vector.h"
#include "texture.h"

struct hit_record;

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const vec3& lightPos, const vec3& lightDirection, ray& refracted, float& kr) const = 0;
	virtual vec3 getColor(const hit_record& rec) const = 0;
	virtual float getAmbient() const = 0;
	virtual float getDiffuse() const = 0;
	virtual float getSpecular() const = 0;
};

vec3 randomPointInSphere() {
	vec3 p;
	do {
		float x = ((float)rand() / (RAND_MAX));
		float y = ((float)rand() / (RAND_MAX));
		float z = ((float)rand() / (RAND_MAX));
		p = 2 * vec3(x, y, z) - vec3(1, 1, 1);
	} while (p.square_length() >= 1.0);

	return p;
}

class lambertian : public material {

public:
	//vec3 albedo; //diffuse color
	texture *albedo;
	float ambient, diffuse, specular;
	lambertian(texture *a, float ambientPercent, float diffusePercent, float specularPercent) {
		albedo = a;
		float ambient = ambientPercent;
		float diffuse = diffusePercent;
		float specular = specularPercent;
	}

	virtual vec3 getColor(const hit_record& rec) const {
		return albedo->getValue(rec.u, rec.v, rec.p);
	}

	virtual float getAmbient() const {
		return ambient;
	}

	virtual float getDiffuse() const {
		return diffuse;
	}

	virtual float getSpecular() const {
		return specular;
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const vec3& lightPos, const vec3& lightDirection, ray& refracted, float& kr) const {
		//vec3 randVector = randomPointInSphere();
		//vec3 target = rec.p + rec.normal + randVector;
		//scattered = ray(rec.p, target - rec.p);
		//attenuation = albedo;
		//ray shadowRay(rec.p, lightDirection);
		//scattered = shadowRay;
		return false;
		//return false;
	}

};


class metal : public material {

public:
	//vec3 albedo; //diffuse color
	texture *albedo;
	float ambient, diffuse, specular;
	metal(texture *a, float ambientPercent, float diffusePercent, float specularPercent) {
		albedo = a;
		float ambient = ambientPercent;
		float diffuse = diffusePercent;
		float specular = specularPercent;
	}

	virtual vec3 getColor(const hit_record& rec) const {
		return albedo->getValue(rec.u, rec.v, rec.p);
	}

	virtual float getAmbient() const {
		return ambient;
	}

	virtual float getDiffuse() const {
		return diffuse;
	}

	virtual float getSpecular() const {
		return specular;
	}

	virtual vec3 reflect(const vec3& v, const vec3& n) const {
		return v - 2 * dot(v, n)*n;
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const vec3& lightPos, const vec3& lightDirection, ray& refracted, float& kr) const {

		vec3 dir = r_in.direction();
		//dir.make_unit_vector();
		vec3 reflectedDirection = reflect( dir,rec.normal );
		scattered = ray(rec.p, reflectedDirection, r_in.time());
		attenuation = albedo->getValue(rec.u, rec.v, rec.p);
		kr = 1;
		refracted = ray(rec.p, vec3(0,0,0), r_in.time());
		return true;
	}

};


class transparent : public material {

public:
	//vec3 albedo; //diffuse color
	texture *albedo;
	float ambient, diffuse, specular, ior;
	transparent(texture *a, float ambientPercent, float diffusePercent, float specularPercent, float refractiveIndex) {
		albedo = a;
		float ambient = ambientPercent;
		float diffuse = diffusePercent;
		float specular = specularPercent;
		ior = refractiveIndex;
	}

	virtual vec3 getColor(const hit_record& rec) const {
		return albedo->getValue(rec.u, rec.v, rec.p);
	}

	virtual float getAmbient() const {
		return ambient;
	}

	virtual float getDiffuse() const {
		return diffuse;
	}

	virtual float getSpecular() const {
		return specular;
	}


	virtual vec3 refract(const vec3& v, const vec3& n) const {
		vec3 normal, transmittedRay;
		float eta1 = 1;
		float eta2 = ior;
		if (dot(v, n) > 0) {
			//exiting current medium
			normal = -n;
			eta1 = ior;
			eta2 = 1;
		}
		else {
			//entering current medium
			normal = n;
		}

		float eta = eta1 / eta2;
		float costheta1 = dot(v, normal);
		float k = 1 - eta * eta * ( 1 - costheta1*costheta1 );

		if (k < 0) {
			//total internal reflection
			k = 0;
		}
		
		transmittedRay = eta * (v - costheta1 * normal) - normal * sqrt(k);

		return transmittedRay;
	}


	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const vec3& lightPos, const vec3& lightDirection, ray& refracted, float& kr) const {

		vec3 dir = r_in.direction();
		//dir.make_unit_vector();
		vec3 transmitterdDirection = refract(dir, rec.normal);
		refracted = ray(rec.p, transmitterdDirection, r_in.time());
		attenuation = albedo->getValue(rec.u, rec.v, rec.p);
		kr = 0;
		scattered = ray(rec.p, vec3(0, 0, 0), r_in.time());

		return true;
	}

};


class fresnelMaterial : public material {

public:
	//vec3 albedo; //diffuse color
	texture *albedo;
	float ambient, diffuse, specular, ior;
	fresnelMaterial(texture *a, float ambientPercent, float diffusePercent, float specularPercent, float refractiveIndex) {
		albedo = a;
		float ambient = ambientPercent;
		float diffuse = diffusePercent;
		float specular = specularPercent;
		ior = refractiveIndex;
	}

	virtual vec3 getColor(const hit_record& rec) const {
		return albedo->getValue(rec.u, rec.v, rec.p);
	}

	virtual float getAmbient() const {
		return ambient;
	}

	virtual float getDiffuse() const {
		return diffuse;
	}

	virtual float getSpecular() const {
		return specular;
	}

	virtual vec3 refract(const vec3& v, const vec3& n) const {
		vec3 normal, transmittedRay;
		float eta1 = 1;
		float eta2 = ior;
		if (dot(v, n) > 0) {
			//exiting current medium
			normal = -n;
			eta1 = ior;
			eta2 = 1;
		}
		else {
			//entering current medium
			normal = n;
		}

		float eta = eta1 / eta2;
		float costheta1 = dot(v, normal);
		float k = 1 - eta * eta * (1 - costheta1 * costheta1);

		if (k < 0) {
			//total internal reflection
			k = 0;
		}

		transmittedRay = eta * (v - costheta1 * normal) - normal * sqrt(k);

		return transmittedRay;
	}


	virtual vec3 reflect(const vec3& v, const vec3& n) const {
		return v - 2 * dot(v, n)*n;
	}

	virtual float calculateEmittingRay(const vec3& v, const vec3& n) const {

		float kr, kt;
		vec3 normal;
		float eta1, eta2;
		eta1 = 1;
		eta2 = ior;

		float costheta1 = dot(-v, n);

		if (costheta1 > 0.4 && costheta1 < 0.6) {
		//cout << costheta1 << endl;
		}

		if (costheta1 < 0) {
			normal = -n;
			eta1 = ior;
			eta2 = 1;
		}
		else {
			normal = n;
		}

		costheta1 = dot(-v,normal);

		float sintheta2 = eta1 / eta2 * sqrtf(fmax(0.0f, 1 - costheta1 * costheta1));
		if (sintheta2 >= 1) {
			//total internal reflection
			kr = 1;
		}
		else {
			float costheta2 = sqrtf(fmax(0.0f, 1 - sintheta2 * sintheta2));
			costheta2 = fabsf(costheta2);
			float Rs = ((eta2*costheta1) - (eta1*costheta2)) / ((eta2*costheta1) + (eta1*costheta2));
			float Rp = ((eta2*costheta1) - (eta2*costheta2)) / ((eta1*costheta1) + (eta2*costheta2));
			kr = (Rs * Rs + Rp * Rp) / 2;

		}

		if (kr > 0 && kr < 1) {
			//cout << " value" << kr;
		}

		kr = kr > 1 ? 1 : kr;
		kr = kr < 0 ? 0 : kr;

		return kr;

	}



	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const vec3& lightPos, const vec3& lightDirection, ray& refracted, float& kr) const {

		vec3 dir = r_in.direction();
		//dir.make_unit_vector();
		kr = calculateEmittingRay(dir, rec.normal);
		vec3 reflectedRay = reflect(dir, rec.normal);
		vec3 refractedRay = refract(dir, rec.normal);

		if (kr >0  && kr < 1) {
			//has both reflection and refraction
			scattered = ray(rec.p, reflectedRay, r_in.time());
			//attenuation = refractedRay;
			refracted = ray(rec.p, refractedRay, r_in.time());
		}
		else if(kr == 1){
			//only reflection occurs
			scattered = ray(rec.p, reflectedRay, r_in.time());
			refracted = ray(rec.p, vec3(0, 0, 0), r_in.time());
		}
		else if (kr == 0) {
			//only refraction occurs
			refracted = ray(rec.p, refractedRay, r_in.time());
			scattered = ray(rec.p, vec3(0, 0, 0), r_in.time());
		}

		attenuation = vec3(0, 0, 0);
		return true;
	}

};





#endif