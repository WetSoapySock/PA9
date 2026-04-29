#ifndef PA9_GAME_H
#define PA9_GAME_H

#pragma once

#include "gameState.h"
#include "../graphics/Renderer.h"
#include "../input/MouseHandler.h"
#include "../utils/Constants.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace Minesweeper
{
    class Game
    {
    private:
        std::unique_ptr<GameState> gameState;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<MouseHandler> mouseHandler;

        bool running;
        float lastTime;
        int fps;

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
        void onWindowResize(const sf::Event& event);

        void startNewGame(const Difficulty& difficulty);
        void showMenu();

        void setDifficulty(const Difficulty& difficulty);

    private:
        void calculateFPS(float deltaTime);
        sf::Vector2i screenToBoardPosition(const sf::Vector2i& screenPos) const;
    };
}

#endif // PA9_GAME_H