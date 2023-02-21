#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class material;

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
// Optimizations:  From the tutorial several optimizations can be achieved.
// 1)  A vector dotted with itself is equal to the squared length of that vector:
//     auto a            = dot(r.direction(), r.direction());
//     // Becomes:
//     auto a            = r.direction().length_squared();
// 2)  The quadratic equation is:
//     x = (-b +/- sqrt(b^2-4*a*c)) / (2*a)
//     If instead b = 2*h then:
//     x = (-2*h +/ sqrt(4*h^2 - 4*a*c)) / (2*a)
//       = (-h +/- sqrt(h^2 - a*c)) / a
//

class sphere : public hittable
{
  public:
    sphere() {}
    sphere(point3 cen, double r, std::shared_ptr<material> m) : center{cen}, radius{r}, mat_ptr{m} {};

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

  public:
    point3                    center{0, 0, 0};
    double                    radius  = 1.0;
    std::shared_ptr<material> mat_ptr = nullptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc     = r.origin() - center;
    auto a      = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c      = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return false;
    }
    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    rec.t               = root;
    rec.p               = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif