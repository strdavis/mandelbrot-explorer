// Implementation of mandelbrot_renderer class.

#include <iostream>
#include "../include/mandelbrot_renderer.hpp"

using std::cout;
using std::endl;

// Vertex shader.
const char *mandelbrot_vertex_shader_source = "#version 450 core\n"
	"in vec3 vertex_pos;\n"
	"out vec3 fragment_pos;\n"
	"void main()\n"
  "{\n"
   "  gl_Position = vec4(vertex_pos, 1.0);\n"
   "	fragment_pos = vertex_pos;\n"
   "}";

// Fragment shader.
// Precision: float.
// Color palette: greyscale.
const char *mandelbrot_fragment_shader_source_float_greyscale = "#version 450 core \n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"void main()\n"
	"{\n"
	"	vec2 c = vec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= vec2(float(x_scale), float(y_scale));\n"
	"	c += vec2(float(x), float(y));\n"
	"	vec2 z = c;\n"
	"	int i = 0;\n"
	"	float z_x_sqr = z.x * z.x;\n"
	"	float z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	float t = 1.0 - (float(i) / float(max_it));\n"
	"	fragment_colour = vec4(t, t, t, 1.0);\n"
	"}";
	
// Fragment shader.
// Precision: double.
// Color palette: greyscale.
const char *mandelbrot_fragment_shader_source_double_greyscale = "#version 450 core \n"
	"#extension GL_ARB_gpu_shader_fp64: enable\n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"void main()\n"
	"{\n"
	"	dvec2 c = dvec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= dvec2(x_scale, y_scale);\n"
	"	c += dvec2(x, y);\n"
	"	dvec2 z = c;\n"
	"	int i = 0;\n"
	"	double z_x_sqr = z.x * z.x;\n"
	"	double z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	double t = 1.0 - (double(i) / double(max_it));\n"
	"	fragment_colour = vec4(t, t, t, 1.0);\n"
	"}";
	
// Fragment shader.
// Precision: float.
// Color palette: abyss.
const char *mandelbrot_fragment_shader_source_float_abyss = "#version 450 core \n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"float lookup(float x, float c)\n"
	"{\n"
	"	return c * mod(x, 1.0 / c);\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	vec2 c = vec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= vec2(float(x_scale), float(y_scale));\n"
	"	c += vec2(float(x), float(y));\n"
	"	vec2 z = c;\n"
	"	int i = 0;\n"
	"	float z_x_sqr = z.x * z.x;\n"
	"	float z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	float t = float(i) / float(max_it);\n"
	"	fragment_colour = vec4(lookup(t, 2.0), lookup(t, 4.0), lookup(t, 8.0), 1.0);\n"
	"}";
	
// Fragment shader.
// Precision: double.
// Color palette: abyss.
const char *mandelbrot_fragment_shader_source_double_abyss = "#version 450 core \n"
	"#extension GL_ARB_gpu_shader_fp64: enable\n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"double lookup(double x, double c)\n"
	"{\n"
	"	return c * mod(x, 1.0 / c);\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	dvec2 c = dvec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= dvec2(x_scale, y_scale);\n"
	"	c += dvec2(x, y);\n"
	"	dvec2 z = c;\n"
	"	int i = 0;\n"
	"	double z_x_sqr = z.x * z.x;\n"
	"	double z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	double t = double(i) / double(max_it);\n"
	"	fragment_colour = vec4(lookup(t, 2.0), lookup(t, 4.0), lookup(t, 8.0), 1.0);\n"
	"}";
	
