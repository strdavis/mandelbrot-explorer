// Implementation of state_manager class.

#define FMT_HEADER_ONLY

#include <iostream>
#include <sstream>
#include <fstream>
#include <fmt/core.h>
#include "../include/state_manager.hpp"

using std::cout;
using std::endl;
using std::istringstream;
using std::fstream;
using std::ios;
using std::find;
using fmt::format;

// Constructor.
// Initializes explorer state to defaults.
state_manager::state_manager()
{
	state.window_width = WINDOW_WIDTH_DEFAULT;
	state.window_height = WINDOW_HEIGHT_DEFAULT;
	state.mouse_down = false;
	state.x = X_INITIAL_DEFAULT;
	state.y = Y_INITIAL_DEFAULT;
	state.x_scale = SCALE_INITIAL_DEFAULT;
	state.y_scale = SCALE_INITIAL_DEFAULT;
	state.max_it = MAX_ITERATIONS_DEFAULT;
	state.x_mouse = 0;
	state.y_mouse = 0;
	state.colour = COLOUR_PALETTE_DEFAULT;
	state.precision = PRECISION_DEFAULT;
	state.display_text = true;
	state.text_colour_r = TEXT_COLOUR_R_DEFAULT;
	state.text_colour_g = TEXT_COLOUR_G_DEFAULT;
	state.text_colour_b = TEXT_COLOUR_B_DEFAULT;
}

// For each line of form "<option>,<value>" in file specified by filename,
// add elements <option> and <value> sequentially to options vector.
bool state_manager::load_options(string filename, vector<string>* options)
{
	fstream state_file;
	state_file.open(filename, ios::in);
	if(state_file.is_open())
	{
		string line;
		string delimiter = ",";
		size_t pos = 0;
		while(getline(state_file, line))
		{
			pos = line.find(delimiter);
			string option = line.substr(0, pos);
			line.erase(0, pos + delimiter.length());
			string value = line;
			options->push_back(option);
			options->push_back(value);	
		}	
				
		state_file.close();	
	}
	else
	{
		cout << "Could not open file." << endl;
		return false;
	}
	
	return true;
}

// For each state parameter necessary to define a loadable explorer state,
// write the line "<parameter>,<value>" to the file specified by filename.
bool state_manager::save_options(string filename)
{
	fstream save_file;
	save_file.open(filename, ios::out);
	if(save_file.is_open())
	{
		save_file << "-d," << state.window_width << "\n";
		save_file << "-x," << format("{}", state.x) << "\n";
		save_file << "-y," << format("{}", state.y) << "\n";
		save_file << "-s," << format("{}", state.x_scale) << "\n";
		save_file << "-i," << state.max_it << "\n";
		save_file << "-c," << state.colour << "\n";
		save_file << "-p," << state.precision << "\n";
		save_file << "-t," << format("{}", state.display_text) << "\n";

		save_file.close();
		cout << "State saved in file \"" << filename << "\"" << endl;
	}
	else
	{
		cout << "Could not open file. Aborting save." << endl;	
		return false;	
	}
	
	return true;
}

