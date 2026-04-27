#ifndef PA9_GAME_H
#define PA9_GAME_H

#pragma once
#include "GameState.h"
#include "../graphics/Renderer.h"
#include "../input/InputHandler.h"
#include "../utils/Constants.h"

namespace Minesweeper {
    class Game {
    private:
        std::unique_ptr<GameState> gameState;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<InputHandler> inputHandler;

        bool running;
        float lastTime;
        int fps;

    public:
        Game();
        ~Game() = default;

        // Lifecycle methods
        bool initialize(int windowWidth, int windowHeight);
        void run();
        void shutdown();

        // Game loop
        void processInput();
        void update(float deltaTime);
        void render();

        // Event handlers
        void onMouseClick(const sf::Event& event);
        void onKeyPress(const sf::Event& event);
        void onWindowResize(const sf::Event& event);

        // Game actions
        void startNewGame(const Difficulty& difficulty);
        void togglePause();
        void showMenu();

        // Settings
        void setDifficulty(const Difficulty& difficulty);

    private:
        void setupCallbacks();
        void calculateFPS(float deltaTime);
        sf::Vector2i screenToBoardPosition(const sf::Vector2i& screenPos) const;
    };
};
#endif //PA9_GAME_H
