#include "../include/core/game.h"
#include <SFML/System/Clock.hpp>

Minesweeper::Game::Game()
{
    gameState = nullptr;
    renderer = nullptr;
    mouseHandler = nullptr;
    networkClient = nullptr;

    running = false;
    showLeaderboard = false;
}

bool Minesweeper::Game::initialize(int windowWidth, int windowHeight)
{
    gameState = std::make_unique<GameState>();
    renderer = std::make_unique<Renderer>();
    mouseHandler = std::make_unique<MouseHandler>();
    networkClient = std::make_unique<NetworkClient>();

    if (!renderer->initialize(windowWidth, windowHeight, WINDOW_TITLE))
    {
        return false;
    }

    gameState->startNewGame(Difficulty::EASY);

    // Optional: only works if your partner's server is running.
    // Change the port if your server uses a different one.
    networkClient->connect("127.0.0.1", 54000);

    running = true;

    return true;
}

void Minesweeper::Game::run()
{
    sf::Clock clock;

    while (running && renderer->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        processInput();
        update(deltaTime);
        render();
    }
}

void Minesweeper::Game::shutdown()
{
    running = false;

    if (renderer != nullptr)
    {
        renderer->close();
    }
}

void Minesweeper::Game::processInput()
{
    while (auto event = renderer->getWindow().pollEvent())
    {
        mouseHandler->update(*event);

        if (event->is<sf::Event::Closed>())
        {
            shutdown();
        }

        if (event->is<sf::Event::MouseButtonPressed>())
        {
            onMouseClick(*event);
        }

        if (event->is<sf::Event::KeyPressed>())
        {
            onKeyPress(*event);
        }

        if (event->is<sf::Event::Resized>())
        {
            onWindowResize(*event);
        }
    }
}

void Minesweeper::Game::update(float deltaTime)
{
    if (gameState != nullptr)
    {
        gameState->getTimer().tick();
    }

    if (mouseHandler != nullptr)
    {
        mouseHandler->update();
    }
}

void Minesweeper::Game::render()
{
    renderer->clear();

    if (gameState != nullptr)
    {
        renderer->renderBoard(gameState->getBoard());
    }

    if (showLeaderboard)
    {
        renderLeaderboard();
    }

    renderer->renderUI();
    renderer->display();
}

void Minesweeper::Game::onMouseClick(const sf::Event& event)
{
    const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();

    if (mouse == nullptr || gameState == nullptr)
    {
        return;
    }

    sf::Vector2i boardPosition = screenToBoardPosition(mouse->position);

    if (boardPosition.x == -1 || boardPosition.y == -1)
    {
        return;
    }

    if (mouse->button == sf::Mouse::Button::Left)
    {
        gameState->handleReveal(boardPosition.x, boardPosition.y);

        if (gameState->isVictory())
        {
            submitScoreToLeaderboard();
        }
    }
    else if (mouse->button == sf::Mouse::Button::Right)
    {
        gameState->handleFlag(boardPosition.x, boardPosition.y);
    }
}

void Minesweeper::Game::onKeyPress(const sf::Event& event)
{
    const auto* key = event.getIf<sf::Event::KeyPressed>();

    if (key == nullptr)
    {
        return;
    }

    if (key->code == sf::Keyboard::Key::Escape)
    {
        shutdown();
    }
    else if (key->code == sf::Keyboard::Key::R)
    {
        if (gameState != nullptr)
        {
            gameState->resetGame();
        }

        showLeaderboard = false;
    }
    else if (key->code == sf::Keyboard::Key::L)
    {
        showLeaderboardUI();
    }
}

void Minesweeper::Game::onWindowResize(const sf::Event& event)
{
    // Nothing needed for now.
}

void Minesweeper::Game::startNewGame(const Difficulty& difficulty)
{
    if (gameState != nullptr)
    {
        gameState->startNewGame(difficulty);
    }

    showLeaderboard = false;
}

void Minesweeper::Game::submitScoreToLeaderboard()
{
    if (networkClient == nullptr || gameState == nullptr)
    {
        return;
    }

    if (!networkClient->isConnected())
    {
        return;
    }

    networkClient->submitScore(
        "Player",
        gameState->getTimer().getSeconds(),
        getDifficultyName()
    );
}

void Minesweeper::Game::showLeaderboardUI()
{
    showLeaderboard = true;

    if (networkClient != nullptr && networkClient->isConnected())
    {
        currentLeaderboard = networkClient->getLeaderboard(getDifficultyName());
    }
}

void Minesweeper::Game::showMenu()
{
    showLeaderboard = false;
}

void Minesweeper::Game::setDifficulty(const Difficulty& difficulty)
{
    startNewGame(difficulty);
}

std::string Minesweeper::Game::getDifficultyName() const
{
    if (gameState == nullptr)
    {
        return "Unknown";
    }

    return gameState->getDifficulty().name;
}

sf::Vector2i Minesweeper::Game::screenToBoardPosition(const sf::Vector2i& screenPos) const
{
    if (gameState == nullptr || renderer == nullptr)
    {
        return sf::Vector2i(-1, -1);
    }

    int boardPixelWidth = gameState->getBoard().getWidth() * CELL_SIZE;
    int boardPixelHeight = gameState->getBoard().getHeight() * CELL_SIZE;

    int boardOffsetX = (static_cast<int>(renderer->getWindow().getSize().x) - boardPixelWidth) / 2;
    int boardOffsetY = (static_cast<int>(renderer->getWindow().getSize().y) - boardPixelHeight) / 2;

    int adjustedX = screenPos.x - boardOffsetX;
    int adjustedY = screenPos.y - boardOffsetY;

    if (adjustedX < 0 || adjustedY < 0)
    {
        return sf::Vector2i(-1, -1);
    }

    int boardX = adjustedX / CELL_SIZE;
    int boardY = adjustedY / CELL_SIZE;

    if (boardX < 0 || boardY < 0 ||
        boardX >= gameState->getBoard().getWidth() ||
        boardY >= gameState->getBoard().getHeight())
    {
        return sf::Vector2i(-1, -1);
    }

    return sf::Vector2i(boardX, boardY);
}

void Minesweeper::Game::renderLeaderboard()
{
    // Placeholder for now.
    // Later, draw currentLeaderboard using SFML text/UI.
}

void Minesweeper::Game::showNameInputDialog()
{
    // Placeholder for now.
    // Later, let the player enter their name instead of using "Player".
}