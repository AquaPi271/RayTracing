
#include "raytrace_config.h"

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

// If the cast ray hits the sphere t will be the value used to
// compute the point of intersection.  Create a new ray from the
// center of the sphere to this point.  This vector is the
// surface normal.  The value can range from -1 to 1.  Scale to
// 0 to 1 to get color gradient to color the sphere.
// Limit recursion, or ray bouncing, to depth number of recursive calls.

color ray_color(const ray &r, const hittable &world, int depth, bool use_hemisphere_scatter = true)
{
    hit_record rec;

    // If we've exceeded the ray bound limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    // The 0.001 threshold eliminates shadow acne when bounces occur at t not exactly 0.
    // This imprecision comes from floating point limitations.  Applying a tolerance fixes
    // the issue.
    if (world.hit(r, 0.001, infinity, rec))
    {
        // Bounce the ray.  The commented method uses a slightly incorrect distribution
        // to select the reflection direction.
        point3 target{0, 0, 0};
        if (use_hemisphere_scatter)
        {
            target = rec.p + random_in_hemisphere(rec.normal);
        }
        else
        {
            target = rec.p + rec.normal + random_in_unit_sphere();
        }
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1, use_hemisphere_scatter);
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

    const auto aspect_ratio      = 16.0 / 9.0;
    const int  image_width       = 400;
    const int  image_height      = static_cast<int>(image_width / aspect_ratio);
    const int  samples_per_pixel = 100;
    const int  max_depth         = 50;

    // World

    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera

    camera cam;

    // Render

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray  r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth, true);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}