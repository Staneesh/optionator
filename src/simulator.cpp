#include "simulator.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

StockSimulator::StockSimulator(double iterations, double initialPrice, double years, double numDays, double timeStep, double mu, double sigma)
    : iter(iterations), S_0(initialPrice), T(years), days(numDays), timestep(timeStep), mu(mu), sigma(sigma),
      prices(iterations, std::vector<double>(numDays, 0.0)) {}


void StockSimulator::runSimulation() {
    // Function to be executed by each thread
    auto simulationThread = [this](int start, int end) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::normal_distribution<double> distribution(0.0, 1.0);

        for (int i = start; i < end; ++i) {
            double temp = S_0;

            for (int j = 0; j < T * days; ++j) {
                for (int k = 0; k < 1 / timestep; ++k) {
                    double dW = distribution(generator);
                    double change = timestep * temp * mu / 365.0 + sigma/sqrt(365.0) * temp * sqrt(timestep) * dW;
                    temp = temp + change;
                }
                prices[i][j] = temp;
            }
        }
    };

    // Determine the number of threads to use
    unsigned int numThreads = std::thread::hardware_concurrency();
    unsigned int numThreadsUsed = 0;
    std::vector<std::thread> threads;

    std::cerr << "Number of threads available: " << numThreads << std::endl;
    std::cerr << "Enter a number of threads to use in simulation:\n";
    std::cin >> numThreadsUsed;

    // Distribute work among threads
    int iterationsPerThread = static_cast<int>(iter / numThreadsUsed); //number of iterations (each iteration generates a path, so we want to split our code by iterations) assigned for each thread.
    for (unsigned int i = 0; i < numThreadsUsed; ++i) {
        int start = i * iterationsPerThread; // calculates starting index (iteration of simulation) for the current thread
        int end = (i == numThreadsUsed - 1) ? iter : (i + 1) * iterationsPerThread; // calculates ending index (iteration of simulation) for the current thread. If the current thread is the last one, it sets "end" to the total number of iterations, otherwise it sets "end" to (i+1) * iterationsPerThread.
        threads.emplace_back(simulationThread, start, end); // creates and launches a new thread. Executes simulationThread function, passing "start" and "end" as arguments. .emplace_back function is used to add new element (thread) to the "threads" vector.
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join(); // if exception is thrown during the execution of the thread, the "std::terminate" is called and forces program to stop (so if it doesn't succeed then program will terminate itself.
    }
}



const std::vector<std::vector<double>>& StockSimulator::getPrices() const {
    return prices;
}

void StockSimulator::printPrices() const {
    for (const auto& iteration : prices) {
        for (double price : iteration) {
            std::cout << price << " ";
        }
        std::cout << std::endl;
    }
}
