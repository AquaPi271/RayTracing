#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class material;

// A normal's direction from the surface indicates front or back face
// by the convention chosen.  In this case, a normal will emanate
// outwards from the front face.

struct hit_record
{
    point3                    p;
    vec3                      normal;
    std::shared_ptr<material> mat_ptr;
    double                    t;
    bool                      front_face;

    inline void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal     = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
  public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

#endif