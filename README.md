# 3D Interactive Thermoelasticity Simulator

A real-time, interactive 3D physics simulator written in C and OpenGL/GLUT. This project visualizes linear elastic mechanics and thermoelasticity on a 3D grid using kinematic deformation. 

## Features
* **Real-Time Deformation:** Interactively drag points on the mesh to see immediate strain and stress propagation.
* **Thermoelasticity:** "Paint" a thermal field ($\theta - \theta_0$) onto the mesh to observe thermally induced stress due to constrained expansion.
* **Dynamic Material Properties:** Adjust Young's Modulus ($E$) and Poisson's Ratio ($\nu$) on the fly.
* **Multiple View Modes:** Heatmap visualization of:
  * Von Mises Equivalent Strain
  * Von Mises Equivalent Stress
  * Elastic Strain Energy Density
  * Thermal Field (Temperature change)

## Physics Background
The simulation computes the total strain tensor $E_{ij}$ using spatial derivatives of the displacement field. It then applies the **Duhamel-Neumann** equations for isotropic thermoelasticity to compute the stress tensor $T_{ij}$:

$$T_{ij} = \lambda \text{tr}(E - E^{th}) \delta_{ij} + 2\mu (E_{ij} - E^{th}_{ij})$$

Where the thermal strain is: 

$$E^{th}_{ij} = \alpha (\theta - \theta_0) \delta_{ij}$$

For a linear isotropic thermoelastic material, the Helmholtz free energy density $\psi$ is defined as:

$$\psi(\mathbf{E}, \theta) = \frac{1}{2} \lambda (\text{tr}\mathbf{E})^2 + \mu \text{tr}(\mathbf{E}^2) - (3\lambda + 2\mu)\alpha (\theta - \theta_0) \text{tr}\mathbf{E} + C(\theta - \theta_0)$$

## Dependencies
To compile and run this project, you need a C compiler and the OpenGL utility toolkit:
* `gcc`
* `OpenGL`
* `GLU`
* `FreeGLUT` (or standard GLUT)

On Debian/Ubuntu-based systems, install dependencies via:
```bash
sudo apt-get install build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
```

## Build and Run
This project includes a `Makefile` for easy compilation. 

1. **Compile the project:**
```bash
   make
```
2. **Run the executable:**
```bash
    make run
```
3. **Clean build files:**
```bash
    make clean
```

## Controls
### Interaction
* **Left-Click + Drag:** Rotate the camera.
* **Right-Click + Drag:** Perform the active action (Deform or Paint Heat).
* **`M` or `m`:** Toggle Right-Click Action (Deform Mesh <-> Paint Heat).
* **Spacebar:** Cycle through Visualization Modes (Strain, Stress, Energy, Temperature).

### Material Tuning
* **`e` / `E`:** Decrease / Increase Young's Modulus.
* **`p` / `P`:** Decrease / Increase Poisson's Ratio.
* **`R` or `r`:** Reset the grid, temperature, and camera to the initial state.
* **`ESC`:** Exit application.