// Fragment shader.
// Precision: float.
// Color palette: woodstock.
const char *mandelbrot_fragment_shader_source_float_woodstock = "#version 450 core \n"
	"#define PI 3.1415926535897932384626433832795\n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"float lookup(float ratio, float phase)\n"
	"{\n"
	"	float temp = (cos(ratio * PI * 2.0 + phase) + 1.0) / 2.0;\n"
	"	return temp * sqrt(sqrt(ratio));\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	vec2 c = vec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= vec2(float(x_scale), float(y_scale));\n"
	"	c += vec2(float(x), float(y));\n"
	"	vec2 z = c;\n"
	"	int i = 0;\n"
	"	float z_x_sqr = z.x * z.x;\n"
	"	float z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	if (i == max_it) discard;\n"
	"	else\n"
	"	{\n"
	" 		float ratio = float(i) / float(max_it);\n"
	"		fragment_colour = vec4(lookup(ratio, 2.0 * PI / 3.0), lookup(ratio, 4.0 * PI / 3.0), lookup(ratio, 0.0), 1.0);\n"
	"	}\n"	
	"}";
	
// Fragment shader.
// Precision: double.
// Color palette: abyss.
const char *mandelbrot_fragment_shader_source_double_woodstock = "#version 450 core \n"
	"#define PI 3.1415926535897932384626433832795\n"
	"#extension GL_ARB_gpu_shader_fp64: enable\n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"double lookup(double ratio, double phase)\n"
	"{\n"
	"	float temp = (cos(float(ratio) * PI * 2.0 + float(phase)) + 1.0) / 2.0;\n"
	"	return double(temp * sqrt(sqrt(float(ratio))));\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	dvec2 c = dvec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= dvec2(x_scale, y_scale);\n"
	"	c += dvec2(x, y);\n"
	"	dvec2 z = c;\n"
	"	int i = 0;\n"
	"	double z_x_sqr = z.x * z.x;\n"
	"	double z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	if (i == max_it) discard;\n"
	"	else\n"
	"	{\n"
	" 		double ratio = double(i) / double(max_it);\n"
	"		fragment_colour = vec4(lookup(ratio, 2.0 * PI / 3.0), lookup(ratio, 4.0 * PI / 3.0), lookup(ratio, 0.0), 1.0);\n"
	"	}\n"	
	"}";

// Fragment shader.
// Precision: float.
// Color palette: spectrum.
const char *mandelbrot_fragment_shader_source_float_spectrum = "#version 450 core \n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"vec4 lookup(float ratio)\n"
	"{\n"
	"	float r = 0.0;\n"
	"	float g = 0.0;\n"
	"	float b = 0.0;\n"
	"	int step = int(ratio * 6.0);\n"
	"	float from_step = ratio * 6.0 - step;\n"
	"	float to_next_step = 1.0 - from_step;\n"
	"	if (step == 0)\n"
	"	{\n"
	"		r = 1.0;\n"
	"		g = from_step;\n"
	"		b = 0.0;\n"
	"	}\n"
	"	else if (step == 1)\n"
	"	{\n"
	"		r = to_next_step;\n"
	"		g = 1.0;\n"
	"		b = 0.0;\n"
	"	}\n"
	"	else if (step == 2)\n"
	"	{\n"
	"		r = 0.0;\n"
	"		g = 1.0;\n"
	"		b = from_step;\n"
	"	}\n"
	"	else if (step == 3)\n"
	"	{\n"
	"		r = 0.0;\n"
	"		g = to_next_step;\n"
	"		b = 1.0;\n"
	"	}\n"
	"	else if (step == 4)\n"
	"	{\n"
	"		r = from_step;\n"
	"		g = 0.0;\n"
	"		b = 1.0;\n"
	"	}\n"
	"	else\n"
	"	{\n"
	"		r = 1.0;\n"
	"		g = 0.0;\n"
	"		b = to_next_step;\n"
	"	}\n"
	"	return vec4(r, g, b, 1.0);\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	vec2 c = vec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= vec2(float(x_scale), float(y_scale));\n"
	"	c += vec2(float(x), float(y));\n"
	"	vec2 z = c;\n"
	"	int i = 0;\n"
	"	float z_x_sqr = z.x * z.x;\n"
	"	float z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	if (i == max_it) discard;\n"
	"	else\n"
	"	{\n"
	" 		float ratio = float(i) / float(max_it);\n"
	"		fragment_colour = lookup(ratio);\n"
	"	}\n"	
	"}";

