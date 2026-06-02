# DS_OOP_HW03

## Introduction!
This project is the implementation of Assignment 3 for the DS&OOP course at National Yang Ming Chiao Tung University (NYCU).

The objective of this assignment is to design and implement a Graph System using Object-Oriented Programming principles. The system supports graph construction, graph visualization, graph traversal, shortest path analysis, node depth calculation, and graph editing operations.

The project is built upon the framework provided by the instructor and extends it by implementing the required graph functionalities within the student work modules.

## Project Objectives!
The main objectives of this project are:

- Implement graph generation algorithms.
- Support node and edge manipulation.
- Perform shortest path calculations.
- Calculate node depth information.
- Maintain graph integrity after modifications.
- Apply object-oriented design principles in a practical graph application.

## Development Environment!
- Course: Data Structures and Object-Oriented Programming (DS&OOP)
- Institution: National Yang Ming Chiao Tung University (NYCU)
- Language: C++
- IDE: Microsoft Visual Studio
- Framework: Instructor-provided OpenGL Graph Framework

-------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Graph Generation Functions!
Implemented multiple graph construction algorithms to support different graph topologies for testing and visualization.

### Features Implemented:

#### Connected Circle Graph!
- Creates nodes arranged in a circular layout.
- Connects adjacent nodes to form a closed loop.
- Ensures all nodes are reachable.

#### Connected Square Graph!
- Places nodes around the boundary of a square.
- Connects neighboring nodes to form a square structure.
- Supports configurable node counts.

#### Radical Circle Graph!
- Creates a circular graph with additional radial connections.
- Improves graph connectivity compared to a simple circle graph.
- Useful for shortest path and traversal testing.

#### Random Graph!
- Generates nodes at random positions.
- Creates random edges between nodes.
- Used for stress testing graph operations.

### Purpose!!
These graph generation methods provide different graph structures for testing node operations, edge operations, shortest path analysis, and depth calculations.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------
NEXT WE SHALL CONTINUE WITH:
## Node and Edge Operations
Implemented graph editing operations for interactive graph manipulation.

### Node Deletion
- Allows users to select and remove nodes.
- Automatically removes all edges connected to the deleted node.
- Updates active node lists and graph data structures.

### Edge Deletion
- Allows users to remove selected edges.
- Updates adjacency information for affected nodes.
- Maintains graph consistency after removal.

### Automatic Graph Deletion
- Continuously removes nodes from the graph.
- Removes corresponding edges automatically.
- Stops when no nodes remain.

### Purpose
These operations allow dynamic modification of graph structures during runtime and provide support for graph maintenance and testing.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------
NEXT WE SHALL CONTINUE WITH:

## Shortest Path Analysis
Implemented shortest path computation between selected nodes.

### Functionality
- Select a start node.
- Select a destination node.
- Calculate the shortest path connecting both nodes.
- Highlight the resulting path for visualization.

### Applications
- Graph traversal analysis.
- Connectivity verification.
- Route optimization demonstration.

### Result
Users can visually inspect the minimum path between two nodes within the graph.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------
NEXT WE SHALL CONTINUE WITH:

## Node Depth Calculation
Implemented node depth analysis to determine graph traversal levels.

### Functionality
- Select a root node.
- Traverse connected nodes.
- Compute depth values relative to the root.
- Display depth information for graph analysis.

### Applications
- Graph hierarchy analysis.
- Breadth-first traversal visualization.
- Structural graph inspection.

### Result
Users can evaluate node relationships and graph structure using depth information.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------
NEXT WE SHALL CONTINUE WITH:

## Testing and Debugging
Performed comprehensive testing of all implemented graph functionalities.

### Tested Components
- Node creation
- Edge creation
- Graph generation functions
- Node deletion
- Edge deletion
- Automatic deletion
- Shortest path calculation
- Node depth analysis

### Bug Fixes
- Corrected graph connectivity issues.
- Improved node selection behavior.
- Fixed edge update inconsistencies after deletion.
- Verified graph integrity after modifications.

### Final Status
All required functionalities were successfully implemented and validated for submission.
