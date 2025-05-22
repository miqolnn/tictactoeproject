#include "GameState.hpp"

GameState::GameState() : board("#########") {}

bool GameState::applyPlayerMove(int index) {
    if (index >= 0 && index < 9 && board[index] == '#') {
        board[index] = 'X';
        return true;
    }
    return false;
}

void GameState::applyAIMove(int index) {
    if (index >= 0 && index < 9 && board[index] == '#') {
        board[index] = 'O';
    }
}

std::string& GameState::getBoardState() {
    return board;
}

bool GameState::isGameOver() const {
    const std::string& b = board;
    int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };

    for (auto& line : wins) {
        if (b[line[0]] != '#' &&
            b[line[0]] == b[line[1]] &&
            b[line[1]] == b[line[2]]) {
            return true;
        }
    }

    return b.find('#') == std::string::npos;
}

