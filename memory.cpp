#include "simulation.h"

// Function implementations

void generateRequest(SimulationData& simData, int clock) {
    simData.totalRequests++;

    int blockSize = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
    int leaseDuration = rand() % (MAX_LEASE - MIN_LEASE) + MIN_LEASE;
    int leaseExpiry = clock + leaseDuration;

    bool requestSatisfied = false;

    // First attempt to allocate
    for (int i = 0; i < simData.freeList.size(); i++) {
        if (simData.freeList[i].second >= blockSize) {
            requestSatisfied = true;

            simData.satisfiedRequests++;
            simData.totalBlockSize += blockSize;

            // Update smallest and largest block sizes
            if (blockSize < simData.smallestBlock) {
                simData.smallestBlock = blockSize;
            }
            if (blockSize > simData.largestBlock) {
                simData.largestBlock = blockSize;
            }

            simData.totalLeaseDuration += leaseDuration;

            // Update shortest and longest lease durations
            if (leaseDuration < simData.shortestLease) {
                simData.shortestLease = leaseDuration;
            }
            if (leaseDuration > simData.longestLease) {
                simData.longestLease = leaseDuration;
            }

            // Allocate block
            range allocated = { simData.freeList[i].first, blockSize };
            simData.allocatedList.push_back({ allocated, leaseExpiry });

            // Update the free list
            simData.freeList[i].second -= blockSize;
            simData.freeList[i].first += blockSize;

            if (simData.freeList[i].second == 0) {
                simData.freeList.erase(simData.freeList.begin() + i);
            }
            return;
        }
    }

    // Merge and retry if first attempt failed
    if (!requestSatisfied) {
        std::sort(simData.freeList.begin(), simData.freeList.end());
        for (size_t i = 0; i < simData.freeList.size() - 1; ++i) {
            if (simData.freeList[i].first + simData.freeList[i].second == simData.freeList[i + 1].first) {
                simData.freeList[i].second += simData.freeList[i + 1].second;
                simData.freeList.erase(simData.freeList.begin() + i + 1);
                --i;
                simData.mergeCount++;
            }
        }

        for (int i = 0; i < simData.freeList.size(); i++) {
            if (simData.freeList[i].second >= blockSize) {
                requestSatisfied = true;

                simData.satisfiedRequests++;
                simData.totalBlockSize += blockSize;

                // Update smallest and largest block sizes
                if (blockSize < simData.smallestBlock) {
                    simData.smallestBlock = blockSize;
                }
                if (blockSize > simData.largestBlock) {
                    simData.largestBlock = blockSize;
                }

                simData.totalLeaseDuration += leaseDuration;

                // Update shortest and longest lease durations 
                if (leaseDuration < simData.shortestLease) {
                    simData.shortestLease = leaseDuration;
                }
                if (leaseDuration > simData.longestLease) {
                    simData.longestLease = leaseDuration;
                }

                // Allocate block
                range allocated = { simData.freeList[i].first, blockSize };
                simData.allocatedList.push_back({ allocated, leaseExpiry });

                // Update the free list 
                simData.freeList[i].second -= blockSize;
                simData.freeList[i].first += blockSize;

                // Remove the element if the size is zero
                if (simData.freeList[i].second == 0) {
                    simData.freeList.erase(simData.freeList.begin() + i);
                }
                return;
            }
        }

        // If still unsatisfied, count as unsatisfied
        if (!requestSatisfied) {
            simData.unsatisfiedRequests++;
        }
    }
}

void checkExpiredLeases(SimulationData& simData, int clock) {
    while (!simData.allocatedList.empty() && simData.allocatedList.front().second <= clock) {
        range expired = simData.allocatedList.front().first;
        simData.allocatedList.erase(simData.allocatedList.begin());

        simData.freeList.push_back(expired);
        std::sort(simData.freeList.begin(), simData.freeList.end());
    }
}

void mergeFreeList(std::vector<range>& freeList) {
    std::sort(freeList.begin(), freeList.end());

    for (size_t i = 0; i < freeList.size() - 1; ++i) {
        if (freeList[i].first + freeList[i].second == freeList[i + 1].first) {
            freeList[i].second += freeList[i + 1].second;
            freeList.erase(freeList.begin() + i + 1);
            --i;
        }
    }
}

void dumpCore(const SimulationData& simData, double avgBlockSize, double avgLeaseDuration) {
    std::cout << "\n-----------------------------------------------\n";
    std::cout << "                SIMULATION STATS\n";
    std::cout << "-----------------------------------------------\n";

    std::cout << "Total Requests: " << simData.totalRequests << "\n";
    std::cout << "Satisfied Requests: " << simData.satisfiedRequests << "\n";
    std::cout << "Unsatisfied Requests: " << simData.unsatisfiedRequests << "\n";
    std::cout << "Smallest Block Size: " << simData.smallestBlock << "\n";
    std::cout << "Largest Block Size: " << simData.largestBlock << "\n";
    std::cout << "Average Block Size: " << avgBlockSize << "\n";
    std::cout << "Shortest Lease: " << simData.shortestLease << "\n";
    std::cout << "Longest Lease: " << simData.longestLease << "\n";
    std::cout << "Average Lease Duration: " << avgLeaseDuration << "\n";
    std::cout << "Merges Performed: " << simData.mergeCount << "\n";

    std::cout << "-----------------------------------------------\n";
    std::cout << "                  FREE LIST\n";
    std::cout << "-----------------------------------------------\n";
    for (const auto& r : simData.freeList)
        std::cout << "Start: " << r.first << ", Size: " << r.second << "\n";

    std::cout << "\n-----------------------------------------------\n";
    std::cout << "              ALLOCATED LIST\n";
    std::cout << "-----------------------------------------------\n";
    for (const auto& a : simData.allocatedList)
        std::cout << "Start: " << a.first.first << ", Size: " << a.first.second
        << ", Lease Expiry: " << a.second << "\n";

    std::cout << "-----------------------------------------------\n";
}



int main() {
    // Seed random generator
    srand(static_cast<unsigned>(time(nullptr)));

    // Free and allocated memory lists
    std::vector<range> freeList = { {0, MEMORY_SIZE} };
    std::vector<alloc> allocatedList;

    // Simulation data structure
    SimulationData simData = { freeList, allocatedList };

    // Simulation loop
    for (int clock = 0; clock < TIME_LIMIT; ++clock) {
        if (clock % REQUEST_INTERVAL == 0) {
            generateRequest(simData, clock);
        }
        checkExpiredLeases(simData, clock);
    }

    // Compute averages
    double avgBlockSize = 0.0;
    double avgLeaseDuration = 0.0;

    // static casting to double to avoid integer division errors
    if (simData.satisfiedRequests > 0) {
        avgBlockSize = static_cast<double>(simData.totalBlockSize) / simData.satisfiedRequests;
        avgLeaseDuration = static_cast<double>(simData.totalLeaseDuration) / simData.satisfiedRequests;
    }

    dumpCore(simData, avgBlockSize, avgLeaseDuration);

    return 0;
}
