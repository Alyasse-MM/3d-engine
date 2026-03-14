# Software-Based 3D Engine

A custom-built 3D graphics pipeline implemented in Modern C++. 

## The Goal
This is an educational project whose goal is to learn the mechanics of 3D rendering. It serves as a practical exploration of computer graphics, from linear algebra to the rendering of a scene.

## Core Systems
* **Custom Math**: Fixed-function pipeline math using a bespoke Maths namespace.
* **CPU Based**: All geometry processing is performed on the CPU.
* **Build**: Powered by xmake for efficient setup and dependency management.
* **Modular Architecture**: Decoupled systems for Engine State, Scene management, Rendering, and Input handling.

## Usage
1. Install [xmake](https://xmake.io/)
2. Run `xmake` to compile the project.
3. Run `xmake run` to launch the engine.