// Fragment shader.
// Precision: float.
// Color palette: spectrum.
const char *mandelbrot_fragment_shader_source_double_spectrum = "#version 450 core \n"
	"in vec3 fragment_pos;\n"
	"out vec4 fragment_colour;\n"
	"uniform double x;\n"
	"uniform double y;\n"
	"uniform double x_scale;\n"
	"uniform double y_scale;\n"
	"uniform int max_it;\n"
	"vec4 lookup(float ratio)\n"
	"{\n"
	"	float r = 0.0;\n"
	"	float g = 0.0;\n"
	"	float b = 0.0;\n"
	"	int step = int(ratio * 6.0);\n"
	"	float from_step = ratio * 6.0 - step;\n"
	"	float to_next_step = 1.0 - from_step;\n"
	"	if (step == 0)\n"
	"	{\n"
	"		r = 1.0;\n"
	"		g = from_step;\n"
	"		b = 0.0;\n"
	"	}\n"
	"	else if (step == 1)\n"
	"	{\n"
	"		r = to_next_step;\n"
	"		g = 1.0;\n"
	"		b = 0.0;\n"
	"	}\n"
	"	else if (step == 2)\n"
	"	{\n"
	"		r = 0.0;\n"
	"		g = 1.0;\n"
	"		b = from_step;\n"
	"	}\n"
	"	else if (step == 3)\n"
	"	{\n"
	"		r = 0.0;\n"
	"		g = to_next_step;\n"
	"		b = 1.0;\n"
	"	}\n"
	"	else if (step == 4)\n"
	"	{\n"
	"		r = from_step;\n"
	"		g = 0.0;\n"
	"		b = 1.0;\n"
	"	}\n"
	"	else\n"
	"	{\n"
	"		r = 1.0;\n"
	"		g = 0.0;\n"
	"		b = to_next_step;\n"
	"	}\n"
	"	return vec4(r, g, b, 1.0);\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	dvec2 c = dvec2(fragment_pos.x, fragment_pos.y);\n"
	"	c *= dvec2(x_scale, y_scale);\n"
	"	c += dvec2(x, y);\n"
	"	dvec2 z = c;\n"
	"	int i = 0;\n"
	"	double z_x_sqr = z.x * z.x;\n"
	"	double z_y_sqr = z.y * z.y;\n"
	"	while (i < max_it && z_x_sqr + z_y_sqr < 4.0) {\n"
	"		z.y = 2.0 * z.x * z.y + c.y;\n"
	"		z.x = z_x_sqr - z_y_sqr + c.x;\n"
	"		z_x_sqr = z.x * z.x;\n"
	"		z_y_sqr = z.y * z.y;\n"
	"		i++;\n"
	"	}\n"
	"	if (i == max_it) discard;\n"
	"	else\n"
	"	{\n"
	" 		float ratio = float(i) / float(max_it);\n"
	"		fragment_colour = lookup(ratio);\n"
	"	}\n"	
	"}";

// Constructor.
mandelbrot_renderer::mandelbrot_renderer(string colour_palette, string precision, int max_it, double x, double y, double x_scale, double y_scale)
{
	// Initialize rendering components.
	init_failed = false;
	init_vbo_vao();
	init_shader(colour_palette, precision);
	
	// Set uniform variables.
	set_x(x);
	set_y(y);
	set_x_scale(x_scale);
	set_y_scale(y_scale);
	set_max_it(max_it);
}

