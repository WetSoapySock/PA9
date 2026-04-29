#include "../include/core/game.h"

Minesweeper::Game::Game()
{
    gameState = nullptr;
    renderer = nullptr;
    mouseHandler = nullptr;

    running = false;
    lastTime = 0.0f;
    fps = 0;
}

bool Minesweeper::Game::initialize(int windowWidth, int windowHeight)
{
    gameState = std::make_unique<GameState>();
    renderer = std::make_unique<Renderer>();
    mouseHandler = std::make_unique<MouseHandler>();

    if (!renderer->initialize(windowWidth, windowHeight, WINDOW_TITLE))
    {
        return false;
    }

    gameState->startNewGame(Difficulty::EASY);

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
    calculateFPS(deltaTime);

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
    renderer->renderBoard(gameState->getBoard());
    renderer->renderUI();
    renderer->display();
}

void Minesweeper::Game::onMouseClick(const sf::Event& event)
{
    const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();

    if (mouse == nullptr)
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

    if (key->code == sf::Keyboard::Key::R)
    {
        gameState->resetGame();
    }
}

void Minesweeper::Game::onWindowResize(const sf::Event& event)
{
    // Nothing needed for now
}

void Minesweeper::Game::startNewGame(const Difficulty& difficulty)
{
    gameState->startNewGame(difficulty);
}

void Minesweeper::Game::showMenu()
{
    // Nothing needed yet
}

void Minesweeper::Game::setDifficulty(const Difficulty& difficulty)
{
    startNewGame(difficulty);
}

void Minesweeper::Game::calculateFPS(float deltaTime)
{
    if (deltaTime > 0.0f)
    {
        fps = static_cast<int>(1.0f / deltaTime);
    }
}

sf::Vector2i Minesweeper::Game::screenToBoardPosition(const sf::Vector2i& screenPos) const
{
    int boardPixelWidth = gameState->getBoard().getWidth() * CELL_SIZE;
    int boardPixelHeight = gameState->getBoard().getHeight() * CELL_SIZE;

    int boardOffsetX = (renderer->getWindow().getSize().x - boardPixelWidth) / 2;
    int boardOffsetY = (renderer->getWindow().getSize().y - boardPixelHeight) / 2;

    int boardX = (screenPos.x - boardOffsetX) / CELL_SIZE;
    int boardY = (screenPos.y - boardOffsetY) / CELL_SIZE;

    if (boardX < 0 || boardY < 0 ||
        boardX >= gameState->getBoard().getWidth() ||
        boardY >= gameState->getBoard().getHeight())
    {
        return sf::Vector2i(-1, -1);
    }

    return sf::Vector2i(boardX, boardY);
}