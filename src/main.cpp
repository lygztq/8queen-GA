#include "environment.hpp"
#include <fstream>

int main() {
    int numExperiments, numGeneration, populationSize, boardSize;
    float mutationRate;
    std::cout << "num of experiment: ";
    std::cin >> numExperiments;
    std::cout << "num of generation per experiment: ";
    std::cin >> numGeneration;
    std::cout << "mutation rate: ";
    std::cin >> mutationRate;
    std::cout << "population size: ";
    std::cin >> populationSize;
    std::cout << "board size: ";
    std::cin >> boardSize;

    // log file
    std::string filename("simulation_result.txt");
    std::ofstream ostrm(filename);

    ostrm << "num of experiment: " << numExperiments << std::endl \
          << "num of generation per experiment: " << numGeneration << std::endl \
          << "mutation rate: " << mutationRate << std::endl \
          << "population size: " << populationSize << std::endl \
          << "board size: " << boardSize << std::endl \
          << "-------------------\n\n";

    int currentExperiment = 0, successCount = 0;
    State::SetRandomSeed();
    while (currentExperiment < numExperiments) {
        Environment env(populationSize, boardSize, mutationRate);
        State best = env.Evaluate(numGeneration);
        ostrm << "-----\nExperiment: " << currentExperiment << std::endl \
              << "FitnessValue: " << best.FitnessValue() << "/" << env.MaxFitnessValue() << std::endl;
        if (env.GetBestResult()) {
            ++successCount;
            ostrm << best.ToString() << std::endl;
        }
        ++currentExperiment;
    }

    float rate = successCount * 1.0 / numExperiments;
    ostrm << "Rate of success: " << rate << std::endl;

    return 0;
}
