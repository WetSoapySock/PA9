#include "../include/core/game.h"
#include <SFML/System/Clock.hpp>
#include <iostream>

/*
 Function: Game
 Description: Constructor. Initializes pointers, flags, selected difficulty,
              player name, and starts the game on the main menu screen.
*/
Minesweeper::Game::Game()
{
    gameState = nullptr;
    renderer = nullptr;
    mouseHandler = nullptr;
    networkClient = nullptr;

    running = false;
    showLeaderboard = false;
    timerAccumulator = 0.0f;

    currentScreen = ScreenState::MAIN_MENU;
    playerName = "Player";
    selectedDifficulty = Difficulty::EASY;
}

/*
 Function: initialize
 Description: Creates the main game systems and opens the SFML window.
              Also creates the NetworkClient, but leaderboard submission uses
              temporary clients later to avoid socket reuse issues.
*/
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

    selectedDifficulty = Difficulty::EASY;

    running = true;
    return true;
}

/*
 Function: run
 Description: Main game loop. Runs until the window closes or shutdown is called.
*/
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

/*
 Function: shutdown
 Description: Stops the game and closes the SFML window.
*/
void Minesweeper::Game::shutdown()
{
    running = false;

    if (renderer != nullptr)
    {
        renderer->close();
    }
}

/*
 Function: processInput
 Description: Reads all SFML events and sends them to the correct handler.
*/
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

        if (event->is<sf::Event::TextEntered>())
        {
            onTextEntered(*event);
        }

        if (event->is<sf::Event::Resized>())
        {
            onWindowResize(*event);
        }
    }
}

/*
 Function: update
 Description: Updates the game timer while the game is active.
*/
void Minesweeper::Game::update(float deltaTime)
{
    if (gameState != nullptr &&
        gameState->isPlaying() &&
        gameState->getTimer().isRunning())
    {
        timerAccumulator += deltaTime;

        if (timerAccumulator >= 1.0f)
        {
            gameState->getTimer().tick();
            timerAccumulator = 0.0f;
        }
    }

    if (mouseHandler != nullptr)
    {
        mouseHandler->update();
    }
}

/*
 Function: render
 Description: Draws the correct screen based on currentScreen.
*/
void Minesweeper::Game::render()
{
    renderer->clear();

    switch (currentScreen)
    {
    case ScreenState::MAIN_MENU:
        renderMenu();
        break;

    case ScreenState::HOW_TO_PLAY:
        renderHowToPlay();
        break;

    case ScreenState::DIFFICULTY_SELECT:
        renderDifficultySelect();
        break;

    case ScreenState::LEADERBOARD:
        renderLeaderboard();
        break;

    case ScreenState::NAME_INPUT:
        renderNameInput();
        break;

    case ScreenState::PLAYING:
        renderer->renderBoard(gameState->getBoard());
        renderTimer();
        renderMineCounter();
        break;

    case ScreenState::GAME_OVER:
        renderGameOver();
        break;

    case ScreenState::VICTORY:
        renderVictory();
        break;
    }

    renderer->display();
}

/*
 Function: onMouseClick
 Description: Handles left and right clicks during gameplay only.
*/
void Minesweeper::Game::onMouseClick(const sf::Event& event)
{
    const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();

    if (mouse == nullptr || gameState == nullptr)
    {
        return;
    }

    if (currentScreen != ScreenState::PLAYING)
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
            currentScreen = ScreenState::VICTORY;
        }
        else if (gameState->isGameOver())
        {
            currentScreen = ScreenState::GAME_OVER;
        }
    }
    else if (mouse->button == sf::Mouse::Button::Right)
    {
        gameState->handleFlag(boardPosition.x, boardPosition.y);
    }
}

