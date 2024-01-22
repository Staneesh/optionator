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
                double change = (timestep/(T*days)) * mu + sigma * temp * sqrt(timestep / (T*days)) * dW;
                temp = temp + change;
            }
            prices[i][j] = temp;
        }
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
