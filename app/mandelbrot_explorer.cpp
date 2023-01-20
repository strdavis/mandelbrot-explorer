// Mandelbrot set explorer.

#define FMT_HEADER_ONLY

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include "../include/mandelbrot_renderer.hpp"
#include "../include/text_renderer.hpp"
#include "../include/state_manager.hpp"

using std::cout;
using std::endl;
using std::strcmp;
using std::istringstream;
using std::string;
using std::fstream;
using std::ios;
using std::cin;
using std::vector;
using std::map;
using std::pair;
using fmt::format;
using current_state = state_manager::current_state;

// Defines.
#define FONT_PATH "/usr/share/fonts/gnu-free/FreeSans.ttf"
#define FONT_SIZE 48

// Renderers.
mandelbrot_renderer* mr;
text_renderer* tr;

// Forward declarations.
void error_callback(int error, const char* description);
void handle_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
static void handle_mouse_movement(GLFWwindow* window, double x_mouse_new, double y_mouse_new);
void handle_mouse_buttons(GLFWwindow* window, int button, int action, int mods);
void handle_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset);
void save_options(string filename, current_state* state);


// Main.
int main(int argc, char** argv) {	
	// ****************************************************************
	// Initialize explorer state.

	// Initialize state manager with default state.
	state_manager* sm = new state_manager();
	current_state* state = &sm->state;
	
	// Modify initial state according to command line options.
	vector<string> options;
	for (int i = 1; i < argc; i++)
	{
		options.push_back(argv[i]);
	}
	if (!sm->parse_options(options))
	{
		delete sm;
		return EXIT_FAILURE;
	}

	// ****************************************************************
	// Initialize GLFW, GLEW, and OPENGL.

	// Set GLFW error callback function.
	glfwSetErrorCallback(error_callback);
		
	// Initialize GLFW.
	if (!glfwInit())
	{
		delete sm;
		return EXIT_FAILURE;	
	}
	
	// Create window.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(state->window_width, state->window_height, "Mandelbrot Explorer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		delete sm;
		return EXIT_FAILURE;	
	}
	
	// Initialize context.
	glfwMakeContextCurrent(window);
	
	// Set swap interval.
	glfwSwapInterval(1);
	
	// Initialize GLEW.
	glewExperimental = GL_TRUE;
	glewInit();
	
	// Set GLFW callback functions.
	glfwSetKeyCallback(window, handle_keyboard);
	glfwSetCursorPosCallback(window, handle_mouse_movement);
	glfwSetMouseButtonCallback(window, handle_mouse_buttons);
	glfwSetScrollCallback(window, handle_mouse_scroll);
	
	// Set GLFW window user pointer to the state manager.
	// Then state manager can be accessed in callback functions.
	glfwSetWindowUserPointer(window, sm);
	
	// Enable transparency (for text overlaid on Mandelbrot image).
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Print active OpenGL configuration.
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version: %s\n", version);
	int major;
	int minor;
	int rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("GLFW version: %d.%d.%d\n", major, minor, rev);	
	
	// ****************************************************************
	// Initialize renderers.
	mr = new mandelbrot_renderer(state->colour, state->precision, state->max_it, state->x, state->y, state->x_scale, state->y_scale);
	tr = new text_renderer(FONT_PATH, FONT_SIZE, state->text_colour_r, state->text_colour_g, state->text_colour_b, state->window_width);
	if (mr->init_failed or tr->init_failed)
	{
		glfwDestroyWindow(window); glfwTerminate();
		delete sm; delete mr; delete tr;
		return EXIT_FAILURE;	
	}	
	
	// ****************************************************************
	// Rendering loop.
	while(!glfwWindowShouldClose(window))
	{
		// Clear the drawing surface.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw Mandelbrot image.
		mr->render_mandelbrot();
		
		// Draw text showing current position and scale.
		if (state->display_text)
		{
			float text_scale = float(state->window_width) / float(sm->WINDOW_WIDTH_DEFAULT);
			tr->render_text("X: " + format("{}", state->x), -0.9, -0.7, text_scale);
			tr->render_text("Y: " + format("{}", state->y), -0.9, -0.8, text_scale);
			tr->render_text("S: " + format("{}", state->x_scale), -0.9, -0.9, text_scale);
		}
		
		// Update the display.
		glfwSwapBuffers(window);
		
		// Wait for input.
		glfwWaitEvents();
	}
	
	// Cleanup and exit.
	glfwDestroyWindow(window); glfwTerminate();
	delete mr; delete tr; delete sm;
	return EXIT_SUCCESS;
}

// ****************************************************************
// GLFW callback functions.

void error_callback(int error, const char* description)
{
    cout << "Error: " << description << endl;
}

void handle_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// If "Q" key pressed, quit.
	if (key == GLFW_KEY_Q and action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	
	// If "S" pressed, prompt for filename and save state.
	if (key == GLFW_KEY_S and action == GLFW_PRESS)
	{
		string filename;
		cout << "Enter name for save file:";
		cin >> filename;
		
		if (filename == "")
		{
			cout << "Filename must not be empty. Aborting save." << endl;	
			return;	
		}
		
		state_manager* sm = static_cast<state_manager*>(glfwGetWindowUserPointer(window));
		sm->save_options(filename);
	}
}

static void handle_mouse_movement(GLFWwindow* window, double x_mouse_new, double y_mouse_new)
{
	state_manager* sm = static_cast<state_manager*>(glfwGetWindowUserPointer(window));
	current_state* state = &sm->state;
	
	// If mouse is currently pressed.
	if (state->mouse_down)
	{	
		// Get difference of current position and previous position.
		int x_diff = x_mouse_new - state->x_mouse;
		int y_diff = y_mouse_new - state->y_mouse;

		// Use difference to update state with new position.
		state->x -= double(x_diff) * state->x_scale / state->window_width;
		state->y += double(y_diff) * state->y_scale / state->window_height;
		
		// Update Mandelbrot renderer with new position.
		mr->set_x(state->x);
		mr->set_y(state->y);
	}
	
	// Update state with new mouse position.
	state->x_mouse = x_mouse_new;
	state->y_mouse = y_mouse_new;
}

void handle_mouse_buttons(GLFWwindow* window, int button, int action, int mods)
{
	state_manager* sm = static_cast<state_manager*>(glfwGetWindowUserPointer(window));
	current_state* state = &sm->state;
	
	// If left mouse button pressed, set flag.
	if (button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS)
	{
		state->mouse_down = true;
	}
	
	// If left mouse button released, clear flag.
	if (button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_RELEASE)
	{
		state->mouse_down = false;
	}
}

void handle_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	state_manager* sm = static_cast<state_manager*>(glfwGetWindowUserPointer(window));
	current_state* state = &sm->state;
	
	// If scroll wheel up.
	if (yoffset > 0)
	{
		// Modify state with new scale (zoom in).
		if ((state->precision == "float" and state->x_scale > 0.00001) or (state->precision == "double" and state->x_scale > 0.00000000000002))
		{
			state->x_scale *= 0.9;
			state->y_scale *= 0.9;
		}
	}
	// Else if scroll wheel down.
	else
	{
		// Modify state with new scale (zoom out).
		if (state->x_scale < 100.0)
		{
			state->x_scale *= 1.1;
			state->y_scale *= 1.1;
		}
	}
	
	// Update Mandelbrot renderer with new scale.
	mr->set_x_scale(state->x_scale);
	mr->set_y_scale(state->y_scale);
}







