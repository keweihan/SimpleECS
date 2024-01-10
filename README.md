# SimpleECS
A basic 2D game framework implementing entity component system using [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/). 

Users of this framework can programatically construct a visual scene using provided definitions for `Entity` and `Components` as well as implement custom components by deriving from `Components`.

The framework also provides components that gives basic physics properties such as velocity and collision as well as rendering for visualization. 

This framework was made for personal use.

## Current Features
- Implements ECS Structure with classes `Entity` and `Component`
- Basic rendering
   - Rectangle Rendering
   - Line Rendering
- Basic physics system
  - Basic Box-Box collision detection and resolution
  - Velocity
  - Time
- Input management
- Sound effects
  
## Demos
[A simple implementation of Pong](https://github.com/keweihan/Pong) using SimpleECS:


https://github.com/keweihan/SimpleECS/assets/55893673/2c8724b5-a8c4-4367-8d7e-cfb05d333f5f


_More to come_

## Attribution
Referenced [Lazy Foo](https://lazyfoo.net/tutorials/SDL/index.php) for SDL introduction.
