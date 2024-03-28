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
  - Velocity
  - Time
- Input management
- Sound effects
  
## Demos
[A simple implementation of Pong](https://github.com/keweihan/Pong) using SimpleECS:


https://github.com/keweihan/SimpleECS/assets/55893673/2c8724b5-a8c4-4367-8d7e-cfb05d333f5f


[Simulating 13k+ collision agents at 60fps](https://github.com/keweihan/Pong) using SimpleECS:
https://github.com/keweihan/SimpleECS/assets/55893673/24fa9c14-4bb8-4ea1-889f-69957d70c8d0


_More to come_

## References
Referenced [Lazy Foo](https://lazyfoo.net/tutorials/SDL/index.php) for SDL introduction.
