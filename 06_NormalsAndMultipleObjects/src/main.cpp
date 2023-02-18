
#include "raytrace_config.h"

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

// If the cast ray hits the sphere t will be the value used to
// compute the point of intersection.  Create a new ray from the
// center of the sphere to this point.  This vector is the
// surface normal.  The value can range from -1 to 1.  Scale to
// 0 to 1 to get color gradient to color the sphere.

color ray_color(const ray &r, const hittable &world)
{
    hit_record rec;
    if (world.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t              = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char **argv)
{
    // CMakefile version info example:

    // report version
    //    std::cout << argv[0] << " Version " << baseline_png_VERSION_MAJOR << "."
    //              << baseline_png_VERSION_MINOR << std::endl;
    //    std::cout << "Usage: " << argv[0] << " number" << std::endl;

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int  image_width  = 400;
    const int  image_height = static_cast<int>(image_width / aspect_ratio);

    // World

    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera

    // Do not enjoy that camera is disjointed.  This should be a matrix.  I'm hopeful that
    // later lessons move in that direction.

    auto viewport_height = 2.0;
    auto viewport_width  = aspect_ratio * viewport_height;
    auto focal_length    = 1.0;

    auto origin            = point3{0.0, 0.0, 0.0};
    auto horizontal        = vec3{viewport_width, 0, 0};  // 16.0 / 9.0 * 2.0, 0, 0 => 32.0 / 9.0, 0, 0
    auto vertical          = vec3{0, viewport_height, 0}; // 0, 2, 0
    auto lower_left_corner = origin - (horizontal / 2) - (vertical / 2) - vec3{0, 0, focal_length};
    // (0,0,0) - (32/9, 0, 0) / 2 - (0,2,0) / 2 - (0,0,1) =
    // (0,0,0) - (16/9, 0, 0) - (0,1,0) - (0,0,1) =
    // (-16/9, -1, -1)

    // Render

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto u = double(i) / (image_width - 1);  // u goes from 0 to 1
            auto v = double(j) / (image_height - 1); // v goes from 1 to 0
            ray  r{origin, lower_left_corner + u * horizontal + v * vertical - origin};
            // first time through, u = 0, v = 1, hence ray is from (0,0,0) to (lower_left_corner + 0 * horizontal + 1.0 * vertical - (0,0,0)) =
            // (0,0,0) to (-16/9, -1, -1) + (0,2,0) = (0,0,0) to (-16/9,1,-1)
            // so his drawing is different than this evaluation.  Boundaries for X coordinate is -16/9 to 16/9.
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}