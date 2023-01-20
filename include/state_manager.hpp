// Interface for state_manager class.
// This class is used to store and manage the state of the explorer.
// It provides functions for parsing command-line options and setting state accordingly,
// saving state to a file, and loading state from a file.

#include <stdlib.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class state_manager
{
	public:
		const int WINDOW_WIDTH_DEFAULT = 750;
		const int WINDOW_HEIGHT_DEFAULT = 750;
		const double X_INITIAL_DEFAULT = -0.5;
		const double Y_INITIAL_DEFAULT = 0.0;
		const double SCALE_INITIAL_DEFAULT = 1.5;
		const int MAX_ITERATIONS_DEFAULT = 250;
		const string COLOUR_PALETTE_DEFAULT = "greyscale";
		const string PRECISION_DEFAULT = "float";
		const float TEXT_COLOUR_R_DEFAULT = 1.0;
		const float TEXT_COLOUR_G_DEFAULT = 0.0;
		const float TEXT_COLOUR_B_DEFAULT = 0.0;
		
		struct current_state
		{
			int window_width;
			int window_height;
			bool mouse_down;
			double x;
			double y;
			double x_scale;
			double y_scale;
			int max_it;
			int x_mouse;
			int y_mouse;
			string colour;
			string precision;
			bool display_text;
			float text_colour_r;
			float text_colour_g;
			float text_colour_b;
		};
		
		state_manager();
		bool load_options(string filename, vector<string>* options);
		bool save_options(string filename);
		bool parse_options(vector<string> options);
		current_state state;
};