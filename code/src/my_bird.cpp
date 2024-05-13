#include "my_bird.hpp"

namespace cgp
{
    void BirdMesh::initializeBirdMesh(){

        body.initialize_data_on_gpu(mesh_primitive_sphere(1.0)); body.model.scaling_xyz = vec3{2.0f, 0.7f, 1.0f};
        head.initialize_data_on_gpu(mesh_primitive_sphere(0.7));
        beak.initialize_data_on_gpu(mesh_primitive_cone(0.35, 0.35, {0,0,0}, {1,0,0}));
        eye_left.initialize_data_on_gpu(mesh_primitive_sphere(1.0)); eye_left.model.scaling_xyz = vec3{0.03, 0.1, 0.1};
        eye_right.initialize_data_on_gpu(mesh_primitive_sphere(1.0)); eye_right.model.scaling_xyz = vec3{0.03, 0.1, 0.1};
        wing_left.initialize_data_on_gpu(mesh_primitive_quadrangle({1,1,0}, {0,1,0}, {0,0,0}, {1,0,0})); wing_left.model.scaling_xyz = vec3{1.7, 2, 1.0};
        wing_right.initialize_data_on_gpu(mesh_primitive_quadrangle({1,1,0}, {0,1,0}, {0,0,0}, {1,0,0})); wing_right.model.scaling_xyz = vec3{1.7, 2, 1.0};
        wing_son_left.initialize_data_on_gpu(mesh_primitive_triangle({0,1,0}, {1,1,0}, {0,2,0})); wing_son_left.model.scaling_xyz = vec3{1.7, 2, 1.0};
        wing_son_right.initialize_data_on_gpu(mesh_primitive_triangle({0,1,0}, {1,1,0}, {0,2,0})); wing_son_right.model.scaling_xyz = vec3{1.7, 2, 1.0};

        vec3 white = { 1.0f, 1.0f, 1.0f };
        body.material.color = white;
        head.material.color = white;
        wing_left.material.color = white;
        wing_right.material.color = white;
        wing_son_left.material.color = white;
        wing_son_right.material.color = white;

        vec3 orange = { 1.0f, 0.5f,  0.0f };
        beak.material.color = orange;

        vec3 black = {0, 0, 0};
        eye_left.material.color = black;
        eye_right.material.color = black;

        hierarchy.add(body, "Body");
        hierarchy.add(head, "Head", "Body", {2.4, 0, 0});
        hierarchy.add(beak, "Beak", "Head", {0.55, 0, 0});
        hierarchy.add(eye_left, "Eye left", "Head", {0.6, -0.2, 0.3});
        hierarchy.add(eye_right, "Eye right", "Head", {0.6, 0.2, 0.3});
        hierarchy.add(wing_left, "Wing left", "Body", {-1.0f, -.5f, 0});
        hierarchy.add(wing_right, "Wing right", "Body", {-1.0f, .5f, 0});
        hierarchy.add(wing_son_left, "Wing son left", "Wing left", {0, 0, 0});
        hierarchy.add(wing_son_right, "Wing son right", "Wing right", {0, 0, 0});
    }

    void BirdMesh::drawBirdMesh(environment_structure &environment){
        // Update the current time
        timer.update();

        // Apply transformation to some elements of the hierarchy
        
        const float minAngle = -M_PI / 6;
        const float maxAngle = M_PI / 6;
        float scaled_time = 3*timer.t; // Initialize time to zero or use your time variable
        float angle = minAngle + (maxAngle - minAngle) * 0.5f * (1 + sin(scaled_time));

        hierarchy["Wing left"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, angle);
        hierarchy["Wing right"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, -M_PI - angle);

        float head_oscillation = 0.15 * sin(0.5*M_PI*scaled_time);

        hierarchy["Head"].transform_local.rotation = rotation_transform::from_axis_angle({0, 1, 0}, head_oscillation);

        // This function must be called before the drawing in order to propagate the deformations through the hierarchy
        hierarchy.update_local_to_global_coordinates();

        draw(hierarchy, environment);
    }

    void BirdMesh::translateBirdMesh(float dx, float dy, float dz){
        hierarchy["Body"].transform_local.translation = vec3{dx, dy, dz};
        hierarchy.update_local_to_global_coordinates();
    }
}
