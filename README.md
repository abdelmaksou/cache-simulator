# Set Associative Cache Simulator
A Set Associative cache simulator that simulates the methodology of a set associative cache using C++ to measure the cahce hit and miss ratios on different random test cases.

![alt text](https://i.imgur.com/c8x3Buy.png)
# How to use:
The program uses FTXUI for its Terminal UI Library. The user is able to choose one of different memory generators, 4 different ways, two experiments, and 4 different line sizes. It displays the main info about the cache and the experiment chosen or constructed, so that the user have all the info before running
it. The result of the hit and miss ratios will be displayed at the bottom. 
There are two pre-defined experiments:
• For every possible line size (number of ways = 4), the hit ratio is calculated.
• For every possible number of ways (line size = 16 bytes), the hit ratio is calculated.
If one of the Experiments were chosen, a graph will be displayed to show the resulted hit rates.

# Dependencies and required libraries
- FTXUI C++
- CMake

# Programming Languages Used
- C++

# How to install and run?
```bash
git clone https://github.com/abdelmaksou/cache-simulator.git
cd cache-simulator
mkdir build
cd build
cmake ..
make -j
./cache
```
