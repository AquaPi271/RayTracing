#include "color.h"
#include "ray.h"
#include "raytrace_config.h"
#include "vec3.h"

#include <iostream>

// Sphere equation with radius r and center at point C.
// Let P be a point on the sphere then:
// P = (Px,Py,Pz), C = (Cx,Cy,Cz)
// (Px-Cx)^2 + (Py-Cy)^2 + (Pz-Cz)^2 = r^2
// (P - C)^2 = r^2
// (P - C)(P - C) = r^2
// Let ray P(t) = A + t*b.  Solving for when this ray intersects P.
// (P(t) - C)(P(t) - C) = r^2
// (A + t*b - C)(A + t*b - C) - r^2 = 0
// A*A + 2*A*t*b - 2*A*C -t*t*b*b - 2*t*b*C + C*C - r^2 = 0
// -t*t*b*b + t*2*b(A - C) + A*A - 2*A*C + C*C - r^2 = 0
// Quadritic equation:  ax*x + b*x + c = 0
// x = (-b +/- sqrt(b*b - 4*a*c))/2
//
// From above:
// a = -b*b  [b is the direction vector of the ray]
// b = 2*b*(A-C) [A is the source of the ray, C is the center of the circle]
// c = (A-C)^2 - r^2 [r is the radius]
// t exists only when b*b - 4*a*c >= 0, hence only need to check it.
// Update: Now send back -1.0 if there is no interesection and the value of t
// for where the ray intersects the sphere.

double hit_sphere(const point3 &center, double radius, const ray &r)
{
    vec3 oc           = r.origin() - center;
    auto a            = dot(r.direction(), r.direction());
    auto b            = 2.0 * dot(oc, r.direction());
    auto c            = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-b - std::sqrt(discriminant)) / (2.0 * a);
    }
}

// If the cast ray hits the sphere t will be the value used to
// compute the point of intersection.  Create a new ray from the
// center of the sphere to this point.  This vector is the
// surface normal.  The value can range from -1 to 1.  Scale to
// 0 to 1 to get color gradient to color the sphere.

color ray_color(const ray &r)
{
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        vec3 N = unit_vector(r.at(t) - vec3{0, 0, -1});
        return 0.5 * color{N.x() + 1, N.y() + 1, N.z() + 1};
    }
    vec3 unit_direction = unit_vector(r.direction());
    t                   = 0.5 * (unit_direction.y() + 1.0);
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
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}