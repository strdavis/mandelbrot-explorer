// Implementation of text_renderer class.
// Technique for loading and using characters with FreeType adapted from:
// https://learnopengl.com/In-Practice/Text-Rendering

#include <iostream>
#include "../include/text_renderer.hpp"

using std::cout;
using std::endl;

// Text vertex shader.
const char *text_vertex_shader_source = "#version 450 core\n"
	"layout(location = 2) in vec2 vertex_pos;\n"
	"layout(location = 3) in vec2 texture_pos_in;\n"
	"out vec2 texture_pos;\n"
	"void main()\n"
   "{\n"
   "  gl_Position = vec4(vertex_pos, 0.0, 1.0);\n"
   "	texture_pos = texture_pos_in;\n"
   "}";
   
// Text fragment shader. 
const char *text_fragment_shader_source = "#version 450 core\n"
	"out vec4 frag_colour;\n"
	"in vec2 texture_pos;\n"
	"uniform sampler2D source_texture;\n"
	"uniform vec3 text_colour;\n"
	"void main()\n"
	"{\n"
	"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(source_texture, texture_pos).r);\n"
	"	frag_colour = vec4(text_colour, 1.0) * sampled;\n"
	"}";

// Constructor.
text_renderer::text_renderer(string font, int font_size, float text_colour_r, float text_colour_g, float text_colour_b, int window_dim)
{	
	// Initialize rendering components.
	init_failed = false;
	init_font(font, font_size);
	init_vbo_vao();
	init_shader();
	
	// Prepare uniform variables.
	set_text_colour(text_colour_r, text_colour_g, text_colour_b);
							 
	// Record window dimension.
	window_dim_ = window_dim;
}

// Draw text.
// Text is drawn at position (x, y),
// where x and y are each in range [-1, 1].
// Text is scaled from its original size by parameter "scale".
// Technique for rendering strings adapted from:
// https://learnopengl.com/In-Practice/Text-Rendering
void text_renderer::render_text(string text, float x, float y, float scale)
{
	glUseProgram(shader_program_);
	
	// For each character in string.
	string::const_iterator c = text.begin();
	for (c = text.begin(); c != text.end(); c++)
	{
		// Obtain corresponding texture from the map of stored character textures.
		character ch = characters_[*c];
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		glBindVertexArray(VAO_);
		glActiveTexture(GL_TEXTURE0);
		
		// Define the quad to which the character will be drawn.
		float xpos = x + ((ch.left * scale) / window_dim_);
		float ypos = y - (((ch.height - ch.top) * scale) / window_dim_);
		float w = (ch.width * scale) / window_dim_;
		float h = (ch.height * scale) / window_dim_;
		float text_vertices[] = {
			xpos + w, 	ypos + h,	1.0f, 0.0f,
			xpos + w, 	ypos,			1.0f, 1.0f,
			xpos, 		ypos,			0.0f, 1.0f,
			xpos + w, 	ypos + h,	1.0f, 0.0f,
			xpos, 		ypos,			0.0f, 1.0f,
			xpos, 		ypos + h,	0.0f, 0.0f,
		};
		
		// Draw the character.
		glBindBuffer(GL_ARRAY_BUFFER, VBO_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(text_vertices), text_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Advance the x-position for next character.
		x += ((ch.advance >> 6) * scale) / window_dim_;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Initialize font using FreeType.
// Font is loaded into a map of character textures for access during text rendering.
// Technique for loading font into textures adapted from:
// https://learnopengl.com/In-Practice/Text-Rendering
void text_renderer::init_font(string font_path, int font_size)
{
	// Initialize freetype.
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		cout << "Failed to initialize Freetype." << endl;
		init_failed = true;
		return;
	}
	
	// Initialize font.
	FT_Face face;
	if (FT_New_Face(ft, font_path.c_str(), 0, &face))
	{
		cout << "Failed to load font." << endl;
		FT_Done_FreeType(ft);
		init_failed = true;
		return;
	}
	
	// Set font size.
	FT_Set_Pixel_Sizes(face, 0, font_size);
	
	// Initialize map of character textures.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++)
	{
		// For each ASCII character from 0 - 127,
		// load character from font.
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			cout << "Failed to load character." << endl;
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
			init_failed = true;
			return;
		}	
		
		// Create character texture.
		GLuint char_texture = 0;
		glGenTextures(1, &char_texture);
		glBindTexture(GL_TEXTURE_2D, char_texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		character new_character = {
			char_texture,
			(int) face->glyph->bitmap.width,
			(int) face->glyph->bitmap.rows,
			face->glyph->bitmap_left,
			face->glyph->bitmap_top,
			face->glyph->advance.x		
		};
		
		// Store character in map.
		characters_.insert(pair<char, character>(c, new_character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

// Set uniform variables for text colour.
void text_renderer::set_text_colour(float r, float g, float b)
{
	GLint text_colour_uniform_loc = glGetUniformLocation(shader_program_, "text_colour");
	glProgramUniform3f(shader_program_, text_colour_uniform_loc, r, g, b);
}

void text_renderer::init_vbo_vao()
{
	// Create VBO.
	glGenBuffers(1, &VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	
	// Create VAO.
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(3);
}

void text_renderer::init_shader()
{
	// Create text vertex shader.
	vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_, 1, &text_vertex_shader_source, NULL);
	glCompileShader(vertex_shader_);
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex_shader_, 512, NULL, info_log);
		cout << "Failed to compile text vertex shader" << info_log << endl;
		init_failed = true;
		return;
	}
	
	// Create text fragment shader.
	fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_, 1, &text_fragment_shader_source, NULL);
	glCompileShader(fragment_shader_);
	glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment_shader_, 512, NULL, info_log);
		cout << "Failed to compile text fragment shader" << info_log << endl;
		init_failed = true;
		return;
	}
	
	// Create text shader program.
	shader_program_ = glCreateProgram();
	glAttachShader(shader_program_, vertex_shader_);
	glAttachShader(shader_program_, fragment_shader_);
	glLinkProgram(shader_program_);
	glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program_, 512, NULL, info_log);
		cout << "Failed to link text shader program" << info_log << endl;
		init_failed = true;
		return;
	}
	
	// Delete the shaders. They are no longer needed.
	glDeleteShader(vertex_shader_);
	glDeleteShader(fragment_shader_);
}






