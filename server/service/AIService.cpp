#include "AIService.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <cctype>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

std::string parse_ai_response(const std::string& response) {
    try {
        json response_json = json::parse(response);
        if (response_json.contains("choices") && !response_json["choices"].empty()) {
            return response_json["choices"][0]["message"]["content"];
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
    }
    return "-1";
}

std::string AIService::getAIMove(std::string& board_state) {
    CURL* curl = curl_easy_init();
    std::string response_string;

    if (!curl) return "-1";

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.groq.com/openai/v1/chat/completions");

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Authorization: Bearer <your-api-key>");

    std::string prompt = "You are playing tic-tac-toe as 'O'. The current board state is: " + board_state +
        ". Provide your next move as a single digit from 0-8, only return the digit.";

    std::string jsonData = R"({
        "model": "llama-3.3-70b-versatile",
        "messages": [{"role": "user", "content": ")" + prompt + R"("}]
    })";

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        return "-1";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    std::string move = parse_ai_response(response_string);
    for (char c : move) {
        if (isdigit(c)) return std::string(1, c);
    }

    return "-1";
}