/*
 Function: onKeyPress
 Description: Handles keyboard navigation for menus and gameplay.
*/
void Minesweeper::Game::onKeyPress(const sf::Event& event)
{
    const auto* key = event.getIf<sf::Event::KeyPressed>();

    if (key == nullptr)
    {
        return;
    }

    // Prevent menu shortcuts while typing a name.
    if (currentScreen == ScreenState::NAME_INPUT)
    {
        if (key->code == sf::Keyboard::Key::Enter)
        {
            if (playerName.empty())
            {
                playerName = "Player";
            }

            currentScreen = ScreenState::MAIN_MENU;
        }
        else if (key->code == sf::Keyboard::Key::Escape)
        {
            currentScreen = ScreenState::MAIN_MENU;
        }

        return;
    }

    if (key->code == sf::Keyboard::Key::Escape)
    {
        shutdown();
    }
    else if (key->code == sf::Keyboard::Key::M)
    {
        currentScreen = ScreenState::MAIN_MENU;
    }
    else if (key->code == sf::Keyboard::Key::Enter)
    {
        if (currentScreen == ScreenState::MAIN_MENU)
        {
            startNewGame(selectedDifficulty);
            currentScreen = ScreenState::PLAYING;
        }
    }
    else if (key->code == sf::Keyboard::Key::H)
    {
        currentScreen = ScreenState::HOW_TO_PLAY;
    }
    else if (key->code == sf::Keyboard::Key::D)
    {
        currentScreen = ScreenState::DIFFICULTY_SELECT;
    }
    else if (key->code == sf::Keyboard::Key::N)
    {
        currentScreen = ScreenState::NAME_INPUT;
    }
    else if (key->code == sf::Keyboard::Key::L)
    {
        showLeaderboardUI();
        currentScreen = ScreenState::LEADERBOARD;
    }
    else if (key->code == sf::Keyboard::Key::R)
    {
        startNewGame(selectedDifficulty);
        currentScreen = ScreenState::PLAYING;
    }

    if (currentScreen == ScreenState::DIFFICULTY_SELECT)
    {
        if (key->code == sf::Keyboard::Key::Num1)
        {
            selectedDifficulty = Difficulty::EASY;
        }
        else if (key->code == sf::Keyboard::Key::Num2)
        {
            selectedDifficulty = Difficulty::MEDIUM;
        }
        else if (key->code == sf::Keyboard::Key::Num3)
        {
            selectedDifficulty = Difficulty::HARD;
        }
    }
}

/*
 Function: onTextEntered
 Description: Allows the user to type their player name on the NAME_INPUT screen.
*/
void Minesweeper::Game::onTextEntered(const sf::Event& event)
{
    const auto* textEvent = event.getIf<sf::Event::TextEntered>();

    if (textEvent == nullptr)
    {
        return;
    }

    if (currentScreen != ScreenState::NAME_INPUT)
    {
        return;
    }

    if (textEvent->unicode == 8)
    {
        if (!playerName.empty())
        {
            playerName.pop_back();
        }
    }
    else if (textEvent->unicode == 13)
    {
        if (playerName.empty())
        {
            playerName = "Player";
        }

        currentScreen = ScreenState::MAIN_MENU;
    }
    else if (textEvent->unicode >= 32 && textEvent->unicode < 128)
    {
        if (playerName.length() < 12)
        {
            playerName += static_cast<char>(textEvent->unicode);
        }
    }
}

/*
 Function: onWindowResize
 Description: Placeholder for future window resize logic.
*/
void Minesweeper::Game::onWindowResize(const sf::Event& event)
{
    // No resize logic needed right now.
}

/*
 Function: startNewGame
 Description: Starts a fresh game using the selected difficulty.
*/
void Minesweeper::Game::startNewGame(const Difficulty& difficulty)
{
    if (gameState != nullptr)
    {
        gameState->startNewGame(difficulty);
    }

    timerAccumulator = 0.0f;
    showLeaderboard = false;
}

/*
 Function: submitScoreToLeaderboard
 Description: Sends the player's score to the server after winning.
              Uses a temporary NetworkClient because the server closes each
              connection after one request.
*/
void Minesweeper::Game::submitScoreToLeaderboard()
{
    if (gameState == nullptr)
    {
        return;
    }

    NetworkClient tempClient;

    if (!tempClient.connect("127.0.0.1", 8080))
    {
        return;
    }

    tempClient.submitScore(
        playerName,
        gameState->getTimer().getSeconds(),
        getDifficultyName()
    );
}

