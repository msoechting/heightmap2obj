#include <iostream>
#include <windows.h>
#include "lodepng.h"
#include <vector>
#include <ctime>
#include "obj.h"
#include "hmap2obj.h"


using namespace std;

string ExtractPath(string path) {
	string::size_type pos = string(path).find_last_of("\\/");
	return string(path).substr(0, pos + 1);
}

int main(int argc, char** argv)
{
	if (argc != 8) {
		cout << endl;
		cout << "Usage:" << endl; 
		cout << "hmap2obj <pngpath> <width> <height> <objpath> <extentX> <extentZ> <maxY> <prec>" << endl;
		cout << endl;
		cout << "<pngpath>\tThe relative path to the input png file." << endl;
		cout << "<width> \tThe width of the input png file." << endl;
		cout << "<height>\tThe height of the input png file." << endl;
		cout << "<objpath>\tThe relative path to the output obj file." << endl;
		cout << "<extentX>\tThe resulting model extent on the X-axis." << endl;
		cout << "<extentZ>\tThe resulting model extent on the Z-axis." << endl;
		cout << "<maxY>\t\tWhite pixels will be mapped to this Y value. All other pixels" << endl 
					<< "\t\twill be mapped to a value between [0; maxY] according to their" << endl 
					<< "\t\tgreyscale value. A black pixel will always map to Y = 0." << endl;
		cout << endl;
		cout << "\tExample: hmap2obj map.png 640 640 map.obj 16 16 32" << endl;
		return -1;
	}


	std::clock_t start;
	start = std::clock();
	double duration;
	string filename = argv[1];
	string path = ExtractPath(argv[0]);
	string fullpath = path + filename;
	int precision = 5;
	unsigned int w = atoi(argv[2]);
	unsigned int h = atoi(argv[3]);
	unsigned int objXExtent = atoi(argv[5]);
	unsigned int objZExtent = atoi(argv[6]);
	float maxElevation = atof(argv[7]);
	
	cout << "Decoding input image...";
	vector<unsigned char> data;
	unsigned int error = lodepng::decode(data, w, h, fullpath, LCT_GREY, 8U);
	if (error) {
		cout << "Decoder error: " << error << ": " << lodepng_error_text(error) << endl;
		return -1;
	}
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "done. (" << duration << "s)" << endl;
	
	start = std::clock();
	cout << "Creating vertices...";
	obj *O;
	O = obj_create(nullptr);
	float coords[] = { 0.0f, 0.0f, 0.0f };
	//#pragma omp parallel for
	for (int i = 0; i < data.size(); i++) {
		int v = obj_add_vert(O);
		coords[0] = (i % w) / (float) w * objXExtent;
		coords[1] = data[i] / 255.f * maxElevation;
		coords[2] = (i / w) / (float) h * objZExtent;
		obj_set_vert_v(O, v, coords);
	}
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "done. (" << duration << "s)" << endl;

	cout << "Creating polygons...";
	start = std::clock();
	int surface = obj_add_surf(O);
	int vertices[] = { 0, 0, 0 };
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
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "done. (" << duration << "s)" << endl;

	cout << "Writing obj file...";
	start = std::clock();
	string objpath = path + argv[4];
	obj_write(O, objpath.c_str(), nullptr, precision);
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "done. (" << duration << "s)" << endl;
    return 0;
}