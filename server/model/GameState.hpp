#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>

class GameState {
public:
    GameState();
    bool applyPlayerMove(int index);
    std::string& getBoardState();
    void applyAIMove(int index);

    bool isGameOver() const;
private:
    std::string board;
};

#endif

