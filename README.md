# Star Knight
Star Knight - A game dev project.

## Dependencies & Development Environment

The project is developed using Ubuntu 22.04. The build system uses CMake which can be installed the following way.

```sh
sudo apt install cmake
```

### SDL

#### SDL Dependencies
Before SDL is installed, the following apt packages are needed.
```sudo apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev```

#### SDL Build and Install
The project requires [SDL](https://www.libsdl.org/) which is used for user input and windowing. SDL is included in this project as a ```git submodule``` which lives in the ```lib/``` folder.

### Versions of Dependencies

- SDL: 2.26.4
- CMake: 3.22.1
- C++ STL: C++17