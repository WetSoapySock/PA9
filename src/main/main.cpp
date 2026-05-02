#include "../include/core/game.h"
#include "../include/utils/constants.h"
#include "../include/core/board.h"
#include "../include/core/cell.h"
#include "../include/utils/randomGenerator.h"
#include "../include/utils/timer.h"
#include "../include/network/NetworkClient.h"
#include "../include/network/leaderboardProtocol.h"

#include <iostream>

using namespace Minesweeper;

/*
 Function: printTestResult
 Description: Helper function that prints the result of a test case.
              Displays test name and whether it passed or failed.
              Parameters: testName = description of test being run, passed = boolean result
*/
void printTestResult(const std::string& testName, bool passed) {
    std::cout << "Test case: " << testName << "\n" << (passed ? "Passed!" : "Failed!") << "\n";
}

/*
 Function: printBoard
 Description: Helper function that prints the current state of the game board
              to the console for debugging purposes. Shows flags (F), mines (*),
              numbers (1-8), empty cells (.), and covered cells (?).
              Parameters: board = reference to board to display
*/
void printBoard(Board& board) {
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            Cell* cell = board.getCell(x, y);
            if (cell->isFlagged()) {
                std::cout << " F ";
            } else if (cell->isRevealed()) {
                if (cell->isMine()) {
                    std::cout << " * ";
                } else {
                    int num = cell->getNumber();
                    std::cout << " " << (num == 0 ? '.' : char('0' + num)) << " ";
                }
            } else {
                std::cout << " ? ";
            }
        }
        std::cout << std::endl;
    }
}

/*
 Function: testBoardInitialization
 Description: Test case 1 - Verifies board is created with correct dimensions
              and mine count. Checks that all cells start covered, mines are
              placed correctly, and first click cell is safe.
*/
void testBoardInitialization() {
    std::cout << "\nTest 1: Board Initialization" << "\n";

    Board board(9, 9, 10);

    printTestResult("Board width = 9", board.getWidth() == 9);
    printTestResult("Board height = 9", board.getHeight() == 9);
    printTestResult("Total mines = 10", board.getTotalMines() == 10);
    printTestResult("Total cells = 81", board.getTotalCells() == 81);

    bool allCovered = true;
    for (int x = 0; x < board.getWidth(); x++) {
        for (int y = 0; y < board.getHeight(); y++) {
            if (!board.getCell(x, y)->isCovered()) {
                allCovered = false;
                break;
            }
        }
    }
    printTestResult("All cells start are COVERED", allCovered);

    board.placeMines(5, 5);
    board.calculateNumbers();

    int mineCount = 0;
    for (int x = 0; x < board.getWidth(); x++) {
        for (int y = 0; y < board.getHeight(); y++) {
            if (board.getCell(x, y)->isMine()) mineCount++;
        }
    }
    printTestResult("Correct number of mines placed", mineCount == 10);
    printTestResult("First click cell is safe", !board.getCell(5, 5)->isMine());
}

/*
 Function: testRevealMechanics
 Description: Test case 2 - Verifies cell reveal logic works correctly.
              Checks that revealing a safe cell returns false and that
              the cell becomes revealed after clicking.
*/
void testRevealMechanics() {
    std::cout << "\nTest 2: Reveal Mechanics" << "\n";

    Board board(9, 9, 10);
    board.placeMines(5, 5);
    board.calculateNumbers();

    bool hitMine = board.revealCell(5, 5);
    printTestResult("Reveal safe cell returns false (no mine)", !hitMine);
    printTestResult("Cell becomes REVEALED", board.getCell(5, 5)->isRevealed());
}

/*
 Function: testRandomGenerator
 Description: Test case 3 - Verifies random number generator produces
              integers within the specified range. Ensures getInt() returns
              values between min and max inclusive.
*/
void testRandomGenerator() {
    std::cout << "\nTest 3: Random Generator" << "\n";

    RandomGenerator rng;
    std::vector<int> intResults;
    for (int i = 0; i < 100; i++) {
        int val = rng.getInt(1, 10);
        intResults.push_back(val);
    }

    bool inRange = true;
    for (int val : intResults) {
        if (val < 1 || val > 10) {
            inRange = false;
            break;
        }
    }
    printTestResult("getInt(1,10) returns values in range", inRange);
}

