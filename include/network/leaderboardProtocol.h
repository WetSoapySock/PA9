#ifndef PA9_LEADERBOARDPROTOCOL_H
#define PA9_LEADERBOARDPROTOCOL_H

#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace Minesweeper {

    struct LeaderboardEntry {
        std::string playerName;
        int seconds;
        std::string difficulty;
        int rank;        // Top 10 ranks
    };

    enum class SubmitResult {
        ACCEPTED_TOP_10,       // Player made leaderboard
        REJECTED_NOT_TOP_10,   // Player did not make top 10
        INVALID_SCORE,         // >=999 range
        CONNECTION_FAILED      // Network error
    };

    /**Gen AI, "How can we use the network protocol libraries to create a player leaderboard with network sockets?
        The server will send back a string with thier name and best time." */
    class LeaderboardProtocol {
    public:
        static std::string formatSubmit(const std::string& name, int seconds, const std::string& difficulty) {
            return "SUBMIT:" + name + ":" + std::to_string(seconds) + ":" + difficulty + "\n";
        }

        static std::string formatGet(const std::string& difficulty) {
            return "GET:" + difficulty + "\n";
        }

        static bool parseSubmitResponse(const std::string& response) {
            return response == "OK\n";
        }

        static std::vector<LeaderboardEntry> parseLeaderboard(const std::string& data, const std::string& difficulty) {
            std::vector<LeaderboardEntry> entries;
            std::istringstream iss(data);
            std::string line;

            while (std::getline(iss, line)) {
                if (line.empty()) continue;

                size_t dotPos = line.find('.');
                size_t colonPos = line.find(':');

                if (dotPos != std::string::npos && colonPos != std::string::npos) {
                    LeaderboardEntry entry;
                    entry.rank = std::stoi(line.substr(0, dotPos));
                    entry.playerName = line.substr(dotPos + 1, colonPos - dotPos - 1);
                    entry.seconds = std::stoi(line.substr(colonPos + 1));
                    entry.difficulty = difficulty;
                    entries.push_back(entry);
                }
            }
            return entries;
        }
    };

} // namespace Minesweeper

#endif //PA9_LEADERBOARDPROTOCOL_H
