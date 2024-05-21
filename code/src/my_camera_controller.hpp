#pragma once

#include <GLFW/glfw3.h>

#include "cgp/14_window/window.hpp"
#include "cgp/15_input_devices/input_devices.hpp"
#include "cgp/10_camera_model/camera_model.hpp"

namespace cgp
{
	//   The camera controller is set to provide a mapping between mouse/keyboard action onto the camera of the scene
	//   This mapping is set with the action functions:
	//     - action_mouse_move - called when the mouse is moved
	//     - action_keyboard - called when a keyboard touch is pressed/released
	//     - action_mouse_click - called when a mouse button is pressed/released
	//     - idle_frame - called at any frame (ex. for camera animation)
	//   This class and these functions can be specialized in derived controller class for specific behavior.
	//
	//  Technical details:
	//    The camera controller has access to the global state of the user Inputs (keyboard/mouse), and to the Window via pointers.
	//    The controller doesn't own the camera matrix but only modifies the one passed as parameter in the action functions.
	//    Specialized camera controller can own a model of camera allowing to store intermediate states for internal computation.
	struct my_camera_controller
	{
		// Allow to activate/deactivate the camera
		bool is_active = true;

		// Assign the pointers of inputs and window to the global state
		// This method must be called at the initialization before any other action handling
		void initialize(input_devices& inputs, window_structure& window);

		void update(mat4& camera_matrix_view);
		void action_mouse_click(mat4&) {};

		// Pointers to the global state of the inputs (keyboard, mouse, etc)
		input_devices* inputs = nullptr;
		// Pointer to the global state of the window
		window_structure* window = nullptr;

		camera_first_person camera_model;

		float speed = 0.05f; // default speed of the fly mode
		float roll = 0.8f;   // roll magnitude
		float pitch = 0.5f;  //pitch magnitude

		float speed_increase = 1.02f;
		float speed_max = 0.5f;
		float speed_min = 0.0001f;

		void action_mouse_move(mat4& camera_matrix_view);
		void idle_frame(mat4& camera_matrix_view);

		// Key 'C' (in capital) to capture the cursor
		void action_keyboard(mat4& camera_matrix_view);

		std::string doc_usage() const;

	protected:
		// true = cursor captured (/infinite motion), false = cursor free
		bool is_cursor_trapped = false; 
	};
}