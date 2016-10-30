#include <iostream>
#include <windows.h>
#include "lodepng.h"
#include <vector>
#include "obj.h"

using namespace std;

string ExtractPath(string path) {
	string::size_type pos = string(path).find_last_of("\\/");
	return string(path).substr(0, pos + 1);
}

int main(int argc, char** argv)
{
	/*if (argc != 4) {
		std::cout << "Usage: hmap2obj map.png 640 640 map.obj" << std::endl;
		return -1;
	}*/
	std::string filename = argv[1];
	std::string fullpath = ExtractPath(argv[0]) + filename;
	unsigned int w = atoi(argv[2]);
	unsigned int h = atoi(argv[3]);
	std::vector<unsigned char> data;
	unsigned int error = lodepng::decode(data, w, h, fullpath, LCT_GREY, 8U);
	if (error) {
		unsigned int secondtry_error = lodepng::decode(data, w, h, filename, LCT_GREY, 8U);
		if (secondtry_error) {
			std::cout << "Decoder error: " << error << ": " << lodepng_error_text(error) << std::endl;
			return -1;
		}
	}
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			std::cout << static_cast<unsigned int>(data[x + y * w]) / 25;
		}
		std::cout << endl;
	}
    return 0;
}

