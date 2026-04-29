#ifndef PA9_LEADERBOARDSERVER_H
#define PA9_LEADERBOARDSERVER_H

#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <mutex>
#include <algorithm>
#include <fstream>
#include <sstream>

// CITE AI
#pragma comment(lib, "ws2_32.lib")

namespace Minesweeper {

    struct ServerLeaderboardEntry {
        std::string playerName;
        int seconds;
        std::string difficulty;
    };

    class LeaderboardServer {
    private:
        SOCKET serverSocket;
        bool isRunning;
        std::mutex dataMutex;
        std::vector<ServerLeaderboardEntry> leaderboard;
        std::string dataFile;

    public:
        LeaderboardServer() : serverSocket(INVALID_SOCKET), isRunning(false), dataFile("leaderboard.txt") {
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);
            loadFromFile();
        }

        ~LeaderboardServer() {
            stop();
            WSACleanup();
        }

        bool start(int port = 8080);
        void stop();
        void run();

    private:
        void handleClient(SOCKET clientSocket);
        std::string processCommand(const std::string& command);
        std::string getLeaderboardAsString(const std::string& difficulty);
        void addScore(const ServerLeaderboardEntry& entry);
        void sortLeaderboard();
        void saveToFile();
        void loadFromFile();
        int getTop10Threshold(const std::string& difficulty);
        int getCountForDifficulty(const std::string& difficulty);
    };
} // namespace Minesweeper



#endif //PA9_LEADERBOARDSERVER_H
