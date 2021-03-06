#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "ray.h"
#include "hitable.h"
#include "vector.h"


class hitable_list : hitable {
public:
	hitable **list;
	int list_size;
	hitable_list() {}
	hitable_list(hitable **l, int size) {
		list = l;
		list_size = size;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}


#endif // !HITABLE_LIST_H

