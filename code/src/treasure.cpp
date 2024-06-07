#include "treasure.hpp"

using namespace cgp;

Treasure::Treasure(std::string path_to_obj, std::string path_to_texture, float scaling_factor, vec3 treasure_position){

    //Glowing sphere will indicate treasure's location
	mesh sphere_mesh = mesh_primitive_sphere(1.0f);
	glowing_sphere.initialize_data_on_gpu(sphere_mesh);

	glowing_sphere.model.translation = treasure_position;
	glowing_sphere.material.color = {1, 1, 1}; //sphere will appear white

    //when player finds the treasure, he will see the treasure itself for a brief period
    reward.initialize_data_on_gpu(mesh_load_file_obj(path_to_obj));
	reward.model.scaling = scaling_factor;
	reward.texture.load_and_initialize_texture_2d_on_gpu(path_to_texture);

    position = treasure_position;
    found = false;
    countdown_display = 150;

    model_zoffset = 5 ? (path_to_obj.compare("assets/treasures/coin.obj") == 0) : 0;
}

Treasure::~Treasure(){

}

bool Treasure::get_found(){
    return found;
}

void Treasure::drawSphere(environment_structure &environment){
    if(found == false){//we show sphere while treasure is not found
        draw(glowing_sphere, environment);
    }
}

void Treasure::pickTreasure(vec3 camera_position){
    double dist = pow((position[0] - camera_position[0]), 2) + pow((position[1] - camera_position[1]), 2) + pow((position[2] - camera_position[2]), 2);
    if(dist < 1000){//player is close enough to pick this treasure!!
        found = true;
    }
}

void Treasure::drawReward(environment_structure &environment, vec3 camera_position, mat3 camera_orientation){
    if(found && countdown_display > 0){
        countdown_display -= 1;

        //we will show the reward in front of the camera
        vec3 relative_position = camera_orientation*vec3(0, model_zoffset, -10);
        vec3 camera_vec = camera_position+relative_position;
        reward.model.translation = {camera_vec[0], camera_vec[1], camera_vec[2]};

        //androtate it so player can have a good look
        timer.update();
        const float minAngle = -M_PI / 6;
        const float maxAngle = M_PI / 6;
        float scaled_time = 3*timer.t;
        float angle = minAngle + (maxAngle - minAngle) * 0.5f * (1 + sin(scaled_time));
        reward.model.rotation = rotation_axis_angle({0,0,1}, angle);

        draw(reward, environment);
    }
}