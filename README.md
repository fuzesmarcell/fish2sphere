# Fish2Sphere

Convert a fish eye image to a partial spherical image. This implementation is based on: https://paulbourke.net/dome/fish2/#3

## Build
Dependencies for the tooling are automatically downloaded via CMake.
There are no dependencies for the projection code.

CMake options:
* `-DF2S_TOOLS=ON` Include tools for doing the projection. This will use stb libraries for reading and writing images

```sh
git clone git@github.com:fuzesmarcell/fish2sphere.git
cd fish2sphere
mkdir build && cd build
cmake ..
```

## Running
If you have build the tools you can use the command `fish2sphere` to convert fish eye images.

It takes the following arguments in order some of them are optional
* `fish eye` the input fish eye image
* `w` the width of the output. The height will be `w/2`
* `fov=180` Field of view in degrees of the fish eye image
* `theta=0` Offset in degrees latitude
* `phi=0` Offset in degrees longtitude

The output image will have the same name as the input with a `spherical_` prefix and will be a `.png`

```sh
fish2sphere fisheye.jpg 1000
fish2sphere fisheye.jpg 800 220 180 0
```