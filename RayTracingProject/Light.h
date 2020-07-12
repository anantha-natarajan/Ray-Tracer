#ifndef LIGHT_H
#define LIGHT_H

#include <math.h>

#define PI 3.14159265

class Light {
	public:
		virtual bool calculateColor(const ray& r_in, const hit_record& rec, vec3& color) const =0;
		virtual bool calculateDirection(const hit_record& rec, vec3& direction) const = 0;
		virtual void getPosition(vec3& position) const=0;
};

class DirectionalLight : public Light {
	public:
		vec3 Position, Direction;
		DirectionalLight(vec3 p, vec3 LookAt) {
			Position = p;
			Direction = p - LookAt;
			Direction.make_unit_vector();
		}

		virtual void getPosition(vec3& position) const {
			position = Position;
		}

		virtual bool calculateDirection(const hit_record& rec, vec3& direction) const {
			direction = Direction;
			return true;
		}

		virtual bool calculateColor(const ray& shadowRay, const hit_record& rec, vec3& color) const {
			float nl = dot(rec.normal, shadowRay.direction());
			float diffusePercent = std::fmaxf(0.0, nl);
			vec3 objectColor = rec.mat->getColor(rec);
			diffusePercent *= rec.mat->getDiffuse();
			vec3 diffuseColor = objectColor * diffusePercent;

			//calculating illumination: specular
			/*vec3 R = -shadowRay.direction() + 2 * dot(shadowRay.direction(), rec.normal) * rec.normal;
			R.make_unit_vector();
			vec3 Eye = vec3(0, 0, 1) - rec.p;
			Eye.make_unit_vector();
			float sl = dot(Eye, R);
			float specularPercent = std::fmaxf(0.0, sl);
			vec3 specularColor(1.0, 1.0, 1.0);
			specularPercent *= 1;
			color = diffuseColor * (1 - specularPercent) + specularColor * specularPercent;*/
	
			color = diffuseColor;
			return true;
		}
};


class PointLight : public Light {
public:
	vec3 Position;
	PointLight(vec3 p) {
		Position = p;
	}

	virtual void getPosition(vec3& position) const {
		position = Position;
	}

	virtual bool calculateDirection(const hit_record& rec, vec3& direction) const {
		direction = Position - rec.p;
		direction.make_unit_vector();
		return true;
	}

	virtual bool calculateColor(const ray& shadowRay, const hit_record& rec, vec3& color) const {
		float nl = dot(rec.normal, shadowRay.direction());
		float diffusePercent = std::fmaxf(0.0, nl);
		vec3 objectColor = rec.mat->getColor(rec);
		vec3 diffuseColor = objectColor * diffusePercent;
		//diffuseColor = objectColor;
		//calculating illumination: specular
		/*vec3 R = -shadowRay.direction() + 2 * dot(shadowRay.direction(), rec.normal) * rec.normal;
		R.make_unit_vector();
		vec3 Eye = vec3(0, 0, 1) - rec.p;
		Eye.make_unit_vector();
		float sl = dot(Eye, R);
		float specularPercent = std::fmaxf(0.0, sl);
		vec3 specularColor(1.0, 1.0, 1.0);
		specularPercent = (specularPercent * 1);
		color = diffuseColor * (1 - specularPercent) + specularColor * specularPercent;*/

		color = diffuseColor;
		return true;
	}
};


class SpotLight : public Light {
public:
	vec3 Position, Direction;
	float coneAngle;
	SpotLight(vec3 p, vec3 LookAt, float angle) {
		Position = p;
		Direction = p - LookAt;
		Direction.make_unit_vector();
		coneAngle = angle;
	}

	virtual void getPosition(vec3& position) const {
		position = Position;
	}

	virtual bool calculateDirection(const hit_record& rec, vec3& direction) const {
		direction = Position - rec.p;
		direction.make_unit_vector();
		return true;
	}

	virtual bool calculateColor(const ray& shadowRay, const hit_record& rec, vec3& color) const {

		vec3 spotLightNormal(-Direction);
		spotLightNormal.make_unit_vector();
		float slc = dot(shadowRay.direction(), -spotLightNormal);
		float limit = cos(coneAngle * PI / 180.0);
		float LightFactor;
		if (slc < limit) {
			//return vec3(0, 0, 0);
			LightFactor = 0;
		}
		else {
			float d = 1.0 / (1.0 - limit);
			float lf = 1.0 - (1.0 - slc) * d;
			LightFactor = lf;
		}

		float nl = dot(rec.normal, shadowRay.direction());
		float diffusePercent = std::fmaxf(0.0, nl);
		vec3 objectColor = rec.mat->getColor(rec);
		vec3 diffuseColor = objectColor * diffusePercent;

		//calculating illumination: specular
		vec3 R = -shadowRay.direction() + 2 * dot(shadowRay.direction(), rec.normal) * rec.normal;
		R.make_unit_vector();
		vec3 Eye = vec3(0, 0, 1) - rec.p;
		Eye.make_unit_vector();
		float sl = dot(Eye, R);
		float specularPercent = std::fmaxf(0.0, sl);
		vec3 specularColor(1.0, 1.0, 1.0);
		specularPercent = (specularPercent * 1);
		color = diffuseColor * (1 - specularPercent) + specularColor * specularPercent;

		color *= LightFactor;
		//color = diffuseColor;
		return true;
	}
};

#endif