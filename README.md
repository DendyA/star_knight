# Star Knight
Star Knight - A game dev project.

## Dependencies & Development Environment

The project is developed using Ubuntu 22.04. The build system uses CMake which can be installed the following way.

```sh
sudo apt install cmake
```

### SDL

#### SDL Dependencies
Before SDL is to be built, the following apt packages are needed.
```shell
sudo apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

#### SDL Build
The project requires [SDL](https://www.libsdl.org/) which is used for user input and windowing. SDL is included in this project as a ```git submodule``` which lives in the ```lib/``` folder.

### bgfx

#### bgfx Dependencies
Before bgfx is to be built (more information [here](https://bkaradzic.github.io/bgfx/build.html)), the following apt packages are needed.

```shell
sudo apt install libgl1-mesa-dev x11proto-core-dev libx11-dev
```

#### bgfx Build
The project requires [bgfx](https://bkaradzic.github.io/bgfx/overview.html) and its dependencies which is used as a wrapper around various rendering backends. bgfx and its dependencies are included in this project as git submodules, which live in the ```lib/bgfx_cmake/``` folder. The ```bgfx_cmake/``` folder is itself a submodule.

### Versions of Dependencies

- C++ STL: C++17
- CMake: 3.22.1
- SDL: 2.26.4
- bgfx_cmake: f8ea763
- bgfx: c3e3053
- bimg: c3b3c6b
- bx: 4e67e34
