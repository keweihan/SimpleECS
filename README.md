# SimpleECS
A basic 2D game framework implementing entity component system using [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/). 

Users of this framework can programatically construct a visual scene using provided definitions for `Entity` and `Components` as well as implement custom components by deriving from `Components`.

`Components` are stored contiguously for faster iteration and access. 

The framework also provides components that gives basic physics properties such as velocity and collision as well as rendering for visualization. 

This framework was made for personal use.

## Current Features
- Implements Entity Component System with pooled components.
- Basic rendering
   - Rectangle Rendering
   - Line Rendering
- Basic physics system
  - Basic Box-Box collision detection and resolution
  - Grid based collision detection
  - Momentum/mass based collisions 
  - Velocity
  - Time
- Input management
- Sound effects

## Getting Started
### Prerequsites
- [Python](https://www.python.org/) (>=3.8)
- [Conan](https://conan.io/downloads) 
- [CMake](https://cmake.org/download/)
- [CUDA](https://developer.nvidia.com/cuda-downloads) 

### Setup
1. Run installer helper `python3 scripts/build.py install`

### Build and run
1. Optionally configure scene in `demos/collisionStress.cpp`
2. Build executable `python src/build.py build --type release`
3. Execute runnable in `./build/bin/collisionStress`
