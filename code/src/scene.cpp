#include "scene.hpp"
#include <iostream>
#include <random> // for std::random_device and std::mt19937
#include <ctime>  // for std::time
#include <cstdlib> // for std::rand and std::srand

using namespace cgp;

void deform_terrain(mesh& m)
{

	vec2 p_i[54] = { {-10,-10}, {5,5}, {-3,4}, {6,4}, {117,471}, {19,-120}, {311,102}, {-301,314}, {-178,446}, {-17,330}, {-356,-143}, {143,481}, {-337,-175}, {-160,295}, {-10,-184}, {295,126}, {271,-478}, {-297,392}, {-284,-262}, {132,389}, {-412,366}, {-229,197}, {28,-189}, {353,-371}, {-20,424}, {-366,-74}, {-8,354}, {229,466}, {-40,324}, {255,-187}, {130,308}, {450,-302}, {124,-27}, {-209,-468}, {467,230}, {-66,-167}, {-174,229}, {-449,419}, {224,-164}, {406,242}, {337,-423}, {175,-43}, {401,-347}, {226,-202}, {288,283}, {181,-175}, {67,185}, {-1,48}, {-450,-315}, {140,-416}, {-384,-350}, {142,-223}, {92,299}, {-145,-371} };
	float h_i[54] = {3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f, 2.0f, 3.0f, -1.5f, 1.0f};
    float sigma_i[54] = {10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3, 4, 4, 10, 3};

	// Set the terrain to have a gaussian shape
	for (int k = 0; k < m.position.size(); ++k)
	{
		vec3& p = m.position[k];
		float d2 = p.x*p.x + p.y * p.y;
		//float z = exp(-d2 / 4)-1;

		float z = 0;
		for(int i=0; i<54; i++){
			float d_i = norm(vec2(p.x, p.y) - p_i[i]) / sigma_i[i];

			float z_i = h_i[i] * std::exp(-d_i * d_i);

			z += z_i;
		} 
		z = z + noise_perlin({ p.x,p.y });

		p = { p.x, p.y, z };
	}

	m.normal_update();
}

// Function to generate random coordinates
void generateRandomCoordinates(int& x_rand, int& y_rand) {
    // Use std::random_device to obtain a seed for the random number engine
    std::random_device rd;

    // Use the current time as a seed for the random number engine
    std::mt19937 gen(rd());

    // Define the distribution for random coordinates (-500 to 500)
    std::uniform_int_distribution<> distrib(-500, 500);

    // Generate random x and y coordinates
    x_rand = distrib(gen);
    y_rand = distrib(gen);
}

