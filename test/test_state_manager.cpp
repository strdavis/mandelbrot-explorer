// Code used to test state_manager class.

#define CATCH_CONFIG_MAIN

#include "../include/state_manager.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using std::cout;
using std::endl;

TEST_CASE("Test header.", "[sm]")
{
	cout << "*************************************" << endl;
	cout << "Output of state_manager during tests:" << endl;
	cout << "*************************************" << endl;
}

TEST_CASE("Default state.", "[sm]")
{
	state_manager* sm = new state_manager();
	CHECK(sm->state.window_width == 750);
	CHECK(sm->state.window_height == 750);
	CHECK(sm->state.mouse_down == false);
	CHECK(sm->state.x == -0.5);
	CHECK(sm->state.y == 0.0);
	CHECK(sm->state.x_scale == 1.5);
	CHECK(sm->state.y_scale == 1.5);
	CHECK(sm->state.max_it == 250);
	CHECK(sm->state.x_mouse == 0);
	CHECK(sm->state.y_mouse == 0);
	CHECK(sm->state.colour == "greyscale");
	CHECK(sm->state.precision == "float");
	CHECK(sm->state.display_text == true);
	CHECK(sm->state.text_colour_r == 1.0);
	CHECK(sm->state.text_colour_g == 0.0);
	CHECK(sm->state.text_colour_b == 0.0);
}

TEST_CASE("Default state, parse empty options vector.", "[sm]")
{
	vector<string> options;
	state_manager* sm = new state_manager();
	REQUIRE(sm->parse_options(options));
	CHECK(sm->state.window_width == 750);
	CHECK(sm->state.window_height == 750);
	CHECK(sm->state.mouse_down == false);
	CHECK(sm->state.x == -0.5);
	CHECK(sm->state.y == 0.0);
	CHECK(sm->state.x_scale == 1.5);
	CHECK(sm->state.y_scale == 1.5);
	CHECK(sm->state.max_it == 250);
	CHECK(sm->state.x_mouse == 0);
	CHECK(sm->state.y_mouse == 0);
	CHECK(sm->state.colour == "greyscale");
	CHECK(sm->state.precision == "float");
	CHECK(sm->state.display_text == true);
	CHECK(sm->state.text_colour_r == 1.0);
	CHECK(sm->state.text_colour_g == 0.0);
	CHECK(sm->state.text_colour_b == 0.0);
}

TEST_CASE("All CLI options specified except -l.", "[sm]")
{
	vector<string> options = {
		"-d","500",
		"-x","-1.5",
		"-y","0.5",
		"-s","0.1",
		"-i","100",
		"-c","woodstock",
		"-p","double",
		"-t","false"
	};
	
	state_manager* sm = new state_manager();
	REQUIRE(sm->parse_options(options));
	CHECK(sm->state.window_width == 500);
	CHECK(sm->state.window_height == 500);
	CHECK(sm->state.mouse_down == false);
	CHECK(sm->state.x == -1.5);
	CHECK(sm->state.y == 0.5);
	CHECK(sm->state.x_scale == 0.1);
	CHECK(sm->state.y_scale == 0.1);
	CHECK(sm->state.max_it == 100);
	CHECK(sm->state.x_mouse == 0);
	CHECK(sm->state.y_mouse == 0);
	CHECK(sm->state.colour == "woodstock");
	CHECK(sm->state.precision == "double");
	CHECK(sm->state.display_text == false);
	CHECK(sm->state.text_colour_r == 1.0);
	CHECK(sm->state.text_colour_g == 1.0);
	CHECK(sm->state.text_colour_b == 1.0);
	delete sm;
}

