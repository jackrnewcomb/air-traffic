# Air Traffic Simulation (Microservices + ECS + C++)

A modular air-traffic simulation implemented in C++, using microservices architecture and an Entity-Component-System (ECS) design pattern.

This project models an air-traffic control environment including aircraft movement, routing, and real-time visualization using SFML.

---

## Overview

This repository contains:

- A simulated air traffic environment
- A microservices architecture separating simulation logic from visualization  
- An Entity-Component-System (ECS) core for flexible runtime behavior  
- A 2D visualization of aircraft using SFML  
- Clean modular code with simulation and rendering decoupled

The structure emphasizes real-time systems, modular design, and maintainable architecture.

---

## Features

- ECS architecture for flexible entity logic  
- Microservices-style separation of simulation and visualization  
- Real-time position updates and movement logic  
- SFML-backed 2D rendering of aircraft  
- Component-based system expansion  
- Clean build process with CMake

---

## Tech Stack

- **Language:** C++  
- **Architecture:** ECS (Entity-Component-System)  
- **Visualization:** SFML  
- **Build:** CMake  
- **Messaging:** Custom simulation messaging between modules  
- **Tools:** Git, CMake, SFML

---

## Build Instructions

### Prerequisites

- C++17 compatible compiler  
- CMake  
- SFML development libraries

### Steps

```bash
mkdir build
cd build
cmake ..
make
```

## Usage 

Simply run the `execute.exe` from `build/Execution` to see a sample output of the air traffic visualization.

## Author 

Jack Newcomb
