#include "cgp/01_base/base.hpp"
#include "my_camera_controller.hpp"

#include <iostream>

namespace cgp
{
	void my_camera_controller::initialize(input_devices& inputs_param, window_structure& window_param)
	{
		inputs = &inputs_param;
		window = &window_param;
	}

	void my_camera_controller::look_at(vec3 const& eye, vec3 const& center, vec3 const& up)
	{
		camera_model.look_at(eye, center, up);
	}

	void my_camera_controller::update(mat4& camera_matrix_view) {
		camera_matrix_view = camera_model.matrix_view();
	}

	void my_camera_controller::action_keyboard(mat4&)
	{
		if ( inputs->keyboard.last_action.is_pressed(GLFW_KEY_C) && inputs->keyboard.shift ) {
			is_cursor_trapped = !is_cursor_trapped;
			if (is_cursor_trapped)
				glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		// Escape also gives back the cursor
		if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_ESCAPE)) {
			is_cursor_trapped = false;
			glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void my_camera_controller::idle_frame(mat4& camera_matrix_view)
	{
		// Preconditions
		assert_cgp_no_msg(inputs != nullptr);
		assert_cgp_no_msg(window != nullptr);
		if (!is_active) return;

		if (inputs->keyboard.is_pressed(GLFW_KEY_KP_ADD) || inputs->keyboard.is_pressed(GLFW_KEY_R))
			speed = std::min(speed * speed_increase, speed_max);
		if (inputs->keyboard.is_pressed(GLFW_KEY_KP_SUBTRACT) || inputs->keyboard.is_pressed(GLFW_KEY_F))
			speed = std::max(speed / speed_increase, speed_min);

		float const magnitude = 10*(inputs->time_interval)*speed;

		//WASDQE used for camera rotations
		if (inputs->keyboard.is_pressed(GLFW_KEY_W))
			camera_model.manipulator_rotate_roll_pitch_yaw(0, pitch * magnitude, 0);
		if (inputs->keyboard.is_pressed(GLFW_KEY_S))
			camera_model.manipulator_rotate_roll_pitch_yaw(0, -pitch * magnitude, 0);

		if (inputs->keyboard.is_pressed(GLFW_KEY_A))
            camera_model.manipulator_rotate_roll_pitch_yaw(0, 0.0f, roll * magnitude);
		if (inputs->keyboard.is_pressed(GLFW_KEY_D))
			camera_model.manipulator_rotate_roll_pitch_yaw(0, 0.0, -roll * magnitude);

        if (inputs->keyboard.is_pressed(GLFW_KEY_Q))
            camera_model.manipulator_rotate_roll_pitch_yaw(roll * magnitude, 0.0f, 0.0f);
        if (inputs->keyboard.is_pressed(GLFW_KEY_E))
            camera_model.manipulator_rotate_roll_pitch_yaw(-roll * magnitude, 0.0f, 0.0f);

		//IJKL used for camera translations
		if (inputs->keyboard.is_pressed(GLFW_KEY_I))
			camera_model.manipulator_translate_front(100*magnitude); // forward
		if (inputs->keyboard.is_pressed(GLFW_KEY_K))
			camera_model.manipulator_translate_front(-100*magnitude); // backward
		
		if (inputs->keyboard.is_pressed(GLFW_KEY_L))
			camera_model.manipulator_translate_in_plane({-100*magnitude ,0 }); // right
		if (inputs->keyboard.is_pressed(GLFW_KEY_J))
			camera_model.manipulator_translate_in_plane({100*magnitude ,0 }); // left

		update(camera_matrix_view);
	}

	std::string my_camera_controller::doc_usage() const
	{
		std::string doc;
		doc += "First person 'fly mode': Continuously move forward, while being able to rotate (pull up/down and roll).\n";
		doc += "Control: \n";
		doc += "   - Mouse left click + drag: Pull up/down, roll right/left.\n";
		doc += "   - Key arrows or WASD/ZQSD for translating forward/backward/left/right.\n";
		doc += "   - Key +/- or R/F for speeding up/down.\n";
		doc += "   - Press \"Shift+C\" (Maj C)  to enter Mouse-Captured Mode: Enables infinite mouse tracking (quit the mode with \"Shift+C\" again or Esc).";

		return doc;
	}
	
}