// This function is called only once at the beginning of the program
// This function can contain any complex operation that can be pre-computed once
void scene_structure::initialize()
{
	std::cout << "Start function scene_structure::initialize()" << std::endl;

	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window); 
	camera_control.set_rotation_axis_z(); // camera rotates around z-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{ 5.0f, -4.0f, 3.5f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,0,1} /* direction of the "up" vector */);

	// Display general information
	display_info();
	// Create the global (x,y,z) frame
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// Remove warnings for unset uniforms
	cgp_warning::max_warning = 0;

	opengl_shader_structure shader_standard;
	shader_standard.load(
		project::path + "shaders/mesh/mesh.vert.glsl",
		project::path + "shaders/mesh/mesh.frag.glsl");


	// Load the custom shader
	opengl_shader_structure shader_custom;
	shader_custom.load(
		project::path + "shaders/shading_custom/shading_custom.vert.glsl",
		project::path + "shaders/shading_custom/shading_custom.frag.glsl");


	// Create the shapes seen in the 3D scene
	// ********************************************** //

	
	float L = 1000.0f;
	mesh terrain_mesh = mesh_primitive_grid({ -L,-L,0 }, { L,-L,0 }, { L,L,0 }, { -L,L,0 }, 100, 100);
	deform_terrain(terrain_mesh);
	terrain.initialize_data_on_gpu(terrain_mesh);
	terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/sand.jpg");

	mesh pyramid_mesh = create_pyramid_mesh(10, 10, 0); pyramid_mesh.translate({10, 10, 0});
	mesh sphere_mesh = mesh_primitive_sphere(100.0f, {0,0,300}, 40, 20);

	pyramid.initialize_data_on_gpu(pyramid_mesh);
	pyramid.model.scaling = 10;
	pyramid.model.translation = {-350, -100, 0};
	pyramid.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/pyramid.jpg", GL_REPEAT, GL_REPEAT);

	camel.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/ar0camel.obj"));
	camel.model.scaling = 1;
	camel.model.rotation = rotation_axis_angle({1,0,0}, Pi/2.0f);
	camel.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/camleuvmap.png");

	sphinx.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/BMSudanSandstoneSphinx02.obj"));
	sphinx.model.scaling = 8;
	sphinx.model.rotation = rotation_axis_angle({1,0,0}, Pi/2.0f);
	sphinx.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/BMSudanSandstoneSphinx01.jpg");

	tree.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/palm_tree/palm_tree.obj"));
	tree.model.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);
	tree.model.scaling = 3;
	tree.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/palm_tree/palm_tree.jpg", GL_REPEAT, GL_REPEAT);

	for(int i=0; i<100; i++){
		// Generate random tree coordinates
		int x_rand, y_rand;
        generateRandomCoordinates(x_rand, y_rand);
		x_rand_trees.push_back(x_rand);
		y_rand_trees.push_back(y_rand);
	}

	sphere_light.initialize_data_on_gpu(sphere_mesh);
	
	pyramid.shader = shader_standard;
	camel.shader = shader_standard;
	terrain.shader = shader_standard;
	sphinx.shader = shader_standard;

    SDL_Init(SDL_INIT_AUDIO); // Initialize SDL
    SDL_LoadWAV("../sounds/uncharted.wav", &wavSpec, &wavBuffer, &wavLength); //  load .wav file
    deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0); // open audio device
    SDL_QueueAudio(deviceId, wavBuffer, wavLength); // queue file for playback
    SDL_PauseAudioDevice(deviceId, 0); // play audio

}


// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation defined there. This function is mostly used to call the draw() functions on pre-existing data.
void scene_structure::display_frame()
{

	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();

	// Update time
	timer.update();

	// conditional display of the global frame (set via the GUI)
	if (gui.display_frame)
		draw(global_frame, environment);
	

	// Draw all the shapes
	draw(terrain, environment);
	draw(pyramid, environment);
	sphinx.model.translation = { -60, 50, 0};
	draw(sphinx, environment);
	sphinx.model.translation = { -60, -50, 0};
	draw(sphinx, environment);

	for(int i=0; i<10; i++){
		camel.model.translation = { -10 + 10*(i/2), -50 + 10*(i%2), 0};
		draw(camel, environment);

		camel.model.translation = { -10 + 10*(i/2), +50 + 10*(i%2), 0};
		draw(camel, environment);
	}

	for(int i=0; i<100; i++){
		tree.model.translation = {x_rand_trees[i], y_rand_trees[i], 0};
		draw(tree, environment);
	}

	sphere_light.model.translation = gui.light_position;
	sphere_light.material.color = gui.light_color * 0.8f;
	sphere_light.material.phong.ambient = 1;
	sphere_light.material.phong.diffuse = 0;
	sphere_light.material.phong.specular = 0;
	draw(sphere_light, environment);

	if (gui.display_wireframe) {
		draw_wireframe(terrain, environment);
		draw_wireframe(pyramid, environment);
		draw_wireframe(camel, environment);
	}

	// Set additional uniform parameters to the shader
	environment.uniform_generic.uniform_float["ambiant"] = gui.ambiant;
	environment.uniform_generic.uniform_vec3["light_color"] = gui.light_color;
	environment.uniform_generic.uniform_vec3["light_position"] = gui.light_position;

	//add parameters for Phong Ilumination
	environment.uniform_generic.uniform_float["diffuse"] = gui.diffuse;
	environment.uniform_generic.uniform_float["specular"] = gui.specular;
	environment.uniform_generic.uniform_float["exp_specular"] = gui.exp_specular;
	environment.uniform_generic.uniform_vec3["fog_color"] = gui.fog_color;

	environment.background_color = gui.fog_color;
	
	// Wait for the sound to finish playing
    if (SDL_GetQueuedAudioSize(deviceId) == 0) {
        // and play it again!!!!
		SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		SDL_PauseAudioDevice(deviceId, 0);
    }
    
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}


void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;


	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Display here the information you would like to appear at the start of the program on the command line (file scene.cpp, function display_info())." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}