# heightmap2obj
A small and simple C++ CLI tool that converts greyscale heightmaps to 3D OBJ models. Uses [lodepng](https://github.com/lvandeve/lodepng) and [obj](https://github.com/rlk/obj).

## Usage
`./hmap2obj <pngpath> <width> <height> <objpath> <extentX> <extentZ> <maxY>`

* pngpath - The relative path to the input png file.
* width   - The width of the input png file in pixels.
* height  -	The height of the input png file in pixels.
* objpath -	The relative path to the output obj file.
* extentX - The resulting model extent on the X-axis.
* extentZ -	The resulting model extent on the X-axis.
* maxY    -	White pixels will be mapped to this Y value. All other pixels will be mapped to a value between [0; maxY] according to their greyscale value. A black pixel will always map to Y = 0.

## Example
### Input PNG
![Height map of the Grand Canyon](https://raw.githubusercontent.com/emberflare/heightmap2obj/master/example_screenshots/input.png)
### Output OBJ (opened in Blender)
![3D model of the Grand Canyon](https://raw.githubusercontent.com/emberflare/heightmap2obj/master/example_screenshots/output.png)

