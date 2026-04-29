#pragma once
<<<<<<< HEAD

#include "gameState.h"
#include "../graphics/Renderer.h"
#include "../input/MouseHandler.h"
#include "../utils/Constants.h"
=======
#include "gameState.hpp"
#include "../graphics/renderer.h"
#include "../input/inputHandler.h"
#include "../input/mouseHandler.h"
#include "../network/networkClient.h"
#include <memory>
>>>>>>> 6e56a480248c3fb0b7683dfce1fc4123cd19d0a1

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
<<<<<<< HEAD
        std::unique_ptr<MouseHandler> mouseHandler;
=======
        std::unique_ptr<InputHandler> inputHandler;
        std::unique_ptr<MouseHandler> mouseHandler;
        std::unique_ptr<NetworkClient> networkClient;
>>>>>>> 6e56a480248c3fb0b7683dfce1fc4123cd19d0a1

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
<<<<<<< HEAD
        void showMenu();
=======
        void togglePause();
        void submitScoreToLeaderboard();
        void showLeaderboardUI();
>>>>>>> 6e56a480248c3fb0b7683dfce1fc4123cd19d0a1

        void setDifficulty(const Difficulty& difficulty);
        std::string getDifficultyName() const;
    private:
<<<<<<< HEAD
        void calculateFPS(float deltaTime);
=======
        void setupCallbacks();
>>>>>>> 6e56a480248c3fb0b7683dfce1fc4123cd19d0a1
        sf::Vector2i screenToBoardPosition(const sf::Vector2i& screenPos) const;
        void renderLeaderboard();
        void showNameInputDialog();
    };
<<<<<<< HEAD
}

#endif // PA9_GAME_H
=======
} // namespace Minesweeper
>>>>>>> 6e56a480248c3fb0b7683dfce1fc4123cd19d0a1
