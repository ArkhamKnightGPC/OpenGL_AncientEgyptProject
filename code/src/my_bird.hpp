#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"

using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;
using cgp::timer_basic;

namespace cgp
{
    struct BirdMesh{
        mesh_drawable body;
        mesh_drawable head;
        mesh_drawable beak;
        mesh_drawable eye_left;
        mesh_drawable eye_right;
        mesh_drawable wing_left;
        mesh_drawable wing_right;
        mesh_drawable wing_son_left;
        mesh_drawable wing_son_right;

        timer_basic timer;
        cgp::hierarchy_mesh_drawable hierarchy;

        // Functions
        void initializeBirdMesh();
        void drawBirdMesh(environment_structure &environment);
        void translateBirdMesh(float dx, float dy, float dz);
    };
}
