# Graph-Based Density Clustering System

## Overview
This project was developed as part of the **Data Structures and Algorithms (DSA) course**. It implements a density-based clustering approach to identify groups of connected nodes in a graph, based on edge weights. The system processes graph data, applies clustering techniques, and provides statistical insights and cluster information.

## Features
- **Graph Clustering Algorithm**: Uses density-based clustering to group connected nodes based on predefined thresholds.
- **Input Graph Processing**: Reads graph data from a structured text file and processes edge relationships.
- **Cluster Node Visualization**: Displays which nodes belong to which clusters after processing.
- **Statistical Analysis**: Provides numerical insights about the clusters, such as the number of clusters formed and the size of each cluster.
- **User-Friendly Interface**: Simple UI (Tkinter-based) to allow users to run clustering and view results.
- **Efficient Performance**: Implemented in C++ for high-speed computation, with Python UI integration.

## Input File Format
The input text file contains edge relationships in the following format:
```
Node1  Node2  EdgeWeight
```
Example:
```
TFC3    MYO4    0.014
ZBF2    LMP9    0.028
```
- **Column 1**: First node identifier.
- **Column 2**: Second node identifier.
- **Column 3**: Edge weight representing the strength of the connection between the nodes.

## How to Run
1. **Compile the C++ File**
   ```sh
   g++ -o graph_clustering graph_clustering.cpp
   ```
2. **Run the Clustering Algorithm**
   ```sh
   ./graph_clustering graph.txt
   ```
3. **Run the Python UI**
   ```sh
   python ui.py
   ```

## Requirements
- **C++ Compiler** (e.g., g++)
- **Python 3**
- **Tkinter** (for UI)

## Project Structure
- `graph.txt` – Input file containing the graph data.
- `graph_clustering.cpp` – Core clustering algorithm implementation.
- `ui.py` – User interface to interact with clustering results.
