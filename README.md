# Cub3D 🚀

Welcome to the **Cub3D** project. This project is inspired by the famous first-person shooter **Wolfenstein 3D**. The goal is to create a simple 3D game engine using raycasting to render a 2D map into a 3D view.

## Overview 🗺️

**Cub3D** is a basic raycasting engine that provides a 3D perspective in a 2D environment. The project focuses on understanding the fundamentals of rendering, vectors, and raycasting. It uses the **MLX42** graphics library for managing window display and rendering graphics.

### Key Objectives 🎯
- Implement a simple 3D game engine using raycasting techniques.
- Load and display a map from a `.cub` file format.
- Create a first-person view from the player's perspective, simulating a 3D experience.
- Handle player movement and interaction within the map (e.g., collision detection).

## Features 🛠️

- **2D Map Rendering**: Display a 2D top-down view of the environment.
- **Raycasting**: Render the 3D scene based on the player's position and orientation.
- **Player Movement**: Move the player through the map with keyboard controls.
- **Basic Wall Textures**: Render textured walls based on map design.

## Usage 🕹️

1. Clone the repository and compile the project:
   ```bash
   make

2. Run the program with a valid .cub map file:
   ```bash
   ./cub3D maps/sample_map.cub

2. Use the following controls to navigate:
    - **W/A/S/D**: Move forward, left, backward, and right.
    - **Left/Right Arrow**: Rotate the player's view.
    - **ESC**: Exit the game.

## MLX42 Graphics Library 🎨

The project uses the **MLX42** library for graphics rendering and window management.
Make sure to have it installed and configured before running the project.
For installation instructions, visit the official [MLX42 GitHub repository](https://github.com/codam-coding-college/MLX42).

---

Good luck and happy coding! 🎮



