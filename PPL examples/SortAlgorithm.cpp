//  In particular, sorting in parallel is useful when you have a large dataset or when you use a computationally-expensive compare operation to sort your data.

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <execution>

// Helper function for getting the current time for profiling
auto get_time() { return std::chrono::high_resolution_clock::now(); }

void measuredSort(std::vector<int>& v, bool isParallel = false) {
    auto start = get_time();
    if(isParallel) std::sort(std::execution::par, begin(v), end(v));
    else std::sort(begin(v), end(v));
    auto finish = get_time();

    // Print out the execution time
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << "Elapsed time = " << duration.count() << " ms\n";

}

int main() {
    // Size of our vector
    const int N = 1 << 20;

    // Create our vector
    std::vector<int> v(N);

    // Set up our random number generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<int> dist(0, 255);

    // Generate our random inputs and copy it to second vector
    std::generate(begin(v), end(v), [&]() { return dist(rng); });
    std::vector<int> v2(v);

    // Compare execution times
    std::cout << "Sequential sort: " << std::endl;
    measuredSort(v);
    std::cout << "Parallel sort: " << std::endl;
    measuredSort(v2, true);
    
    return 0;
}