// Draw Mandelbrot image.
// Image parameters are defined by current values of uniform variables in shader program.
void mandelbrot_renderer::render_mandelbrot()
{
	glUseProgram(shader_program_);
	glBindVertexArray(VAO_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// Set uniform variable x.
void mandelbrot_renderer::set_x(double x)
{
	GLint x_uniform_loc = glGetUniformLocation(shader_program_ , "x");
	glProgramUniform1d(shader_program_ , x_uniform_loc, x);
}

// Set uniform variable y.
void mandelbrot_renderer::set_y(double y)
{
	GLint y_uniform_loc = glGetUniformLocation(shader_program_ , "y");
	glProgramUniform1d(shader_program_ , y_uniform_loc, y);
}

// Set uniform variable x_scale.
void mandelbrot_renderer::set_x_scale(double x_scale)
{
	GLint x_scale_uniform_loc = glGetUniformLocation(shader_program_ , "x_scale");
	glProgramUniform1d(shader_program_ , x_scale_uniform_loc, x_scale);
}

// Set uniform variable y_scale.
void mandelbrot_renderer::set_y_scale(double y_scale)
{
	GLint y_scale_uniform_loc = glGetUniformLocation(shader_program_ , "y_scale");
	glProgramUniform1d(shader_program_ , y_scale_uniform_loc, y_scale);
}

// Set uniform variable max_it.
void mandelbrot_renderer::set_max_it(int max_it)
{
	GLint max_it_loc = glGetUniformLocation(shader_program_, "max_it");
	glProgramUniform1i(shader_program_, max_it_loc, max_it);
}

// Initialize vertex buffer object (VBO) and vertex array object (VAO).
void mandelbrot_renderer::init_vbo_vao()
{
	// Define vertices.
	// Vertices define two triangles which make up a quad to which images are drawn.
	double vertices[] = {
		-1.0, -1.0, 0.0,
		1.0, 1.0, 0.0,
		1.0, -1.0, 0.0,
		
		1.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		-1.0, 1.0, 0.0	
	};
	
	// Create VBO.
	glGenBuffers(1, &VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Create VAO.
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
}

// Initialize shader program using vertex and fragment shaders.
void mandelbrot_renderer::init_shader(string colour_palette, string precision)
{
	// Create vertex shader.
	vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_, 1, &mandelbrot_vertex_shader_source, NULL);
	glCompileShader(vertex_shader_);
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex_shader_, 512, NULL, info_log);
		cout << "Failed to compile Mandelbrot vertex shader" << info_log << endl;
		init_failed = true;
		return;
	}
	
	// Create fragment shader.
	// Select shader source according to specified precision, colour palette.
	fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
	if (colour_palette == "greyscale")
	{
		if (precision == "float")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_float_greyscale, NULL);
		if (precision == "double")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_double_greyscale, NULL);
	}
	else if (colour_palette == "abyss")
	{
		if (precision == "float")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_float_abyss, NULL);
		if (precision == "double")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_double_abyss, NULL);
	}
	else if (colour_palette == "woodstock")
	{
		if (precision == "float")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_float_woodstock, NULL);
		if (precision == "double")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_double_woodstock, NULL);	
	}
	else if (colour_palette == "spectrum")
	{
		if (precision == "float")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_float_spectrum, NULL);
		if (precision == "double")
			glShaderSource(fragment_shader_, 1, &mandelbrot_fragment_shader_source_double_spectrum, NULL);	
	}
	glCompileShader(fragment_shader_);
	glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment_shader_, 512, NULL, info_log);
		cout << "Failed to compile Mandelbrot fragment shader" << info_log << endl;
		init_failed = true;
		return;
	}
	
	// Create shader program.
	shader_program_ = glCreateProgram();
	glAttachShader(shader_program_, vertex_shader_);
	glAttachShader(shader_program_, fragment_shader_);
	glLinkProgram(shader_program_);
	glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program_, 512, NULL, info_log);
		std::cout << "Failed to link Mandelbrot shader program" << info_log << std::endl;
		init_failed = true;
		return;
	}
	
	// Delete the shaders. They are no longer needed.
	glDeleteShader(vertex_shader_);
	glDeleteShader(fragment_shader_);
}
		
		
		
		
		
		