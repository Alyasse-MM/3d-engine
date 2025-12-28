# 3D Engine from Scratch

**A 3D rendering engine built from scratch.**

## Project Goal & Motivation
The primary goal of this project is educational: to demystify 3D graphics by building an engine from scratch, without relying on hardware acceleration (OpenGL/Vulkan) or high-level 3D libraries (Unity/Three.js).

By implementing every step of the pipeline manually—from matrix transformations to rasterization for the core—I aim to gain a deep, mathematical understanding of how a computer turns a list of numbers into a 3D world.

## Current Version: V0.1
It is the first version. It focuses on the **fundamental rendering loop**. It renders two 3D objects (Cubes) with a fully traversable camera system, implementing the complete mathematics of the graphics pipeline.

<p align="center">
  <img src="screenshots/demo_rotation.gif" alt="3D Engine Demo" height="300">
  <img src="screenshots/demo_cam_movements.gif" alt="3D Engine Demo" height="300">
</p>

### Learning Objectives:
* **Linear Algebra:** Understanding Linear Algebra in a visual context (Matrices, Dot/Cross Products).
* **Pipelinen Implementation:** Implementing the 3D Graphics Pipeline (Model -> View -> Projection).
* **Problem solving:** Solving geometric problems like Clipping (Sutherland-Hodgman) and Visibility (Back-face culling).
* **Rasterization:** Writing a software rasterizer using standard 2D plotting tools.

### Key Features
* **Matrices:** Custom implementation of Rotation Matrices (X/Y/Z) and Perspective Projection.
* **Camera System:** 6-DOF (Degrees of Freedom) camera with Translation and Yaw rotation.
* **Rendering Pipeline:**
    * **Back-face Culling:** Hides faces pointing away from the camera using dot products.
    * **Clipping:** Implements the **Sutherland-Hodgman algorithm** to clip triangles against the near plane (preventing divide-by-zero errors and visual glitches).
    * **Depth Sorting:** Uses a **centroid-based Painter's Algorithm** to draw faces in the correct order.
* **Input Management:** Custom event listener for separating Model rotation (spinning the object) from View rotation (moving the camera).

### Tech Stack
* **Language:** Python 3.14.0
* **Libraries:**
    * `numpy`: For vector and matrix math.
    * `matplotlib`: Used strictly as a 2D drawing canvas. No 3D toolkits used.

## Controls
| Key | Action | Context |
| :--- | :--- | :--- |
| **Arrow Keys** | Rotate the two cubes | Model Rotation |
| **Z / S** (or W/S) | Move Forward / Backward | Camera Movement |
| **Q / D** (or A/D) | Strafe Left / Right | Camera Movement |
| **A / E** | Turn Camera Left / Right | Camera Rotation (Yaw) |

---

## How to Run
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Alyasse-MM/3d-engine.git](https://github.com/Alyasse-MM/3d-engine.git)
    cd 3d-engine
    ```
2.  **Install dependencies:**
    ```bash
    pip install -r requirements.txt
    ```
3.  **Run the engine:**
    ```bash
    python engine.py
    ```

---

*This project is a work in progress.