TEST_CASE("All CLI options. Save options, then load.", "[sm]")
{
	vector<string> options_1 = {
		"-d","250",
		"-x","-0.1",
		"-y","1.5",
		"-s","0.01",
		"-i","150",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	vector<string> options_2;
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));
	
	REQUIRE(sm_2->load_options("dummy_save_file", &options_2));
	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 250);
	CHECK(sm_2->state.window_height == 250);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.1);
	CHECK(sm_2->state.y == 1.5);
	CHECK(sm_2->state.x_scale == 0.01);
	CHECK(sm_2->state.y_scale == 0.01);
	CHECK(sm_2->state.max_it == 150);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "abyss");
	CHECK(sm_2->state.precision == "double");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 1.0);
	CHECK(sm_2->state.text_colour_b == 1.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -d on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-d","250"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 250);
	CHECK(sm_2->state.window_height == 250);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.5);
	CHECK(sm_2->state.y == 0.0);
	CHECK(sm_2->state.x_scale == 1.5);
	CHECK(sm_2->state.y_scale == 1.5);
	CHECK(sm_2->state.max_it == 250);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "greyscale");
	CHECK(sm_2->state.precision == "float");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 0.0);
	CHECK(sm_2->state.text_colour_b == 0.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -x on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.002"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 750);
	CHECK(sm_2->state.window_height == 750);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == 0.002);
	CHECK(sm_2->state.y == 0.0);
	CHECK(sm_2->state.x_scale == 1.5);
	CHECK(sm_2->state.y_scale == 1.5);
	CHECK(sm_2->state.max_it == 250);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "greyscale");
	CHECK(sm_2->state.precision == "float");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 0.0);
	CHECK(sm_2->state.text_colour_b == 0.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -y on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-y","0.002"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 750);
	CHECK(sm_2->state.window_height == 750);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.5);
	CHECK(sm_2->state.y == 0.002);
	CHECK(sm_2->state.x_scale == 1.5);
	CHECK(sm_2->state.y_scale == 1.5);
	CHECK(sm_2->state.max_it == 250);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "greyscale");
	CHECK(sm_2->state.precision == "float");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 0.0);
	CHECK(sm_2->state.text_colour_b == 0.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -s on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-s","0.004"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 750);
	CHECK(sm_2->state.window_height == 750);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.5);
	CHECK(sm_2->state.y == 0.0);
	CHECK(sm_2->state.x_scale == 0.004);
	CHECK(sm_2->state.y_scale == 0.004);
	CHECK(sm_2->state.max_it == 250);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "greyscale");
	CHECK(sm_2->state.precision == "float");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 0.0);
	CHECK(sm_2->state.text_colour_b == 0.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -i on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-i","20000"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 750);
	CHECK(sm_2->state.window_height == 750);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.5);
	CHECK(sm_2->state.y == 0.0);
	CHECK(sm_2->state.x_scale == 1.5);
	CHECK(sm_2->state.y_scale == 1.5);
	CHECK(sm_2->state.max_it == 20000);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "greyscale");
	CHECK(sm_2->state.precision == "float");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 0.0);
	CHECK(sm_2->state.text_colour_b == 0.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -c on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-c","abyss"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 750);
	CHECK(sm_2->state.window_height == 750);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.5);
	CHECK(sm_2->state.y == 0.0);
	CHECK(sm_2->state.x_scale == 1.5);
	CHECK(sm_2->state.y_scale == 1.5);
	CHECK(sm_2->state.max_it == 250);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "abyss");
	CHECK(sm_2->state.precision == "float");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 1.0);
	CHECK(sm_2->state.text_colour_b == 1.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -p on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-p","double"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 750);
	CHECK(sm_2->state.window_height == 750);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.5);
	CHECK(sm_2->state.y == 0.0);
	CHECK(sm_2->state.x_scale == 1.5);
	CHECK(sm_2->state.y_scale == 1.5);
	CHECK(sm_2->state.max_it == 250);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "greyscale");
	CHECK(sm_2->state.precision == "double");
	CHECK(sm_2->state.display_text == true);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 0.0);
	CHECK(sm_2->state.text_colour_b == 0.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Only -t on CLI. Save then load.", "[sm]")
{
	vector<string> options_1 = {
		"-t","false"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(sm_2->parse_options(options_2));
	CHECK(sm_2->state.window_width == 750);
	CHECK(sm_2->state.window_height == 750);
	CHECK(sm_2->state.mouse_down == false);
	CHECK(sm_2->state.x == -0.5);
	CHECK(sm_2->state.y == 0.0);
	CHECK(sm_2->state.x_scale == 1.5);
	CHECK(sm_2->state.y_scale == 1.5);
	CHECK(sm_2->state.max_it == 250);
	CHECK(sm_2->state.x_mouse == 0);
	CHECK(sm_2->state.y_mouse == 0);
	CHECK(sm_2->state.colour == "greyscale");
	CHECK(sm_2->state.precision == "float");
	CHECK(sm_2->state.display_text == false);
	CHECK(sm_2->state.text_colour_r == 1.0);
	CHECK(sm_2->state.text_colour_g == 0.0);
	CHECK(sm_2->state.text_colour_b == 0.0);
	delete sm_1;
	delete sm_2;
}

TEST_CASE("-l not first option provided", "[sm]")
{
	vector<string> options_1 = {
		"-t","false"
	};
	vector<string> options_2 = {
		"-d","500",
		"-l","dummy_save_file"	
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(!sm_2->parse_options(options_2));
	delete sm_1;
	delete sm_2;
}

TEST_CASE("-l first, with more options following", "[sm]")
{
	vector<string> options_1 = {
		"-t","false"
	};
	vector<string> options_2 = {
		"-l","dummy_save_file",
		"-d","500",
	};
	
	state_manager* sm_1 = new state_manager();
	state_manager* sm_2 = new state_manager();
	
	REQUIRE(sm_1->parse_options(options_1));
	REQUIRE(sm_1->save_options("dummy_save_file"));

	REQUIRE(!sm_2->parse_options(options_2));
	delete sm_1;
	delete sm_2;
}

TEST_CASE("Window dimension too small.", "[sm]")
{
	vector<string> options_1 = {
		"-x","-0.1",
		"-y","1.5",
		"-s","0.01",
		"-i","150",
		"-c","abyss",
		"-p","double",
		"-t","true",
		"-d","249"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("Window dimension too large.", "[sm]")
{
	vector<string> options_1 = {
		"-x","-0.1",
		"-y","1.5",
		"-s","0.01",
		"-i","150",
		"-d","1001",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("x too negative.", "[sm]")
{
	vector<string> options_1 = {
		"-x","-2.1",
		"-y","1.5",
		"-s","0.01",
		"-i","150",
		"-d","1000",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("x too positive.", "[sm]")
{
	vector<string> options_1 = {
		"-x","2.1",
		"-y","1.5",
		"-s","0.01",
		"-i","150",
		"-d","1000",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("Very small x permitted.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.000000000000000000000000000000001"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("y too negative.", "[sm]")
{
	vector<string> options_1 = {
		"-x","1.1",
		"-y","-2.1",
		"-s","0.01",
		"-i","150",
		"-d","1000",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("y too positive.", "[sm]")
{
	vector<string> options_1 = {
		"-x","1.1",
		"-y","2.1",
		"-s","0.01",
		"-i","150",
		"-d","1000",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("Very small y permitted.", "[sm]")
{
	vector<string> options_1 = {
		"-y","0.000000000000000000000000000000001"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("s too large.", "[sm]")
{
	vector<string> options_1 = {
		"-x","1.1",
		"-y","1.1",
		"-s","100.01",
		"-i","150",
		"-d","500",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("s too small for float precision.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.5",
		"-y","0.0",
		"-s","0.000009",
		"-i","150",
		"-d","500",
		"-c","greyscale",
		"-p","float",
		"-t","false"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("s at minimum for float precision.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.5",
		"-y","0.0",
		"-s","0.00001",
		"-i","150",
		"-d","500",
		"-c","greyscale",
		"-p","float",
		"-t","false"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(sm_1->parse_options(options_1));
	CHECK(sm_1->state.window_width == 500);
	CHECK(sm_1->state.window_height == 500);
	CHECK(sm_1->state.mouse_down == false);
	CHECK(sm_1->state.x == 0.5);
	CHECK(sm_1->state.y == 0.0);
	CHECK(sm_1->state.x_scale == 0.00001);
	CHECK(sm_1->state.y_scale == 0.00001);
	CHECK(sm_1->state.max_it == 150);
	CHECK(sm_1->state.x_mouse == 0);
	CHECK(sm_1->state.y_mouse == 0);
	CHECK(sm_1->state.colour == "greyscale");
	CHECK(sm_1->state.precision == "float");
	CHECK(sm_1->state.display_text == false);
	CHECK(sm_1->state.text_colour_r == 1.0);
	CHECK(sm_1->state.text_colour_g == 0.0);
	CHECK(sm_1->state.text_colour_b == 0.0);

	delete sm_1;
}

TEST_CASE("s too small for double precision.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.5",
		"-y","0.0",
		"-s","0.000000000000019",
		"-i","150",
		"-d","500",
		"-c","greyscale",
		"-p","double",
		"-t","false"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("s at minimum for double precision.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.5",
		"-y","0.0",
		"-s","0.00000000000002",
		"-i","150",
		"-d","500",
		"-c","greyscale",
		"-p","double",
		"-t","false"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(sm_1->parse_options(options_1));
	CHECK(sm_1->state.window_width == 500);
	CHECK(sm_1->state.window_height == 500);
	CHECK(sm_1->state.mouse_down == false);
	CHECK(sm_1->state.x == 0.5);
	CHECK(sm_1->state.y == 0.0);
	CHECK(sm_1->state.x_scale == 0.00000000000002);
	CHECK(sm_1->state.y_scale == 0.00000000000002);
	CHECK(sm_1->state.max_it == 150);
	CHECK(sm_1->state.x_mouse == 0);
	CHECK(sm_1->state.y_mouse == 0);
	CHECK(sm_1->state.colour == "greyscale");
	CHECK(sm_1->state.precision == "double");
	CHECK(sm_1->state.display_text == false);
	CHECK(sm_1->state.text_colour_r == 1.0);
	CHECK(sm_1->state.text_colour_g == 0.0);
	CHECK(sm_1->state.text_colour_b == 0.0);

	delete sm_1;
}

TEST_CASE("i too small.", "[sm]")
{
	vector<string> options_1 = {
		"-x","1.1",
		"-y","1.1",
		"-s","0.01",
		"-i","15",
		"-d","500",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("i too large.", "[sm]")
{
	vector<string> options_1 = {
		"-x","1.1",
		"-y","1.1",
		"-s","0.01",
		"-i","500001",
		"-d","500",
		"-c","abyss",
		"-p","double",
		"-t","true"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("Invalid colour palette.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.5",
		"-y","0.0",
		"-c","hamburger",
		"-s","0.01",
		"-i","150",
		"-d","500",
		"-p","double",
		"-t","false"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("Invalid precision.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.5",
		"-y","0.0",
		"-s","0.01",
		"-i","150",
		"-d","500",
		"-p","arbitrary",
		"-t","false",
		"-c","woodstock"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("Invalid text-display flag.", "[sm]")
{
	vector<string> options_1 = {
		"-x","0.5",
		"-y","0.0",
		"-s","0.01",
		"-i","150",
		"-d","500",
		"-p","float",
		"-t","skydive",
		"-c","woodstock"
	};
	
	state_manager* sm_1 = new state_manager();
	REQUIRE(!sm_1->parse_options(options_1));

	delete sm_1;
}

TEST_CASE("No value for -x.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-d","500",
			"-x"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-x",
			"-d","500"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}

TEST_CASE("No value for -y.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-i","150",
			"-y"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-y",
			"-i","150",
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}

TEST_CASE("No value for -s.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-p","float",
			"-i","150",
			"-s"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-p","float",
			"-s",
			"-i","150",
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}

TEST_CASE("No value for -d.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-p","float",
			"-c","woodstock",
			"-d"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-d",
			"-p","float",
			"-c","woodstock"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}

TEST_CASE("No value for -i.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-s","0.0002",
			"-c","woodstock",
			"-i"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-s","0.0002",
			"-i",
			"-c","woodstock"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}

TEST_CASE("No value for -c.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-s","0.0002",
			"-c"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-c",
			"-s","0.0002"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}

TEST_CASE("No value for -p.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-i","50",
			"-d","500",
			"-s","0.0002",
			"-p"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-p",
			"-i","50",
			"-d","500",
			"-s","0.0002"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}

TEST_CASE("No value for -t.", "[sm]")
{
	SECTION("End position.")
	{
		vector<string> options_1 = {
			"-i","50",
			"-d","500",
			"-s","0.0002",
			"-t"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
	
	SECTION("Other position.")
	{
		vector<string> options_1 = {
			"-p",
			"-i","50",
			"-d","500",
			"-s","0.0002"
		};
	
		state_manager* sm_1 = new state_manager();
		REQUIRE(!sm_1->parse_options(options_1));

		delete sm_1;
	}
}