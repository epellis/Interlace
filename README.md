# Interlace ++

## Install
1. Ensure OpenCV 2.4.x is installed and locatable by cmake
2. Install OpenMP (For Ubuntu: `sudo apt install libomp-dev`)
3. `cmake .`
4. `make`

## Using
1. `./Interlace VideoA VideoB`, VideoA and VideoB are videos with Codecs supported by your installation of OpenCV
2. `./Interlace VideoA VideoB <width> <height>`, width and height denote the resolution of the interlaced video

## Troubleshooting
1. There is the possibility that my CMake was compiling for OpenCV 3.X when I tested (also installed to my computer) so it may be necessary to install OpenCV 3.X as well
2. I believe the Makefile will work if OpenMP cannot be located but if not you can remove the one instance in `FrameProducer.cpp`

## Tips
- This program will give you optimal performance when running on a machine with 3+ cores
- To run unit tests go to `Interlace.cpp` and uncomment the line stating: `#define UNITTEST`