/*
 Function: testNetworkProtocol
 Description: Test case 4 - Verifies network protocol formatting and parsing.
              Tests SUBMIT command format, GET command format, and leaderboard
              data parsing. Uses hardcoded data to avoid actual network calls.
              Gen AI assistance: Helped with arguments for testing network functionality.
*/
void testNetworkProtocol() {
    std::cout << "\nTest 4: Network Protocol" << "\n";

    std::string submitCmd = LeaderboardProtocol::formatSubmit("Alice", 45, "Easy");
    printTestResult("SUBMIT format: \"SUBMIT:Alice:45:Easy\\n\"", submitCmd == "SUBMIT:Alice:45:Easy\n");

    std::string getCmd = LeaderboardProtocol::formatGet("Medium");
    printTestResult("GET format: \"GET:Medium\\n\"", getCmd == "GET:Medium\n");

    std::string leaderboardData = "1.Alice:45\n2.Bob:67\n3.Carol:89\n";
    auto entries = LeaderboardProtocol::parseLeaderboard(leaderboardData, "Easy");

    printTestResult("Parsed 3 entries", entries.size() == 3);

    if (entries.size() >= 3) {
        printTestResult("Entry 1: Alice rank 1",
                        entries[0].playerName == "Alice" && entries[0].rank == 1 && entries[0].seconds == 45);
        printTestResult("Entry 2: Bob rank 2",
                        entries[1].playerName == "Bob" && entries[1].rank == 2 && entries[1].seconds == 67);
        printTestResult("Entry 3: Carol rank 3",
                        entries[2].playerName == "Carol" && entries[2].rank == 3 && entries[2].seconds == 89);
        printTestResult("All entries have difficulty 'Easy'",
                        entries[0].difficulty == "Easy" && entries[1].difficulty == "Easy" && entries[2].difficulty == "Easy");
    }
}

/*
 Function: testGameFlow
 Description: Test case 5 - Tests full game integration from start to victory.
              Verifies game starts in PLAYING state, timer starts on first click,
              and victory is detected after revealing all safe cells.
*/
void testGameFlow() {
    std::cout << "\nTest 5: Game Flow Integration" << "\n";

    GameState gameState;
    gameState.startNewGame(Difficulty::EASY);

    printTestResult("Game starts in PLAYING state", gameState.isPlaying());
    printTestResult("First move flag is true", gameState.isFirstMove());

    gameState.handleReveal(5, 5);
    printTestResult("After first click, timer started", gameState.getTimer().hasStarted());
    printTestResult("First move flag becomes false", !gameState.isFirstMove());

    Board& board = gameState.getBoard();
    int revealed = 0;
    
    for (int x = 0; x < board.getWidth(); x++) {
        for (int y = 0; y < board.getHeight(); y++) {
            if (!board.getCell(x, y)->isMine() && !board.getCell(x, y)->isRevealed()) {
                gameState.handleReveal(x, y);
                revealed++;
            }
        }
    }

    printTestResult("Victory detected after revealing all safe cells", gameState.isVictory());
    printTestResult("Timer stops on victory", !gameState.getTimer().isRunning());
}

/*
 Function: main
 Description: Entry point for the Minesweeper application. Runs all test cases
              to verify game functionality, then launches the full game with
              the main window. Handles game initialization and execution.
*/
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Minesweeper Test Suite v1.0         " << std::endl;
    std::cout << "========================================" << std::endl;

    testBoardInitialization();
    testRevealMechanics();
    testRandomGenerator();
    testNetworkProtocol();
    testGameFlow();

    std::cout << "\n========================================" << std::endl;
    std::cout << "           All Tests Complete           " << std::endl;
    std::cout << "========================================" << std::endl;

    Minesweeper::Game game;

    game.initialize(
        Minesweeper::MIN_WINDOW_WIDTH,
        Minesweeper::MIN_WINDOW_HEIGHT
    );

    game.run();

    return 0;
}
