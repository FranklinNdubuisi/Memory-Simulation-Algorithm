#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Aliases for readability
using range = std::pair<int, int>; // {starting address, size}
using alloc = std::pair<range, int>; // {range , lease Expiry}

// Constants
const int MIN_SIZE = 50;
const int MAX_SIZE = 350;
const int MIN_LEASE = 40;
const int MAX_LEASE = 70;
const int MEMORY_SIZE = 1000;
const int TIME_LIMIT = 1000;
const int REQUEST_INTERVAL = 10;

// Simulation data structure to hold all related simulation state
struct SimulationData {
    std::vector<range>& freeList;
    std::vector<alloc>& allocatedList;
    int totalRequests = 0;
    int satisfiedRequests = 0;
    int unsatisfiedRequests = 0;
    int mergeCount = 0;
    int smallestBlock = MAX_SIZE;
    int largestBlock = MIN_SIZE;
    int totalBlockSize = 0;
    int shortestLease = MAX_LEASE;
    int longestLease = MIN_LEASE;
    int totalLeaseDuration = 0;
};

// Function prototypes
void generateRequest(SimulationData& simData, int clock);
void checkExpiredLeases(SimulationData& simData, int clock);
void mergeFreeList(std::vector<range>& freeList);
void dumpCore(const SimulationData& simData, double avgBlockSize, double avgLeaseDuration);

#endif // SIMULATION_H

