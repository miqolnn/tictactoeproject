#include "GameController.hpp"
#include "../model/GameState.hpp"
#include "../service/AIService.hpp"
#include <rapidjson/document.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

extern std::mutex cout_mutex;

void handle_client(int client_socket, std::string client_ip) {
    char buffer[1001];
    std::string client_name, difficulty;

    ssize_t received = recv(client_socket, buffer, 1000, 0);
    if (received > 0) {
       buffer[received] = '\0';
       rapidjson::Document jsonData;
       jsonData.Parse(buffer);
       try {
           client_name = jsonData["name"].GetString();
           difficulty = jsonData["difficulty"].GetString();
        } catch (...) {
           client_name = "Unknown";
           difficulty = "easy";
    	}
    }

    GameState game;

    while (true) {
        received = recv(client_socket, buffer, 1000, 0);
        if (received <= 0) break;

        buffer[received] = '\0';
        std::string message(buffer);
        std::string reply;
        if (message[0] == 'r') {
            game = GameState();
    	    continue;          
	}
        if (message.size() == 1 && isdigit(message[0])) {
            int move = std::stoi(message);
            bool valid = game.applyPlayerMove(move - 1);

            if (!valid) {
                reply = "error";
                send(client_socket, reply.c_str(), reply.size(), 0);
                continue;
            }

            if (game.isGameOver()) {
		if (game.getBoardState().find('#') == std::string::npos) {
                std::cout<<"DRAW\n";
                send(client_socket, "Draw", 4, 0);
		} else {
                send(client_socket, "Player wins", 12, 0);
		std::cout<<"WIN"<<std::endl; }
		continue;
            }

            std::string ai_move = AIService::getAIMove(game.getBoardState(), difficulty);
            if (!isdigit(ai_move[0])) {
                reply = "error";
                send(client_socket, reply.c_str(), reply.size(), 0);
                continue;
            }

            int ai_index = std::stoi(ai_move) - 1;
            game.applyAIMove(ai_index);

            send(client_socket, ai_move.c_str(), ai_move.size(), 0);

            if (game.isGameOver()) {
                usleep(300000);  
                const std::string& board = game.getBoardState();
		std::cout<<"AIWIN\n";
		send(client_socket, "AI wins", 7, 0);
                continue;
            }

        } else {
            reply = "error";
            send(client_socket, reply.c_str(), reply.size(), 0);
        }

        std::cout << "\nbstate " + game.getBoardState() + "\n";
    }

    close(client_socket);
}

