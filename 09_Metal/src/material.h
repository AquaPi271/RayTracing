#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "rtweekend.h"

class material
{
  public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
  public:
    // enum class scatter_mode_enum
    // {
    //     UNIT_VECTOR,
    //     HEMISPHERE
    // };
    lambertian(const color &a) : albedo{a} {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered /*, scatter_mode_enum scatter_mode_in = scatter_mode_enum::HEMISPHERE */) const override
    {
        vec3 scatter_direction{0, 0, 0};
        // if (scatter_mode == scatter_mode_enum::HEMISPHERE)
        // {
        //     scatter_direction = random_in_hemisphere(rec.normal);
        // }
        // else
        // {
        //     // Was original code.
        //     scatter_direction = rec.normal + random_unit_vector();
        // }

        scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered   = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

    color albedo{0, 0, 0};
    // scatter_mode_enum scatter_mode = scatter_mode_in;
};

class metal : public material
{
  public:
    metal(const color &a, double f) : albedo{a}, fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered      = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation    = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    color  albedo{0, 0, 0};
    double fuzz = 0.0;
};

#endif