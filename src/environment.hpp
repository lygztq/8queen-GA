#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "state.hpp"
#include <climits>

inline int weightedRandomIndex(const std::vector<int> & weight) {
    int weightSum = weight.back();
    int randomPos = (std::rand() % weightSum) + 1;
    int left = 0, right = static_cast<int>(weight.size()) - 1, mid = 0;

    while (left < right) {
        mid = (left + right) / 2;
        if (weight[mid] > randomPos) right = mid;
        else if (weight[mid] < randomPos) left = mid + 1;
        else return mid;
    }
    return right;
}

class Environment {
private: 
    std::vector<State> m_population;
    State m_bestState;
    int m_boardSize;
    float m_mutationRate;

public:
    Environment() = default;
    Environment(int initialPopulation, int boardSize, float mutationRate)
    : m_population(std::vector<State>(initialPopulation, State(boardSize, true)))
    , m_bestState(State(boardSize, false))
    , m_boardSize(boardSize)
    , m_mutationRate(mutationRate) {}

    // functions
    State Evaluate(size_t times) {
        size_t currentLoop = 0;
        while (currentLoop < times) {
            if (evaluate()) return m_bestState;
            ++currentLoop;
        }
        int bestFitnessValue = 0;
        int index = 0;
        for (auto i = 0; i < m_population.size(); ++i) {
            if (m_population[i].FitnessValue() > bestFitnessValue) {
                index = i;
                bestFitnessValue = m_population[i].FitnessValue();
            }
        }
        return m_population[index];
    }
    int MaxFitnessValue() const {
        return m_boardSize * (m_boardSize - 1) / 2;
    }
    bool GetBestResult() const {
        return m_bestState.FitnessValue() >= MaxFitnessValue();
    }

private:
    bool shouldMutate() const { return std::rand() / float(RAND_MAX) < m_mutationRate; }
    bool evaluate() {
        std::vector<int> weight(m_population.size());
        auto weightPtr = weight.begin();
        std::vector<State> nextGeneration(m_population.size());
        int fitnessValueSum = 0;

        // check if exist individual match the goal 
        // and calculate weight for weighted random sampling
        for (const auto & s : m_population) {
            if (s.FitnessValue() == MaxFitnessValue()) {
                m_bestState = s;
                return true;
            }
            fitnessValueSum += s.FitnessValue();
            (*weightPtr) = fitnessValueSum;
            ++weightPtr;
        }

        // get next generation
        for (auto i = 0; i < m_population.size(); ++i) {
            int father = weightedRandomIndex(weight);
            int mother = weightedRandomIndex(weight);
            int dividedPos = (std::rand() % (m_boardSize - 1)) +  1;
            std::vector<int> newBoard(m_boardSize);

            int j = 0;
            for (; j < dividedPos; ++j) newBoard[j] = m_population[father][j];
            for (; j < m_boardSize; ++j) newBoard[j] = m_population[mother][j];
            nextGeneration[i].SetBoard(newBoard);
            if (shouldMutate()) nextGeneration[i].Mutate();
        }
        std::swap(nextGeneration, m_population);
        return false;
    }
};

#endif // ENVIRONMENT_HPP
