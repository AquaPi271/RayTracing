#include "baseline_vec3_config.h"
#include "color.h"
#include "vec3.h"

#include <iostream>

int main(int argc, char **argv)
{
    // CMakefile version info example:

    // report version
    //    std::cout << argv[0] << " Version " << baseline_png_VERSION_MAJOR << "."
    //              << baseline_png_VERSION_MINOR << std::endl;
    //    std::cout << "Usage: " << argv[0] << " number" << std::endl;

    // Image

    const int image_width  = 256;
    const int image_height = 256;

    // Render

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}