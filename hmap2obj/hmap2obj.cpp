#include <iostream>
#include <windows.h>
#include "lodepng.h"
#include <vector>
#include "obj.h"
#include "hmap2obj.h"


using namespace std;

string ExtractPath(string path) {
	string::size_type pos = string(path).find_last_of("\\/");
	return string(path).substr(0, pos + 1);
}

string ExtractName(string path) {
	string::size_type pos = string(path).find_last_of(".");
	return string(path).substr(0, pos + 1);
}

int main(int argc, char** argv)
{
	if (argc != 5) {
		cout << "Usage: hmap2obj <input png path> <width> <height> <output obj name>" << endl;
		cout << "Example: hmap2obj map.png 640 640 map.obj" << endl;
		return -1;
	}

	cout << "Decoding input image..." << endl;
	string filename = argv[1];
	string path = ExtractPath(argv[0]);
	string fullpath = path + filename;
	int precision = 5;
	unsigned int w = atoi(argv[2]);
	unsigned int h = atoi(argv[3]);
	vector<unsigned char> data;
	unsigned int error = lodepng::decode(data, w, h, fullpath, LCT_GREY, 8U);
	if (error) {
		cout << "Decoder error: " << error << ": " << lodepng_error_text(error) << endl;
		return -1;
	}

	cout << "Creating vertices..." << endl;
	obj *O;
	O = obj_create(nullptr);
	float coords[] = { 0.0f, 0.0f, 0.0f };
	//#pragma omp parallel for
	for (int i = 0; i < data.size(); i++) {
		int v = obj_add_vert(O);
		coords[0] = i % w;
		coords[1] = data[i] / 255.f * MAX_HEIGHT;
		coords[2] = i / w;
		obj_set_vert_v(O, v, coords);
	}

	int surface = obj_add_surf(O);
	int vertices[] = { 0, 0, 0 };
	cout << "Creating polygons..." << endl;
	for (int y = 0; y < h - 1; y++) {
		for (int x = 0; x < w - 1; x++) {
			int p1 = obj_add_poly(O, surface);
			vertices[0] =  y      * w + x + 1;
			vertices[1] =  y      * w + x;
			vertices[2] = (y + 1) * w + x;
			obj_set_poly(O, surface, p1, vertices);
			int p2 = obj_add_poly(O, surface);
			vertices[0] = (y + 1) * w + x;
			vertices[1] = (y + 1) * w + x + 1;
			vertices[2] =  y      * w + x + 1;
			obj_set_poly(O, surface, p2, vertices);
		}
	}
	cout << "Writing obj file..." << endl;
	string objpath = path + argv[4];
	obj_write(O, objpath.c_str(), nullptr, precision);
	cout << "Done!" << endl;
    return 0;
}

