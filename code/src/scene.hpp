#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "pyramid.hpp"
#include "my_bird.hpp"
#include "my_camera_controller.hpp"
#include "treasure.hpp"
#include <SDL.h>

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;
using cgp::timer_basic;

// Variables associated to the GUI (buttons, etc)
struct gui_parameters {
	bool display_frame = false;
	bool display_wireframe = false;

	vec3 light_color = { 1,1,1 };
	vec3 light_position = { -2, 2, 2 };

	float ambiant = 0.3f;
	//Phong parameters
	float diffuse = 0.8f;
	float specular = 0.25f;
	float exp_specular = 1;
	vec3 fog_color = {219.0/256.0, 187.0/256.0, 116.0/256.0};
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	my_camera_controller camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	timer_basic timer;

	mesh_drawable terrain;
	mesh_drawable khufu_pyramid;
    mesh_drawable khafre_pyramid;
    mesh_drawable menkaure_pyramid;
    mesh_drawable queen_pyramids;
    mesh_drawable camel;
	mesh_drawable sphinx;
	mesh_drawable tree;
	std::vector<Treasure> treasures;
	BirdMesh bird_mesh;
	std::vector<int> x_rand_trees, y_rand_trees;
	std::map<std::pair<int,int>, float> height_map;

	void deform_terrain(mesh& m);

	mesh_drawable sphere_light;

	SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;
	SDL_AudioDeviceID deviceId;

	// The entire hierarchy
	cgp::hierarchy_mesh_drawable hierarchy;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

	void display_info();

};





