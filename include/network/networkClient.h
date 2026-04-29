#ifndef PA9_NETWORKCLIENT_H
#define PA9_NETWORKCLIENT_H

#pragma once
// AI Generated libraries
#include <winsock2.h>   // core networking functionality
#include <ws2tcpip.h>   // TCP/IP and socket helper functions
#include <string>
#include "leaderboardProtocol.hpp"

#pragma comment(lib, "ws2_32.lib") // Tells the linker to include Winsock 2 library

/** Gen AI, What is a network socket and how can we use them to create a
    NetworkClient class for our Minesweeper game that can submit
    scores and retrieve leaderboards from a server? */

namespace Minesweeper {
     // Gen AI, Handles TCP socket communication with the leaderboard server.
    class NetworkClient {
    private:
        // Gen AI, Socket handle for network communication
        SOCKET socket;
        bool connected;
    public:
        /** Gen AI, 1. socket = INVALID_SOCKET (no connection yet)
                    * 2. connected = false
                    * 3. WSAStartup() initializes Windows sockets (required!) */
        NetworkClient() : socket(INVALID_SOCKET), connected(false) {
           // Gen AI, Windows sockets for network
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);
        }

         // Gen AI, cleans up network resources
        ~NetworkClient() {
            disconnect();   // Close socket if open
            WSACleanup();   // Cleanup Winsock (opposite of WSAStartup)
        }
        
        // Gen AI, Establishes TCP connection to the server for leaderboard.
        bool connect(const std::string& serverIP, int port) {
           // Gen AI, Creates TCP socket
            socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (socket == INVALID_SOCKET) return false;

           // Gen AI, Configures server address structure
            sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;  // IPv4

            // Gen AI, Converts IP to binary
            serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());

            // Gen AI, host to network
            serverAddr.sin_port = htons(port);

            // Gen AI, Establishes TCP connection
            if (::connect(socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                closesocket(socket);  // Clean up failed socket
                return false;
            }
            connected = true;
            return true;
        }

        // Gen AI, Closes the socket connection and disconnects from network
        void disconnect() {
            if (connected) {
                closesocket(socket);   // Close the socket
                connected = false;      // Update state
            }
        }
        
        // Gen AI, Submits scores to leaderboard
        SubmitResult submitScore(const std::string& playerName, int seconds, const std::string& difficulty) {
            if (!connected) return SubmitResult::CONNECTION_FAILED; // Checks connection state
            if (seconds < 0 || seconds > 999) return SubmitResult::INVALID_SCORE; // If > 999 seconds, invalidates score

            // Gen AI, Format and send command
            std::string command = LeaderboardProtocol::formatSubmit(playerName, seconds, difficulty);

            // Gen AI, Sends data to server
            send(socket, command.c_str(), command.length(), 0);

            // Gen AI, Receives server response
            char buffer[256];
            int received = recv(socket, buffer, sizeof(buffer) - 1, 0);

            if (received > 0) {
                buffer[received] = '\0';  // Null-terminate string
                if (LeaderboardProtocol::parseSubmitResponse(buffer)) return SubmitResult::ACCEPTED_TOP_10;
            }
            return SubmitResult::REJECTED_NOT_TOP_10;
        }
        
         // Gen AI, Fetches top 10 scores for specified difficulty.
        std::vector<LeaderboardEntry> getLeaderboard(const std::string& difficulty) {
            // Check connection state
            if (!connected) return {};

            // Format and send GET command
            std::string command = LeaderboardProtocol::formatGet(difficulty);
            send(socket, command.c_str(), command.length(), 0);

            // Gen AI, fetch and retreive leaderboard stats
            char buffer[4096];
            int received = recv(socket, buffer, sizeof(buffer) - 1, 0);

            if (received > 0) {
                buffer[received] = '\0';
                // Parse response into vector of LeaderboardEntry objects
                return LeaderboardProtocol::parseLeaderboard(buffer, difficulty);
            }
            return {};  // Empty vector on failure
        }
        bool isConnected() const { return connected; }
    };

} // namespace Minesweeper

#endif //PA9_NETWORKCLIENT_H
