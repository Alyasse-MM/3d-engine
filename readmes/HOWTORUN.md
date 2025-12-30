# How To Run
**[⬅ Back to Main README](../README.md)**

Below are all the instructions needed to run the project.

## Prerequisites
* **C++ Compiler:** GCC 13+, Clang, or MSVC (Must support C++17).
* **CMake:** Version 3.10 or higher.
* **SFML 3.0:** Must be installed and discoverable by CMake.

## Build Instructions
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Alyasse-MM/3d-engine.git](https://github.com/Alyasse-MM/3d-engine.git)
    cd 3d-engine
    ```

2.  **Generate Build Files:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Compile and Run:**
    ```bash
    cmake --build .
    
    # On Windows
    .\3d-engine.exe
    
    # On Linux/Mac
    ./3d-engine
    ```

**[⬆ Back to Top](#)**