// Parse vector of options. Update state accordingly.
// Input vector "options" contains all CLI arguments.
bool state_manager::parse_options(vector<string> options)
{
	vector<string> valid_ops = {"-l", "-d", "-x", "-y", "-s", "-i", "-c", "-p", "-t"};
	
	// For each pair of sequential elements <option> and <value> in options,
	// set the explorer state accordingly.
	for (int i = 0; i < options.size(); i += 2)
	{	
		string option = options[i];
		if(find(valid_ops.begin(), valid_ops.end(), option) == valid_ops.end())
		{
			cout << "Unrecognized option \"" << option << "\"." << endl;
			return false;
		}	
		
		if (options.size() < i + 2)
		{
			cout << "No value provided for option " << option << "." << endl;
			return false;		
		}
		string value = options[i + 1];
		
		if (option == "-l")
		{
			if (options.size() > 2)
			{
				cout << "Option -l may not be used in conjunction with other options." << endl;		
				return false;	
			}
			
			if (!load_options(value, &options))
			{
				return false;
			}
		}	
		else if (option == "-d")
		{	
			istringstream iss(value);
			int dimension;
			if (iss >> dimension)
			{
				if (dimension < 250 or dimension > 1000)
				{
					cout << "Specified window dimension not in valid range." << endl;	
					return false;		
				}
				state.window_width = dimension;
				state.window_height = dimension;	
			}
			else
			{
				cout << "Specified window dimension cannot be converted to an int." << endl;
				return false;
			}
		}
		else if (option == "-x")
		{
			istringstream iss(value);
			double x_in;
			if (iss >> x_in)
			{
				if (x_in < -2.0  or x_in > 2.0)
				{
					cout << "Specified x value not in valid range." << endl;	
					return false;		
				}
				state.x = x_in;
			}
			else
			{
				cout << "Specified x value cannot be converted to a double." << endl;
				return false;
			}
		}
		else if (option == "-y")
		{	
			istringstream iss(value);
			double y_in;
			if (iss >> y_in)
			{
				if (y_in < -2.0  or y_in > 2.0)
				{
					cout << "Specified y value not in valid range." << endl;	
					return false;		
				}
				state.y = y_in;
			}
			else
			{
				cout << "Specified y value cannot be converted to a double." << endl;
				return false;
			}
		}
		else if (option == "-s")
		{
			istringstream iss(value);
			double scale_in;
			if (iss >> scale_in)
			{
				state.x_scale = scale_in;
				state.y_scale = scale_in;
			}
			else
			{
				cout << "Specified scale cannot be converted to a double." << endl;
				return false;
			}
		}
		else if (option == "-i")
		{		
			istringstream iss(value);
			int max_it;
			if (iss >> max_it)
			{
				if (max_it < 16 or max_it > 50000)
				{
					cout << "Specified maximum iteration count not in valid range." << endl;	
					return false;		
				}
				state.max_it = max_it;
			}
			else
			{
				cout << "Specified maximum iteration count cannot be converted to an int." << endl;
				return false;
			}
		}
		else if (option == "-c")
		{		
			if (value == "greyscale")
			{
				state.colour = "greyscale";
				state.text_colour_r = 1.0;
				state.text_colour_g = 0.0;
				state.text_colour_b = 0.0;
			}
			else if (value == "abyss")
			{
				state.colour = "abyss";
				state.text_colour_r = 1.0;
				state.text_colour_g = 1.0;
				state.text_colour_b = 1.0;
			}
			else if (value == "woodstock")
			{
				state.colour = "woodstock";
				state.text_colour_r = 1.0;
				state.text_colour_g = 1.0;
				state.text_colour_b = 1.0;
			}
			else if (value == "spectrum")
			{
				state.colour = "spectrum";
				state.text_colour_r = 1.0;
				state.text_colour_g = 1.0;
				state.text_colour_b = 1.0;
			}
			else
			{
				cout << "Specified colour palette does not exist." << endl;
				return false;
			}
		}
		else if (option == "-p")
		{	
			if (value == "float")
			{
				state.precision = "float";
			}
			else if (value == "double")
			{
				state.precision = "double";
			}
			else
			{
				cout << "Specified precision does not exist." << endl;
				return false;
			}
		}
		else if (option == "-t")
		{	
			if (value== "true")
			{
				state.display_text = true;
			}
			else if (value == "false")
			{
				state.display_text = false;
			}
			else
			{
				cout << "Specified value invalid for option -t." << endl;
				return false;
			}
		}
		else
		{
			cout << "Unrecognized option \"" << options[i] << "\"." << endl;
			return false;
		}
	}
	
	// After all options are processed, check if scale valid, since its validity depends on the precision.
	if (state.x_scale > 100.0)
	{
		cout << "Specified scale exceeds maximum value of 100.0." << endl;	
		return false;	
	}
	
	if (state.precision == "float" and state.x_scale < 0.00001)
	{
		cout << "Specified scale less than minimum value of 0.00001 for float precision." << endl;	
		return false;		
	}
	else if (state.precision == "double" and state.x_scale < 0.00000000000002)
	{
		cout << "Specified scale less than minimum value of 0.00000000000002 for double precision." << endl;	
		return false;		
	}
	
	return true;
}