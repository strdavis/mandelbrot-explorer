# Mandelbrot Explorer
<img src="demo/spectrum.gif" width="300"/> <img src="demo/woodstock.gif" width="300"/>
<br />
<img src="demo/abyss.gif" width="300"/> <img src="demo/greyscale.gif" width="300"/>
<br /><br />

## OVERVIEW

Mandelbrot Explorer is an interactive Linux application for exploring the Mandelbrot set. It displays a dynamic image of the set which you can traverse in real time using the mouse. The following parameters can be configured:

- Colour palette
- Zoom depth limit
- Level of detail
- Initial position
- Resolution
- Textual display of coordinates and scale

Mandelbrot Explorer can also save and load positions, making it easy to return to them. It is written in C++ with OpenGL.<br/>

```Note:``` Windows users can run Mandelbrot Explorer on the latest version of [WSL](https://learn.microsoft.com/en-us/windows/wsl/), which supports GUI applications.<br />

## DEPENDENCIES

The following tools must be installed on your system:

- C++ 17
- OpenGL 4.5
- [CMake 3.2](https://cmake.org/)
- [GLFW 3.2.1](https://www.glfw.org/)
- [GLEW](https://glew.sourceforge.net/)
- [FreeType](https://freetype.org/)
- [fmt](https://github.com/fmtlib/fmt)
- [Catch2](https://github.com/catchorg/Catch2)

The font [FreeSans.ttf](https://www.gnu.org/software/freefont/) must be installed at:

```/usr/share/fonts/gnu-free/FreeSans.ttf```
  
## QUICKSTART

In the root directory of the repo:
```
mkdir build
cd build
cmake ..
cmake --build .
```
  
Then launch with:
```
./mandelbrot_explorer
```

## CONTROLS

### MOUSE

Click-and-drag to traverse. Use the mouse scroll wheel to zoom.<br />

### KEYBOARD

Press the ```Q``` key to quit.<br /><br />
To save the current explorer state:
  1. Press the ```S``` key.
  2. Click on the terminal window.
  3. Type a name for the save file.
  4. Press enter.
  5. Click on the Mandelbrot Explorer window to continue exploring.

The save file is created in the directory where ```mandelbrot_explorer``` is being executed.  
Saved states can be loaded on startup via the command-line interface (see "OPTIONS").

## OPTIONS

Configure Mandelbrot Explorer via the command line at startup:
```
  ./mandelbrot_explorer <option1> <value1> <option2> <value2> . . . 
```

The following options are provided:

| Option | Description |
| --- | --- |
| ```-d``` | Set window dimension i.e. resolution (width = height, in number of pixels).<br />Accepts int in range [250, 1000].<br />Window is always square, and is not resizable.<br />Default is 750. |
| ```-x``` | Set initial x-position of image center.<br />Accepts double in range [-2.0, 2.0].<br />Default is -0.5.|
| ```-y``` | Set initial y-position of image center.<br />Accepts double in range [-2.0, 2.0].<br />Default is 0.0.|
| ```-s``` | Set initial image scale.<br />If precision is set to ```float``` (default), accepts float in range [0.00001, 100.0].<br />If precision is set to ```double```, accepts double in range [0.00000000000002, 100.0].<br />Default is 1.5.|
| ```-i``` | Set maximum number of iterations used in fractal-generation algorithm.<br />Accepts int in range [16, 50000].<br />Greater values produce more detailed images.<br />Default is 250.<br /><br />Note that greater values will degrade performance more or less quickly depending on your hardware. When increasing this value, try increments of a few hundred or so. If you find a region that is all black at a deep level of zoom, increasing this value will usually reveal hidden details.|
| ```-c``` | Set colour palette.<br />Accepts the following strings: <code>greyscale</code>, <code>woodstock</code>, <code>abyss</code>, <code>spectrum</code>.<br />Default is <code>greyscale</code>.|
| ```-p``` | Set numerical precision (controls zoom depth limit).<br />Accepts the following strings: <code>float</code>, <code>double</code>.<br />Default is <code>float</code>.<br /><br /><code>float</code> configures the explorer to use single-precision floating point computations. This gives better performance, with a shallower maximum zoom. <code>double</code> configures the explorer to use double-precision floating point computations. This gives worse performance, with a deeper maximum zoom.|
| ```-t``` | Toggle the textual display of current position and scale.<br />Accepts the following strings: <code>true</code>, <code>false</code><br /> Default is <code>true</code>.<br /><br />In text display, "X" is the x-coordinate of the current image center, "Y" is the y-coordinate of the current image center, and "S" is the scaleof the current image.|
| ```-l``` | Load state from a save file.<br />Accepts path to save file (relative path from directory where mandelbrot_explorer is being executed).<br />Cannot be used in conjunction with any other options.|
          
### OPTIONS EXAMPLES

To run the explorer with a window dimension of 500 pixels, 
at a starting position of (-0.3731, -0.6466), and a starting scale of 0.05:
```
./mandelbrot_explorer -d 500 -x -0.3731 -y -0.6466 -s 0.05
```

To run the explorer with a maximum number of iterations of 500,
using the "woodstock" colour palette:
```
./mandelbrot_explorer -i 500 -c woodstock
```

The run the explorer using the "abyss" colour palette, at double precision,
and without the text display:
```
./mandelbrot_explorer -c abyss -p double -t false
```

To load a saved state from a file called "mysavefile":
```
./mandelbrot_explorer -l mysavefile
```

## TESTING
After building, run the test program:
```
./test_state_manager
```

This tests the functionality for parsing command-line input, loading state, and saving state. To test rendering and user input, run the program and verify its behaviour manually.


