#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include<iostream>
#include<fstream>
#include<math.h>
#include<string>
#include<stb_image.h>

#include"vector.h"
#include"ray.h"
#include"float.h"

#include"camera.h"
#include"hitable_list.h"
#include"material.h"
#include"sphere.h"
#include"plane.h"
#include"triangle.h"
#include"Light.h"
#include"platonic_shapes.h"
#include"obj_file.h"


using namespace std;

#define PI 3.14159265

vec3 LightPosition(2, 2, 0);
vec3 LightLookAt(0, 0, -1);
float LightAngle = 10;

static int c = 0;

int nx = 800;
int ny = 400;
vec3 LookFrom = vec3(0, 0, 1);
vec3 LookAt = vec3(0, 0, -1);
float aperture = 2;
float dist_to_focus = (LookFrom - LookAt).length();
float time0 = 0;
float time1 = 1;
//camera cam(LookFrom, LookAt , vec3(0, 1, 0), 60, float(nx) / float(ny), aperture, dist_to_focus, time0, time1);
camera cam(LookFrom, LookAt, vec3(0, 1, 0), 60, float(nx) / float(ny), aperture, 1, time0, time1);

vec3 color(const ray &r, hitable_list *world, int depth, Light *oneLight, texture *env) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		c++;
		/*
		//cout << "True" << endl;
		ray scattered;
		vec3 attenuation;	
		if (rec.mat->scatter(r, rec, attenuation, scattered) && depth < 2) {
			return attenuation * color(scattered, world, depth+1);
		}
		else {
			return vec3(0, 0, 0);
		}
		*/

		vec3 directionLightSource;
		oneLight->calculateDirection(rec, directionLightSource);
		vec3 LightPos;
		oneLight->getPosition(LightPos);

		vec3 attenuation;
		ray scattered;
		ray refracted;
		float kr;
		if (rec.mat->scatter(r, rec, attenuation, scattered, LightPos, directionLightSource, refracted, kr) && depth<2) {
			// it's a reflective object
			//cout << "hitting a reflective surface" << endl;
			vec3 pixelColor;
			if (kr == 0) {
				pixelColor = color(refracted, world, depth + 1, oneLight, env);
			}
			else if (kr == 1) {
				pixelColor = 0.8 * color(scattered, world, depth + 1, oneLight, env);
			}
			else if(kr > 0 && kr < 1){
				//fresnel
				//cout << "Im here";
				vec3 colorFromReflection = 0.6*color(scattered, world, depth + 1, oneLight, env);
				vec3 colorFromRefraction = color(refracted, world, depth + 1, oneLight, env);

				pixelColor = kr * colorFromReflection + (1 - kr)*colorFromRefraction;

			}
			
			return pixelColor;
			//cout << "yo" << endl;
		}
		else {
			// it's a lambertian material so calculate shadow ray
			ray shadowRay(rec.p, directionLightSource, r.time());
			hit_record shadowRec;
			if (false && world->hit(shadowRay, 0.001, FLT_MAX, shadowRec)) {
				//the hit point is in shadow;
				float dist_o_l, dist_o_o;
				dist_o_l = distanceVectors(rec.p, LightPos);
				dist_o_o = distanceVectors(rec.p, shadowRec.p);
				if (dist_o_l > dist_o_o) {
					return vec3(0, 0, 0);
				}
				else {
					vec3 pixelColor;
					oneLight->calculateColor(shadowRay, rec, pixelColor);
					return pixelColor;
				}
			}
			else {
				float dist_o_l = distanceVectors(rec.p, LightPos);
				vec3 pixelColor;
				oneLight->calculateColor(shadowRay, rec, pixelColor);
				return pixelColor;
			}
		}

	}
	else {

		//return vec3(1, 0, 0);
	
		vec3 n = r.direction();
		n.make_unit_vector();

		
		float phi = atan2(n.z(), n.x());
		float theta = asin(n.y());
		float u = 1 - (phi + M_PI) / (2 * M_PI);
		float v = (theta + M_PI / 2) / M_PI;
		
		/*
		float phi = atan2(n.z(), n.x());
		float theta = asin(n.y());
		float u = 0.5 + (phi) / (2 * M_PI);
		float v = 0.5 - (theta) / M_PI;*/

		if (u > 1 ) {
			u = 1;
		}
		if (v > 1) {
			v = 1;
		}

		if (u < 0)
			u = 0;
		if (v < 0)
			v = 0;
		vec3 envColor = env->getValue(u, v, n);
		return envColor;
		
	}
}