/*
 Function: showLeaderboardUI
 Description: Fetches leaderboard data from the server for the selected difficulty.
*/
void Minesweeper::Game::showLeaderboardUI()
{
    showLeaderboard = true;

    NetworkClient tempClient;

    if (tempClient.connect("127.0.0.1", 8080))
    {
        currentLeaderboard = tempClient.getLeaderboard(getDifficultyName());
    }
}

/*
 Function: showMenu
 Description: Returns to the main menu screen.
*/
void Minesweeper::Game::showMenu()
{
    showLeaderboard = false;
    currentScreen = ScreenState::MAIN_MENU;
}

/*
 Function: setDifficulty
 Description: Updates the selected difficulty without starting a game immediately.
*/
void Minesweeper::Game::setDifficulty(const Difficulty& difficulty)
{
    selectedDifficulty = difficulty;
}

/*
 Function: getDifficultyName
 Description: Returns the current selected difficulty name.
*/
std::string Minesweeper::Game::getDifficultyName() const
{
    return selectedDifficulty.name;
}

/*
 Function: screenToBoardPosition
 Description: Converts mouse screen coordinates into board cell coordinates.
*/
sf::Vector2i Minesweeper::Game::screenToBoardPosition(const sf::Vector2i& screenPos) const
{
    if (gameState == nullptr || renderer == nullptr)
    {
        return sf::Vector2i(-1, -1);
    }

    int boardPixelWidth = gameState->getBoard().getWidth() * CELL_SIZE;
    int boardPixelHeight = gameState->getBoard().getHeight() * CELL_SIZE;

    int boardOffsetX =
        (static_cast<int>(renderer->getWindow().getSize().x) - boardPixelWidth) / 2;

    int boardOffsetY =
        (static_cast<int>(renderer->getWindow().getSize().y) - boardPixelHeight) / 2;

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

/*
 Function: renderTimer
 Description: Draws the elapsed timer on the screen.
*/
void Minesweeper::Game::renderTimer()
{
    sf::Text text(renderer->getFont(),
        "Time: " + std::to_string(gameState->getTimer().getSeconds()), 24);

    text.setFillColor(sf::Color::White);
    text.setPosition({ 20, 20 });

    renderer->getWindow().draw(text);
}

/*
 Function: renderMineCounter
 Description: Draws remaining mines based on total mines minus flags.
*/
void Minesweeper::Game::renderMineCounter()
{
    sf::Text text(renderer->getFont(),
        "Mines: " + std::to_string(gameState->getBoard().getRemainingMines()), 24);

    text.setFillColor(sf::Color::White);
    text.setPosition({ 20, 55 });

    renderer->getWindow().draw(text);
}

/*
 Function: renderMenu
 Description: Draws the main menu screen.
*/
void Minesweeper::Game::renderMenu()
{
    sf::RenderWindow& window = renderer->getWindow();

    sf::Text title(renderer->getFont(), "MINESWEEPER", 44);
    title.setFillColor(sf::Color::White);
    title.setPosition({ 250, 70 });
    window.draw(title);

    sf::Text menu(renderer->getFont(),
        "Enter - Play\n"
        "H - How To Play\n"
        "D - Difficulty Select\n"
        "N - Enter Player Name\n"
        "L - Leaderboard\n"
        "Esc - Quit",
        24);

    menu.setFillColor(sf::Color::White);
    menu.setPosition({ 260, 160 });
    window.draw(menu);

    sf::Text info(renderer->getFont(),
        "Player: " + playerName +
        "\nDifficulty: " + selectedDifficulty.name,
        22);

    info.setFillColor(sf::Color::Yellow);
    info.setPosition({ 260, 390 });
    window.draw(info);
}

/*
 Function: renderHowToPlay
 Description: Draws instructions for playing Minesweeper.
*/
void Minesweeper::Game::renderHowToPlay()
{
    sf::Text text(renderer->getFont(),
        "HOW TO PLAY\n\n"
        "Left Click: Reveal a cell\n"
        "Right Click: Place/remove a flag\n"
        "Avoid mines.\n"
        "Reveal all safe cells to win.\n\n"
        "Press M to return to menu",
        24);

    text.setFillColor(sf::Color::White);
    text.setPosition({ 180, 100 });

    renderer->getWindow().draw(text);
}

/*
 Function: renderDifficultySelect
 Description: Draws the difficulty selection screen.
*/
void Minesweeper::Game::renderDifficultySelect()
{
    sf::Text text(renderer->getFont(),
        "DIFFICULTY SELECT\n\n"
        "1 - Easy: 9 x 9, 10 mines\n"
        "2 - Medium: 16 x 16, 40 mines\n"
        "3 - Hard: 30 x 16, 99 mines\n\n"
        "Current Difficulty: " + selectedDifficulty.name + "\n\n"
        "Press M to return to menu",
        24);

    text.setFillColor(sf::Color::White);
    text.setPosition({ 160, 100 });

    renderer->getWindow().draw(text);
}

/*
 Function: renderNameInput
 Description: Draws the name entry screen.
*/
void Minesweeper::Game::renderNameInput()
{
    sf::Text text(renderer->getFont(),
        "ENTER PLAYER NAME\n\n"
        "Name: " + playerName + "\n\n"
        "Type your name.\n"
        "Backspace deletes letters.\n"
        "Enter saves name.\n\n"
        "Press M to return to menu",
        24);

    text.setFillColor(sf::Color::White);
    text.setPosition({ 180, 100 });

    renderer->getWindow().draw(text);
}

/*
 Function: renderLeaderboard
 Description: Draws leaderboard entries if any are loaded.
*/
void Minesweeper::Game::renderLeaderboard()
{
    sf::RenderWindow& window = renderer->getWindow();

    sf::Text title(renderer->getFont(), "LEADERBOARD", 38);
    title.setFillColor(sf::Color::White);
    title.setPosition({ 260, 60 });
    window.draw(title);

    sf::Text back(renderer->getFont(), "Press M to return to menu", 20);
    back.setFillColor(sf::Color::Yellow);
    back.setPosition({ 260, 110 });
    window.draw(back);

    if (currentLeaderboard.empty())
    {
        sf::Text empty(renderer->getFont(), "No scores found.", 24);
        empty.setFillColor(sf::Color::White);
        empty.setPosition({ 260, 170 });
        window.draw(empty);
        return;
    }

    int y = 160;

    for (int i = 0; i < currentLeaderboard.size(); i++)
    {
        const LeaderboardEntry& entry = currentLeaderboard[i];

        std::string line =
            std::to_string(entry.rank) + ". " +
            entry.playerName + " - " +
            std::to_string(entry.seconds) + " sec";

        sf::Text score(renderer->getFont(), line, 22);
        score.setFillColor(sf::Color::White);
        score.setPosition({ 260, static_cast<float>(y) });

        window.draw(score);
        y += 35;
    }
}

/*
 Function: renderGameOver
 Description: Draws the game-over screen over the board.
*/
void Minesweeper::Game::renderGameOver()
{
    renderer->renderBoard(gameState->getBoard());

    sf::Text text(renderer->getFont(),
        "GAME OVER\n\n"
        "Press R to restart\n"
        "Press M for menu",
        32);

    text.setFillColor(sf::Color::Black);
    text.setPosition({ 250, 80 });

    renderer->getWindow().draw(text);
}

/*
 Function: renderVictory
 Description: Draws the victory screen over the board.
*/
void Minesweeper::Game::renderVictory()
{
    renderer->renderBoard(gameState->getBoard());

    sf::Text text(renderer->getFont(),
        "YOU WIN!\n\n"
        "Time: " + std::to_string(gameState->getTimer().getSeconds()) + " sec\n"
        "Press R to restart\n"
        "Press M for menu",
        32);

    text.setFillColor(sf::Color::Green);
    text.setPosition({ 250, 80 });

    renderer->getWindow().draw(text);
}