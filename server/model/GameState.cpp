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

