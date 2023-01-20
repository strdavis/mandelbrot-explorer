// Interface for text_renderer class.
// This class is used to draw text.

#include <stdlib.h>
#include <string>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using std::string;
using std::map;
using std::pair;

class text_renderer
{
	public:
		text_renderer(string font_path, int font_size, float text_colour_r, float text_colour_g, float text_colour_b, int window_dim);
		void render_text(string text, float x, float y, float scale);
		void set_text_colour(float r, float g, float b);
		struct character
		{
			GLuint texture_id;
			int width;
			int height;
			int left;
			int top;
			long int advance;	
		};
		bool init_failed;
		
	private:
		map<char, character> characters_;
		GLuint VBO_;
		GLuint VAO_;
		GLuint vertex_shader_;
		GLuint fragment_shader_;
		GLuint shader_program_;
		int window_dim_;
		void init_font(string font, int font_size);
		void init_vbo_vao();
		void init_shader();
};