// Interface for mandelbrot_renderer class.
// This class is used to draw images of the Mandelbrot set.

#include <stdlib.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::string;

class mandelbrot_renderer
{
	public:
		mandelbrot_renderer(string colour_palette, string precision, int max_it, double x, double y, double x_scale, double y_scale);
		void render_mandelbrot();
		void set_x(double x);
		void set_y(double y);
		void set_x_scale(double x_scale);
		void set_y_scale(double y_scale);
		void set_max_it(int max_it);
		bool init_failed;
		
	private:
		GLuint VBO_;
		GLuint VAO_;
		GLuint vertex_shader_;
		GLuint fragment_shader_;
		GLuint shader_program_;
		void init_vbo_vao();
		void init_shader(string colour_palette, string precision);
};