int main() {
	int width = 4096, height = 4096, n;
	unsigned char *pixmapTexture = new unsigned char[width * height * 3];
	//stbi_set_flip_vertically_on_load(true);
	pixmapTexture = stbi_load("pebbles.jpg", &width, &height, &n, 0);

	int widthE = 1500;
	int heightE = 750;
	int m;
	unsigned char *envmapTexture = new unsigned char[widthE * heightE * 3];
	envmapTexture = stbi_load("sunrise.jpg", &widthE, &heightE, &m, 0);
	texture *env = new planar_image_texture(envmapTexture, widthE, heightE);

	int widthB = 512;
	int heightB = 256;
	int l;
	unsigned char *ballTexture = new unsigned char[widthB * heightB * 3];
	ballTexture = stbi_load("football.png", &widthB, &heightB, &l, 0);

	int widthW = 512, heightW = 384, w;
	unsigned char *waterTexture = new unsigned char[widthW * heightW * 3];
	//stbi_set_flip_vertically_on_load(true);
	waterTexture = stbi_load("water.png", &widthW, &heightW, &w, 0);


	int ns = 100;
	int  frame = 43;
	//for (float i = -1; i <= 1.1; i = i+0.05, frame++) {

		string filename = "animation\\sphereMovingHighFrameRate";
		string filenum = std::to_string(frame);
		string extension = ".ppm";
		fstream file(filename + filenum + extension, ios::out);
		if (file.is_open()) {
			file << "P3" << endl << nx << " " << ny << endl << "255" << endl;
			//camera cam(vec3(0, 0, 1), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(nx) / float(ny));
			hitable *list[5];

			material *worldMap = new lambertian(new planar_image_texture(pixmapTexture, width, height), 1, 1, 1);
			material *worldMap2 = new lambertian(new planar_image_texture(envmapTexture, widthE, heightE), 1, 1, 1);
			material *tennisball = new lambertian(new image_texture(ballTexture, widthB, heightB), 1, 1, 1);
			material *checkerBoard = new lambertian(new checker_texture(vec3(0,0,0),vec3(1,1,1), 1, 1), 4, 2, 1);
			material *checkerSurface = new lambertian(new checker_texture(vec3(0, 0, 0), vec3(1, 1, 1), 9, 9), 1, 1, 1);
			material *surface = new lambertian(new constant_texture(vec3(0.6, 0.2, 0.7)), 1.0, 1.0, 1.0);
			material *bg = new lambertian(new constant_texture(vec3(1.0, 0.0, 0.0)), 1.0, 1.0, 1.0);
			material *xyPlane = new lambertian(new constant_texture(vec3(0.6, 0.2, 0.7)), 1.0, 1.0, 1.0);
			material *objmat = new lambertian(new constant_texture(vec3(0.2, 0.6, 0.3)), 1.0, 1.0, 1.0);
			material *metalSurface = new metal(new constant_texture(vec3(0.6, 0.2, 0.7)), 1.0, 1.0, 1.0);
			material *transSurface = new transparent(new constant_texture(vec3(0.6, 0.2, 0.7)), 1.0, 1.0, 1.0, 1.5);
			material *fresnelSurface = new fresnelMaterial(new planar_image_texture(waterTexture, widthW, heightW), 1.0, 1.0, 1.0, 1.2);
			//Assignment 05
			//list[0] = new plane(vec3(0, 0, -1), vec3(0, 0, -2), new lambertian(new constant_texture(vec3(0.2, 0.2, 0.3)), 1.0, 1.0, 1.0));

			vec3 *v[4];
			v[0] = new vec3(0, 0.5, -0.75);
			v[1] = new vec3(-0.5, -0.5, -1);
			v[2] = new vec3(0, -0.5, -0.5);
			v[3] = new vec3(0.5, -0.5, -1);

			//list[0] = new obj_file("bunny.obj", 77, 150, metalSurface);
			//list[0] = new obj_file("cube_rotated.obj", 8, 12, metalSurface);
			//list[0] = new platonic_shapes(v, 4, metalSurface);
			//list[0] = new sphere(vec3(0, 0, -2), 0.5, worldMap);
			//list[2] = new sphere(vec3(1, 0, -1), 0.5, objmat);
			//list[0] = new plane(vec3(0, -1, 0), vec3(0, -1, 0), worldMap);
			//list[0] = new sphere(vec3(0, 0.5, -2), 0.5, xyPlane);
			//list[1] = new sphere(vec3(-1.0, 0, -3.0), 0.5, objmat);
			//list[1] = new plane(vec3(0, -1, 0), vec3(0, -2, 0), checkerBoard);
			//list[2] = new plane(vec3(0, -1, 0), vec3(0, -3.25, 0), worldMap);

			//list[2] = new sphere(vec3(-3, 0, -2), 0.5, xyPlane);
			//list[1] = new plane(vec3(0, -1, 0), vec3(0, -1, 0), metalSurface);
			//list[2] = new plane(vec3(0, 0, -1), vec3(0, 0, -2), worldMap);
		
			//list[1] = new sphere(vec3(-1, 0, -3), 0.5, objmat);

			/*
			material *s1 = new lambertian(new constant_texture(vec3(1.0, 0.0, 0.0)), 1.0, 1.0, 1.0);
			material *s2 = new lambertian(new constant_texture(vec3(0.0, 1.0, 0.0)), 1.0, 1.0, 1.0);
			material *s3 = new lambertian(new constant_texture(vec3(0.0, 0.0, 1.0)), 1.0, 1.0, 1.0);
			material *s4 = new lambertian(new constant_texture(vec3(0.6, 0.2, 0.7)), 1.0, 1.0, 1.0);
			vec3 tmpNormals(0, 0, 0);
			list[0] = new triangle(v[0], v[1], v[2], tmpNormals, tmpNormals, tmpNormals, s1);
			list[1] = new triangle(v[0], v[2], v[3], tmpNormals, tmpNormals, tmpNormals, s2);
			list[2] = new triangle(v[0], v[3], v[1], tmpNormals, tmpNormals, tmpNormals, s3);
			list[3] = new triangle(v[1], v[2], v[3], tmpNormals, tmpNormals, tmpNormals, s4);*/


			//list[0] = new triangle(v[0], v[2], v[3], tmpNormals, tmpNormals, tmpNormals, s2);
			//list[0] = new triangle(vec3(-1,0,-2), vec3(1, 0, -2), vec3(0, 0.3, -4), vec3(0,0,1), vec3(0, 0, 1), vec3(0, 0, 1), bg);
			//list[1] = new plane(vec3(0, -1, 0), vec3(0, -1, 0), surface);
			//list[2] = new sphere(vec3(0, 0, -1), 0.5, checkerBoard);
			//list[2] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(new constant_texture(vec3(0.8, 0.3, 0.3)), 1.0, 1.0, 1.0));
			//list[3] = new sphere(vec3(2, 0, -1), 0.5, new lambertian(new constant_texture(vec3(0.8, 0.2, 0.6)), 1.0, 1.0, 1.0));
			//list[4] = new sphere(vec3(-2, 0, -1), 0.5, new lambertian(new constant_texture(vec3(0.8, 0.6, 0.2)), 1.0, 1.0, 1.0));

			/*if (i == -1) {
				//list[0] = new sphere(vec3(-1, 0, -2), 0.5, tennisball);
			}
			else if (i == 1) {
				//list[0] = new sphere(vec3(1, 0, -2), 0.5, tennisball);
			}
			else {
				//list[0] = new movingSphere(vec3(i, 0, -2), vec3(i + 0.1, 0, -2), 0, 1, 0.5, tennisball);
			}*/

			list[0] = new sphere(vec3(1, 0, -2), 0.5, tennisball);
			


			//Light *oneLight = new DirectionalLight(LightPosition, LightLookAt);
			Light *oneLight = new PointLight(LightPosition);
			//Light *oneLight = new SpotLight(LightPosition, LightLookAt, LightAngle);
			hitable_list *world = new hitable_list(list, 1);

			for (int j = ny - 1; j >= 0; j--) {
				for (int i = 0; i < nx; i++) {
					vec3 col(0, 0, 0);
					for (int k = 0; k < ns; k++) {
						float r = ((double)rand() / (RAND_MAX));
						float u = float(i + r) / float(nx);
						r = ((double)rand() / (RAND_MAX));
						float v = float(j + r) / float(ny);
						ray R = cam.get_ray(u, v);
						col += color(R, world, 0, oneLight, env);
					}
					col /= float(ns);
					//col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
					int r = int(255.99*col[0]);
					int g = int(255.99*col[1]);
					int b = int(255.99*col[2]);

					file << r << " " << g << " " << b << " ";
				}
				file << endl;
			}

		}

		file.close();
		cout << "File No " << frame << " completed" << endl;

	//}
	vec3 v1(1, 2, 3), v2(4,5,6), v3;
	float t = 2;

	v3 = v1 + v2 * t;

//	cin >> v3;
	cout << v3 << endl;
	cout << "number of hits " << c << endl;

	system("pause");

	return 0;

}

//52485394