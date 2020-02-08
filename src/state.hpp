#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>

// class for board state
class State {
private:
    int m_fitnessValue;
    std::vector<int> m_board;

public:
    State() = default;
    State(size_t boardSize, bool needInitialization=true)
    : m_board(std::vector<int>(boardSize)) {
        if (needInitialization) {
            generateState();
            m_fitnessValue = State::computeFitnessValue(m_board);
        }
        else 
            m_fitnessValue = 0;
    }
    
    // get member value
    size_t BoardSize() const { return m_board.size(); }
    int FitnessValue() const { return m_fitnessValue; }

    // functions
    void Mutate() {
        size_t index = std::rand() % BoardSize();
        m_board[index] = std::rand() % BoardSize();
        m_fitnessValue = State::computeFitnessValue(m_board);
    }

    void SetBoard(const std::vector<int> & newBoard) {
        m_board = newBoard;
        m_fitnessValue = State::computeFitnessValue(m_board);
    }
    std::string ToString() const {
        std::ostringstream s;
        for (auto i : m_board) {
            for (auto j = 0; j < BoardSize(); ++j) {
                s << (i == j) << " ";
            }
            s << std::endl;
        }
        return s.str();
    }
    void Print() const {
        std::cout << ToString();
    }
    static void SetRandomSeed() {
        std::srand(std::time(0));
    }

    // operators
    int operator[](int i) const { return m_board[i]; }

private:
    static int computeFitnessValue(const std::vector<int> &board) {
        int boardSize = static_cast<int>(board.size());
        int fitnessValue = 0;
        for (int i = 0; i < boardSize; ++i) 
            for (int j = i; j < boardSize; ++j) 
                if (board[j] != board[i] && \
                    board[j] != board[i] + j - i && \
                    board[j] != board[i] + i - j)
                    fitnessValue += 1;
        return fitnessValue;
    }
    void generateState() {
        size_t boardSize = BoardSize();
        std::for_each(m_board.begin(), m_board.end(), [boardSize](int &i) { i = std::rand() % boardSize; } );
    }
};

#endif // STATE_HPP
