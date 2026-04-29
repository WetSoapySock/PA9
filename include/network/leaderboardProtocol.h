#define PA9_LEADERBOARDPROTOCOL_H
#define PA9_LEADERBOARDPROTOCOL_H

#pragma once
#include <string>
#include <vector>

// CITE AI
namespace Minesweeper {

    struct LeaderboardEntry {
        std::string playerName;
        int seconds;
        std::string difficulty;
        int rank;
    };

    enum class SubmitResult {
        ACCEPTED_TOP_10,
        REJECTED_NOT_TOP_10,
        INVALID_SCORE,
        CONNECTION_FAILED
    };

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
