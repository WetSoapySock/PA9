#include "leaderboardServer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>

namespace Minesweeper {

    // ======================== CONSTRUCTOR & DESTRUCTOR ========================

    LeaderboardServer::LeaderboardServer()
        : serverSocket(INVALID_SOCKET),
        isRunning(false),
        dataFile("leaderboard.txt")
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

        if (result != 0)
        {
            std::cerr << "[ERROR] WSAStartup failed with code: " << result << std::endl;
            return;
        }

        loadFromFile();
    }

    LeaderboardServer::~LeaderboardServer()
    {
        stop();
        WSACleanup();
    }
    // ======================== SERVER LIFECYCLE ========================

    bool LeaderboardServer::start(int port) {
        // Create TCP socket
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (serverSocket == INVALID_SOCKET)
        {
            std::cerr << "[ERROR] Failed to create socket. WSA error: "
                << WSAGetLastError() << std::endl;
            return false;
        }

        // Configure server address
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all network interfaces
        serverAddr.sin_port = htons(port);

        // Bind socket to port
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "[ERROR] Failed to bind to port " << port << std::endl;
            closesocket(serverSocket);
            return false;
        }

        // Start listening for connections (max 10 pending connections)
        if (listen(serverSocket, 10) == SOCKET_ERROR) {
            std::cerr << "[ERROR] Failed to listen on socket" << std::endl;
            closesocket(serverSocket);
            return false;
        }

        isRunning = true;
        std::cout << "[SUCCESS] Leaderboard Server running on port " << port << std::endl;
        std::cout << "[INFO] Data file: " << dataFile << std::endl;
        std::cout << "[INFO] Waiting for connections..." << std::endl;

        return true;
    }

    void LeaderboardServer::stop() {
        isRunning = false;
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }
    }

    void LeaderboardServer::run() {
        while (isRunning) {
            // Accept client connection (blocking call)
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);

            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

            if (clientSocket != INVALID_SOCKET) {
                // Get client IP address for logging
                char clientIP[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
                std::cout << "[CONNECTION] Client connected from " << clientIP << std::endl;

                // Handle client request
                handleClient(clientSocket);

                // Close client connection
                closesocket(clientSocket);
                std::cout << "[DISCONNECT] Client disconnected" << std::endl;
            }
        }
    }

    // ======================== CLIENT HANDLING ========================

    void LeaderboardServer::handleClient(SOCKET clientSocket) {
        char buffer[1024];
        int bytesReceived;

        // Receive command from client
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string command(buffer);

            // Remove trailing newline if present
            if (!command.empty() && command.back() == '\n') {
                command.pop_back();
            }

            std::cout << "[COMMAND] Received: " << command << std::endl;

            // Process command and get response
            std::string response = processCommand(command);

            // Send response back to client
            send(clientSocket, response.c_str(), response.length(), 0);
            std::cout << "[RESPONSE] Sent: " << response.substr(0, response.find('\n')) << std::endl;
        } else if (bytesReceived == 0) {
            std::cout << "[INFO] Client disconnected gracefully" << std::endl;
        } else {
            std::cerr << "[ERROR] recv failed with error: " << WSAGetLastError() << std::endl;
        }
    }

    // ======================== COMMAND PROCESSING ========================

    std::string LeaderboardServer::processCommand(const std::string& command) {
        // Parse command format: "SUBMIT:PlayerName:Time:Difficulty" or "GET:Difficulty"
        size_t firstColon = command.find(':');
        if (firstColon == std::string::npos) {
            return "ERROR: Invalid command format\n";
        }

        std::string cmdType = command.substr(0, firstColon);

        if (cmdType == "SUBMIT") {
            // Parse: "SUBMIT:PlayerName:Time:Difficulty"
            std::string rest = command.substr(firstColon + 1);

            size_t secondColon = rest.find(':');
            if (secondColon == std::string::npos) {
                return "ERROR: Invalid SUBMIT format\n";
            }

            std::string playerName = rest.substr(0, secondColon);
            rest = rest.substr(secondColon + 1);

            size_t thirdColon = rest.find(':');
            if (thirdColon == std::string::npos) {
                return "ERROR: Invalid SUBMIT format\n";
            }

            int seconds = std::stoi(rest.substr(0, thirdColon));
            std::string difficulty = rest.substr(thirdColon + 1);

            // Validate difficulty
            if (difficulty != "Easy" && difficulty != "Medium" && difficulty != "Hard") {
                return "ERROR: Invalid difficulty\n";
            }

            // Check if score qualifies for top 10
            int currentThreshold = getTop10Threshold(difficulty);
            int currentCount = getCountForDifficulty(difficulty);

            if (currentCount < 10 || seconds < currentThreshold) {
                // Add score to leaderboard
                ServerLeaderboardEntry entry{playerName, seconds, difficulty};
                addScore(entry);
                saveToFile();
                std::cout << "[SUBMIT] " << playerName << " - " << seconds
                          << " seconds on " << difficulty << " (ACCEPTED - Top "
                          << getRankForScore(difficulty, seconds) << ")" << std::endl;
                return "OK\n";
            } else {
                std::cout << "[SUBMIT] " << playerName << " - " << seconds
                          << " seconds on " << difficulty << " (REJECTED - Not top 10)" << std::endl;
                return "REJECTED\n";
            }
        }
        else if (cmdType == "GET") {
            // Parse: "GET:Difficulty"
            std::string difficulty = command.substr(firstColon + 1);

            // Validate difficulty
            if (difficulty != "Easy" && difficulty != "Medium" && difficulty != "Hard") {
                return "ERROR: Invalid difficulty\n";
            }

            std::string leaderboardData = getLeaderboardAsString(difficulty);
            std::cout << "[GET] Sent leaderboard for " << difficulty << std::endl;
            return leaderboardData;
        }

        return "ERROR: Unknown command\n";
    }

    // ======================== LEADERBOARD MANAGEMENT ========================

    std::string LeaderboardServer::getLeaderboardAsString(const std::string& difficulty) {
        std::lock_guard<std::mutex> lock(dataMutex);

        std::ostringstream response;
        int rank = 1;

        for (const auto& entry : leaderboard) {
            if (entry.difficulty == difficulty) {
                response << rank << "." << entry.playerName << ":" << entry.seconds << "\n";
                rank++;
                if (rank > 10) break;  // Strictly top 10 only
            }
        }

        // If no entries found
        if (rank == 1) {
            response << "No scores yet for " << difficulty << "\n";
        }

        return response.str();
    }

    void LeaderboardServer::addScore(const ServerLeaderboardEntry& entry) {
        std::lock_guard<std::mutex> lock(dataMutex);

        // Add new score
        leaderboard.push_back(entry);

        // Sort: by difficulty first, then by time (ascending - lower is better)
        sortLeaderboard();

        // Keep only top 10 per difficulty
        std::vector<ServerLeaderboardEntry> filtered;
        int easyCount = 0, mediumCount = 0, hardCount = 0;

        for (const auto& e : leaderboard) {
            if (e.difficulty == "Easy" && easyCount < 10) {
                filtered.push_back(e);
                easyCount++;
            }
            else if (e.difficulty == "Medium" && mediumCount < 10) {
                filtered.push_back(e);
                mediumCount++;
            }
            else if (e.difficulty == "Hard" && hardCount < 10) {
                filtered.push_back(e);
                hardCount++;
            }
        }

        leaderboard = filtered;
    }

    void LeaderboardServer::sortLeaderboard() {
        std::sort(leaderboard.begin(), leaderboard.end(),
            [](const ServerLeaderboardEntry& a, const ServerLeaderboardEntry& b) {
                // First group by difficulty
                if (a.difficulty != b.difficulty) {
                    return a.difficulty < b.difficulty;  // Alphabetical: Easy, Hard, Medium
                }
                // Within same difficulty, lower time = higher rank
                return a.seconds < b.seconds;
            });
    }

    int LeaderboardServer::getTop10Threshold(const std::string& difficulty) {
        std::lock_guard<std::mutex> lock(dataMutex);

        int count = 0;
        for (const auto& entry : leaderboard) {
            if (entry.difficulty == difficulty) {
                count++;
                if (count == 10) {
                    return entry.seconds;  // Return 10th place time
                }
            }
        }

        // If less than 10 scores exist, threshold is infinity (any score qualifies)
        return 9999;
    }

    int LeaderboardServer::getCountForDifficulty(const std::string& difficulty) {
        std::lock_guard<std::mutex> lock(dataMutex);

        int count = 0;
        for (const auto& entry : leaderboard) {
            if (entry.difficulty == difficulty) {
                count++;
            }
        }
        return count;
    }


    int LeaderboardServer::getRankForScore(const std::string& difficulty, int seconds) {
        std::lock_guard<std::mutex> lock(dataMutex);

        int rank = 1;
        for (const auto& entry : leaderboard) {
            if (entry.difficulty == difficulty) {
                if (seconds < entry.seconds) {
                    return rank;
                }
                rank++;
            }
        }
        if (rank < 11)
        {
            return rank;
        }
        return 11;  // 11 = not in top 10
    }

    // ======================== PERSISTENT STORAGE ========================

    void LeaderboardServer::saveToFile() {
        std::lock_guard<std::mutex> lock(dataMutex);

        std::ofstream file(dataFile);
        if (!file.is_open()) {
            std::cerr << "[ERROR] Failed to save leaderboard to " << dataFile << std::endl;
            return;
        }

        // Format: "PlayerName|Seconds|Difficulty"
        for (const auto& entry : leaderboard) {
            file << entry.playerName << "|"
                 << entry.seconds << "|"
                 << entry.difficulty << "\n";
        }

        file.close();
        std::cout << "[SAVE] Leaderboard saved to " << dataFile << " ("
                  << leaderboard.size() << " entries)" << std::endl;
    }

    void LeaderboardServer::loadFromFile() {
        std::lock_guard<std::mutex> lock(dataMutex);

        std::ifstream file(dataFile);
        if (!file.is_open()) {
            std::cout << "[INFO] No existing leaderboard file found. Starting fresh." << std::endl;
            return;
        }

        leaderboard.clear();
        std::string line;

        while (std::getline(file, line)) {
            // Parse format: "PlayerName|Seconds|Difficulty"
            size_t firstPipe = line.find('|');
            size_t secondPipe = line.find('|', firstPipe + 1);

            if (firstPipe != std::string::npos && secondPipe != std::string::npos) {
                ServerLeaderboardEntry entry;
                entry.playerName = line.substr(0, firstPipe);
                entry.seconds = std::stoi(line.substr(firstPipe + 1, secondPipe - firstPipe - 1));
                entry.difficulty = line.substr(secondPipe + 1);
                leaderboard.push_back(entry);
            }
        }

        file.close();

        // Ensure leaderboard is sorted and only top 10 per difficulty
        sortLeaderboard();

        std::cout << "[LOAD] Loaded " << leaderboard.size() << " scores from " << dataFile << std::endl;

        // Display loaded scores
        for (const auto& entry : leaderboard) {
            std::cout << "  - " << entry.playerName << ": "
                      << entry.seconds << " seconds (" << entry.difficulty << ")" << std::endl;
        }
    }

} // namespace Minesweeper

// ======================== MAIN FUNCTION ========================
// This allows the server to run as a standalone executable

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Minesweeper Leaderboard Server v1.0  " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    Minesweeper::LeaderboardServer server;

    // Start server on port 8080 (configurable)
    if (!server.start(8080)) {
        std::cerr << "[FATAL] Failed to start server" << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << "Server commands:" << std::endl;
    std::cout << "  - Press Ctrl+C to shutdown gracefully" << std::endl;
    std::cout << std::endl;

    // Run server (blocks until stopped)
    server.run();

    std::cout << "[SHUTDOWN] Server stopped" << std::endl;
    return 0;
}
