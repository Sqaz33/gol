#include "include/gameOfLife.hpp"

#include <array>
#include <algorithm>
#include <stdexcept>
#include <list>
#include <utility>
#include <iostream>

// #define DEBUG

using game_of_life::GameOfLife;
using game_of_life::Cell;
using game_of_life::CellList;

namespace {
    CellList::iterator getIterOnCellFromLivingCells(CellList& cells, size_t x, size_t y) {
        return std::find(
            cells.begin(), cells.end(), Cell(x, y)
        );
    }
}

size_t GameOfLife::height() const {
    return gameField.size();
}

size_t GameOfLife::width() const {
    return gameField[0].size();
}

bool GameOfLife::isAlive(size_t x, size_t y) const {
    return gameField[y][x] == 1;
}

void GameOfLife::kill(size_t x, size_t y) {
    auto it = getIterOnCellFromLivingCells(
        livingCells, x, y
    );
    if (it != livingCells.end()) {
        livingCells.erase(it);
        gameField[y][x] = false;
    }
}

void GameOfLife::revive(size_t x, size_t y) {
    auto it = getIterOnCellFromLivingCells(
        livingCells, x, y
    );

    if (it == livingCells.end()) {
        livingCells.push_front(Cell(x, y));
        gameField[y][x] = true;
    }
}

void GameOfLife::clearField() {
    gameField = Field(height(), Row(width(), 0));
    livingCells = std::list<Cell>();
}


template <class T>
static void printV(const std::vector<T>& v) {
    #ifdef DEBUG
    for (const auto& a : v) {
        for (const auto& b : a) {
            std::cout << b << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::string(v.size(), '-') + std::string(v.size(), '-') + '\n';
    #endif
}


void GameOfLife::renderNextGameFieldState() {
    Field newGameFild(height(), Row(width(), 0));
    printV(newGameFild);
    int n_x;
    int n_y;
    bool liveStatus; 
    Cell cell;
    for (auto it = livingCells.begin(); it != livingCells.end(); ) {
        cell = *it;
        // просчитать liveStatus для живой занесенной в livingCell клетки
        liveStatus = computeLiveStatus(countNeighbors(cell.first, cell.second), true);
        if (liveStatus) {
            newGameFild[cell.second][cell.first] = 1;
            ++it;
        } else {
            newGameFild[cell.second][cell.first] = 2;
            it = livingCells.erase(it);
        }

        // просчитать liveStatus  для соседей livingCell клетки
        for (const auto& n : neighbors) {
            n_x = n.first + cell.first;
            n_y = n.second + cell.second;

            if (n_x < 0) {
                n_x = gameField[0].size() - 1;
            } else if (n_x >= gameField[0].size()) {
                n_x = 0;
            }

            if (n_y < 0) {
                n_y = gameField.size() - 1;
            } else if (n_y >= gameField.size()) {
                n_y = 0;
            }

            // если клетка еще не обработана
            if (newGameFild[n_y][n_x] == 0) {
                liveStatus = computeLiveStatus(countNeighbors(n_x, n_y), false);
                if (liveStatus) { 
                    newGameFild[n_y][n_x] = 1;
                    livingCells.push_front(Cell(n_x, n_y));
                } else {
                    newGameFild[n_y][n_x] = 2;
                }
            }
        }
    }
    
    printV(newGameFild);
    gameField = std::move(newGameFild);
}

bool GameOfLife::computeLiveStatus(size_t neighborsCount, bool liveStatus) {
    return (!liveStatus && neighborsCount == 3) || // Revive
            ((neighborsCount == 2 || neighborsCount == 3) && liveStatus); // Live continue 
}           // Kill/ Deth continue

const std::array<const std::pair<int, int>, 8> GameOfLife::neighbors {{
    {-1,  -1}, {0, -1}, {1, -1},
    {-1,   0},          {1,  0},
    {-1,   1}, {0,  1}, {1,  1}  
}};


size_t GameOfLife::countNeighbors(size_t x, size_t y) const {
    size_t count = 0;
    int n_x;
    int n_y;
    for (const auto& n : neighbors) {
        n_x = n.first + x;
        n_y = n.second + y;

        if (n_x < 0) {
            n_x = gameField[0].size() - 1;
        } else if (n_x >= gameField[0].size()) {
            n_x = 0;
        }

        if (n_y < 0) {
            n_y = gameField.size() - 1;
        } else if (n_y >= gameField.size()) {
            n_y = 0;
        }
    
        count += gameField[n_y][n_x] == 1 ? 1 : 0;
    }
    return count;
}


