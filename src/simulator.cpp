#include "simulator.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

StockSimulator::StockSimulator(double iterations, double initialPrice, double years, double numDays, double timeStep, double mu, double sigma)
    : iter(iterations), S_0(initialPrice), T(years), days(numDays), timestep(timeStep), mu(mu), sigma(sigma),
      prices(iterations, std::vector<double>(numDays, 0.0)) {}

void StockSimulator::runSimulation() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < iter; ++i) {
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
}

void StockSimulator::runSimulationParallel() {
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
    std::vector<std::thread> threads;

    // Distribute work among threads
    int iterationsPerThread = static_cast<int>(iter / numThreads);
    for (unsigned int i = 0; i < numThreads; ++i) {
        int start = i * iterationsPerThread;
        int end = (i == numThreads - 1) ? iter : (i + 1) * iterationsPerThread;
        threads.emplace_back(simulationThread, start, end);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
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
