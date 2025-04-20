#include "GameController.hpp"
#include "../model/GameState.hpp"
#include "../service/AIService.hpp"

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

extern std::mutex cout_mutex;

void handle_client(int client_socket, std::string client_ip) {
    char buffer[1001];
    std::string client_name;

    ssize_t received = recv(client_socket, buffer, 1000, 0);
    if (received > 0) {
        buffer[received] = '\0';
        client_name = std::string(buffer);
    }

    GameState game;

    while (true) {
        received = recv(client_socket, buffer, 1000, 0);
        if (received <= 0) break;

        buffer[received] = '\0';
        std::string message(buffer);

        std::string reply;

        if (message.size() == 1 && isdigit(message[0])) {
            int move = std::stoi(message);
            if (game.applyPlayerMove(move - 1)) {
                std::string ai_move = AIService::getAIMove(game.getBoardState());
                if (isdigit(ai_move[0])) {
                    game.applyAIMove(std::stoi(ai_move));
                    reply = ai_move;
                } else {
                    reply = "error";
                }
            } else {
                reply = "error";
            }
        } else {
            reply = "error";
        }

        send(client_socket, reply.c_str(), reply.size(), 0);
    }

    close(client_socket);
}

