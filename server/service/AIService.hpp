#ifndef AISERVICE_HPP
#define AISERVICE_HPP

#include <string>

class AIService {
public:
    static std::string getAIMove(std::string& board_state, std::string difficulty);
};

#endif

