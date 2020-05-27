# Optimal placement of traffic security cameras using Vertex cover algorithms  
  
This work belongs to the coursework done by me for, ECE650: Methods and Tools for software Engineering, University of Waterloo.  
**Languages used:**  
- Python 2
- C++  
## Overview
The project is to help the local police department with their installation of security cameras at traffic intersections.
You will solve a particular kind of optimization problem, called the Vertex Cover problem, in this
context. The idea is for the police to be able to minimize the number of cameras they need to
install, and still be as effective as possible with their monitoring.  
  
*The entire project is approched with a series of assignments.*    
  
**Module 1:** Developed a python script that can parse vertices of given streets and localize the one or more intersections, finally structuring the given streets as an undirected graph.  
**Module 2:** Developed a C++ script that finds the shortest path for a given undirected graph using Breadth-First Search algorithm.  
**Module 3:** Developed two C++ scripts namely rgen and driver. Rgen generates random streets for any given user specification, Driver is the control program that helps to couple Module 1 , Module 2 and Rgen. It is developed using IPC.  
**Module 4:** Developed a C++ script that reduces a VERTEX COVER to CNF-SAT and incorporates MiniSAT solver  to compute the minimum sized Vertex cover for any given undirected graph.  
**Final Module:** Developed two more Vertex cover algorithms and did a Time analysis of all three algorithms that finds the minimum Vertex Cover.  

  
