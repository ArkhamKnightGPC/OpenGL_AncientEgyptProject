#include "pyramid.hpp"
#include <math.h>       /* sin, cos */

# define M_PI           3.14159265358979323846  /* pi */

using namespace cgp;
mesh create_pyramid_mesh(float radius, float height, float z_offset)
{
    mesh m;
    vec3 p1 = {radius, -radius, z_offset};
    vec3 p2 = {radius, radius, z_offset};
    vec3 p3 = {-radius, radius, z_offset};
    vec3 p4 = {-radius, -radius, z_offset};
    vec3 apex = {0,0,height+z_offset};

    m.push_back(mesh_primitive_triangle(p1,p2,apex));
    m.push_back(mesh_primitive_triangle(p2,p3,apex));
    m.push_back(mesh_primitive_triangle(p3,p4,apex));
    m.push_back(mesh_primitive_triangle(p4,p1,apex));

    m.fill_empty_field();
    return m;
}