#pragma once
#include "gameState.hpp"
#include "../graphics/renderer.h"
#include "../input/inputHandler.h"
#include "../input/mouseHandler.h"
#include "../network/networkClient.h"
#include <memory>

namespace Minesweeper {
    class Game {
    private:
        std::unique_ptr<GameState> gameState;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<InputHandler> inputHandler;
        std::unique_ptr<MouseHandler> mouseHandler;
        std::unique_ptr<NetworkClient> networkClient;

        bool running;
        bool showLeaderboard;
        std::vector<LeaderboardEntry> currentLeaderboard;
    public:
        Game();
        ~Game() = default;

        bool initialize(int windowWidth, int windowHeight);
        void run();
        void shutdown();

        void processInput();
        void update(float deltaTime);
        void render();

        void onMouseClick(const sf::Event& event);
        void onKeyPress(const sf::Event& event);

        void startNewGame(const Difficulty& difficulty);
        void togglePause();
        void submitScoreToLeaderboard();
        void showLeaderboardUI();

        void setDifficulty(const Difficulty& difficulty);
        std::string getDifficultyName() const;
    private:
        void setupCallbacks();
        sf::Vector2i screenToBoardPosition(const sf::Vector2i& screenPos) const;
        void renderLeaderboard();
        void showNameInputDialog();
    };
} // namespace Minesweeper
