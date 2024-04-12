# 50.051 Programming Language Concepts (PLC) Project

## Image Processor in C

### Add context here

### Add setup instructions here
These instructions are only for windows. Our application has only been tested out in windows.

### GTK Setup

Please download MSYS2 [here](https://www.msys2.org/wiki/MSYS2-installation/) if not already installed.
Add bin path of path/to/msys64/mingw/bin to env variables path

Open MSYS2 terminal and run the following commands:<br>
pacman -S mingw-w64-x86_64-gtk3 <br>
pacman -S mingw-w64-x86_64-toolchain base-devel

### Image magic setup
Install Image Magick [here](https://imagemagick.org/script/download.php) <br>
Please install <b>Win64 dynamic at 8 bits-per-pixel component<b>

### Add overview of features 

**Rotation:** Rotates the image in both positive and negative multiples of 90 degrees. The user input for this function is the angle that they want to rotate the image by. 

**Scaling:** Acts as a “zoom” and will scale image to make it bigger. The user Inputs for this function is the scale factor and the corner that they want to zoom into - Top-left, Top-right, Bottom-left and Bottom-right. 

**Cropping:** Crops the image based on given on required dimensions and start point The user Inputs the start coordinates and new image dimensions. 

**Colour Mapping:** A color filter than produces an image as if red/blue/green/grey light was shone on the image. This function gets the colour that the user want to map the image to.

**Edge Detection:** Produce a graymap and have black as the background and the edges of objects defined in gray/white. This function does not take in any user inputs.
