My name is Franklin Ndubuisi. This is lab Assignment 6 for CS2010. This C++ program simulates memory management, allocating blocks of memory based on random requests. It tracks statistics such as the total number of requests, satisfied/unsatisfied requests, smallest/largest block sizes, and lease durations.
The program uses several constants to define the simulation's parameters:

MIN_SIZE = 50: Minimum block size in the memory.
MAX_SIZE = 350: Maximum block size in the memory.
MIN_LEASE = 40: Minimum lease duration for allocated blocks.
MAX_LEASE = 70: Maximum lease duration for allocated blocks.
MEMORY_SIZE = 1000: Total memory size.
TIME_LIMIT = 1000: Simulation time limit (how many cycles to run).
REQUEST_INTERVAL = 10: The interval at which memory allocation requests are made. (for a maximum of 100 requests 1000/10).
This program has 2 files a header file and a cpp file. To run this program make sure to download/clone the repository and run under ubuntu.
compile:  g++ memory.cpp -o memory_simulation
run :  ./memory_simulation
