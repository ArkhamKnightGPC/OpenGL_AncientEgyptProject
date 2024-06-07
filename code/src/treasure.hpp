#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"
#include <string>

using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;
using cgp::timer_basic;

class Treasure {
public:
    Treasure(std::string path_to_obj, std::string path_to_texture, float scaling_factor, vec3 treasure_position);

    ~Treasure();

    bool get_found();

    void drawSphere(environment_structure &environment);

    void pickTreasure(vec3 camera_position);

    void drawReward(environment_structure &environment, vec3 camera_position, mat3 camera_orientation);

private:
    vec3 position;
    bool found;
    int model_zoffset;
    int countdown_display;

    timer_basic timer;

    mesh_drawable glowing_sphere;
    mesh_drawable reward;
};
