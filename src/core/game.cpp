#include "../include/core/game.h"
#include <SFML/System/Clock.hpp>


/*
 Function: Game
 Description: Constructor. Sets all pointers to nullptr and initializes
              the starting screen, player name, selected difficulty, and flags.
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
 Description: Creates the main game objects and initializes the renderer/window.
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

    // Start with Easy selected, but stay on the main menu.
    selectedDifficulty = Difficulty::EASY;

    // Optional leaderboard server connection.
    networkClient->connect("127.0.0.1", 54000);

    running = true;
    return true;
}

/*
 Function: run
 Description: Main game loop. Processes input, updates logic, and renders.
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
 Description: Stops the game loop and closes the window.
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
 Description: Reads SFML events and sends them to the correct handler.
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

        // Needed for typing the player name.
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
 Description: Updates the game timer once per second while the game is active.
*/
void Minesweeper::Game::update(float deltaTime)
{
    if (gameState != nullptr &&
        gameState->isPlaying() &&
        gameState->getTimer().isRunning())
    {
        timerAccumulator += deltaTime;

        // Timer only ticks once every full second.
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
 Description: Draws a different screen depending on currentScreen.
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
 Description: Handles board clicks only while the game is being played.
*/
void Minesweeper::Game::onMouseClick(const sf::Event& event)
{
    const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();

    if (mouse == nullptr || gameState == nullptr)
    {
        return;
    }

    // Do not let menu clicks affect the board.
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
 Description: Handles keyboard controls for menu navigation and gameplay.
*/
void Minesweeper::Game::onKeyPress(const sf::Event& event)
{
    const auto* key = event.getIf<sf::Event::KeyPressed>();

    if (key == nullptr)
    {
        return;
    }
    if (currentScreen == ScreenState::NAME_INPUT)
    {
        const auto* key = event.getIf<sf::Event::KeyPressed>();

        if (key != nullptr)
        {
            // Allow only Enter and Escape while typing name
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

    // Difficulty screen controls.
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
 Description: Lets the player type their name on the NAME_INPUT screen.
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

    if (textEvent->unicode == 8) // Backspace
    {
        if (!playerName.empty())
        {
            playerName.pop_back();
        }
    }
    else if (textEvent->unicode == 13) // Enter
    {
        if (playerName.empty())
        {
            playerName = "Player";
        }

        currentScreen = ScreenState::MAIN_MENU;
    }
    else if (textEvent->unicode >= 32 && textEvent->unicode < 128)
    {
        // Limit name length.
        if (playerName.length() < 12)
        {
            playerName += static_cast<char>(textEvent->unicode);
        }
    }
}

/*
 Function: onWindowResize
 Description: Placeholder for future resize logic.
*/
void Minesweeper::Game::onWindowResize(const sf::Event& event)
{
    // No resize logic needed right now.
}

/*
 Function: startNewGame
 Description: Starts a new game using the selected difficulty.
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
 Description: Sends the player's score to the server after a victory.
*/
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
        playerName,
        gameState->getTimer().getSeconds(),
        getDifficultyName()
    );
}

/*
 Function: showLeaderboardUI
 Description: Gets leaderboard data from the server if connected.
*/
void Minesweeper::Game::showLeaderboardUI()
{
    showLeaderboard = true;

    if (networkClient != nullptr && networkClient->isConnected())
    {
        currentLeaderboard = networkClient->getLeaderboard(getDifficultyName());
    }
}

/*
 Function: showMenu
 Description: Returns to the main menu.
*/
void Minesweeper::Game::showMenu()
{
    showLeaderboard = false;
    currentScreen = ScreenState::MAIN_MENU;
}

/*
 Function: setDifficulty
 Description: Updates selected difficulty without immediately starting the game.
*/
void Minesweeper::Game::setDifficulty(const Difficulty& difficulty)
{
    selectedDifficulty = difficulty;
}

/*
 Function: getDifficultyName
 Description: Returns the currently selected difficulty name.
*/
std::string Minesweeper::Game::getDifficultyName() const
{
    return selectedDifficulty.name;
}

/*
 Function: screenToBoardPosition
 Description: Converts mouse screen coordinates to board cell coordinates.
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
 Description: Displays the elapsed game time.
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
 Description: Displays remaining mines based on total mines minus flags.
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
 Description: Draws the player name entry screen.
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
 Description: Draws leaderboard entries if available.
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