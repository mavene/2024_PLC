# 50.051 Programming Language Concepts (PLC) Project

## Image Processor in C

An image processor written in C, allowing users to manipulate images using various filters and tools in a single edit session. Transformations applied are immediately displayed in a GUI. 

Several constraints were fulfiled as part of the requirements of the 50.051 Programming Language Concepts course - namely, the implementation of a parser for any inputs and a Finite State Machine (FSM) to manage the overall program.

## Setup Instructions

### Download Dependencies

**NOTE**: Due to time constraints, the program is guaranteed to compile on Windows 64-bit systems but full testing on Linux distributions was not finished. Installations for Linux are included for completeness.

This application depends on 2 external programs - 1) GTK for the preview image display and 2) Image Magick to handle conversions on the fly from PNG to PPM and vice versa. 

#### 1. GTK

##### Windows (MSYS64) - Recommended
Please download MSYS2 [here](https://www.msys2.org/wiki/MSYS2-installation/) if not already installed.

Add bin path of path/to/msys64/mingw/bin to system env variables path.

Open MSYS2 terminal and run the following commands:<br>
    pacman -S mingw-w64-x86_64-gtk3
    pacman -S mingw-w64-x86_64-toolchain base-devel

##### Windows (Cygwin) 
Please download CYGWIN [here](https://www.cygwin.com/install.html) if not already installed.

Add bin path of path/to/cygwin64/bin to system env variables path.

Run the setup-x86_64.exe application and install the following packages:<br>
    libgtk3-devel
    libgtk3_0

##### Linux
Assuming the distribution is Debian/Ubuntu, run:
    apt-get install libgtk-3-0
    apt-get install libgtk-3-dev
    
#### 2. Image Magick

##### Windows
Install Image Magick [here](https://imagemagick.org/script/download.php) <br>
Please install <b>Win64 dynamic at 8 bits-per-pixel component<b>

##### Linux
Use the same link as above. 
Please install the appropriate rpm version for your Linux distribution and follow the official setup instructions on the website. 

### Building from Source

Once the dependencies are installed, you can build the source files using the following command*:

    make -f .\Makefile_MSYS64

There should now be an executable in the /bin folder alongside its required dlls. Run the program using:

    ./bin/image_processor.exe

**\*Note:** A CYGWIN-specific Makefile is also provided but we recommend using the MSYS64 development environment. 

### Usage

A sample of PNG images can be found in /media. Note there is a corrupt PNG included (truncated) to demonstrate error cases during an upload which should re-prompt the user to input a valid file.

You will be prompted with a list of commands at every step of the program. To proceed with committing or undoing a change, close the preview window pop-up to re-enable the terminal session. 

## Features

**Rotation:** Rotates the image. Users can specify the angle of rotation in positive and negative multiples of 90 degrees. 

**Scaling:** Acts as a “zoom” and will scale image to make it bigger. Users can specify the scale factor and corner that they want to zoom into - Top-left, Top-right, Bottom-left and Bottom-right. 

**Cropping:** Crops the image based on given on required dimensions and start point. Users can specify the start coordinates (x,y) and new image dimensions (width, height). 

**Colour Mapping:** Separates out colour channels like a "colour filter". Users can specify the colour filter (R,G,B).

**Grey Mapping:** Converts image to a greyscale. No additional user inputs required.

**Edge Detection:** Produce a bitmap - black as the background and the edges of objects defined in gray/white. No additional user inputs required.