#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <thread>

class StockSimulator {
private:
    double iter;
    double S_0;
    double T;
    double days;
    double timestep;
    double mu;
    double sigma;
    std::vector<std::vector<double>> prices;

public:
    StockSimulator(double iterations, double initialPrice, double years, double numDays, double timeStep, double mu, double sigma);

    void runSimulation();

    const std::vector<std::vector<double>>& getPrices() const;

    void printPrices() const;
};
