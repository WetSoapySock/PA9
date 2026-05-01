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

// Prints results from test cases
void printTestResult(const std::string& testName, bool passed) {std::cout << "Test case: " << testName << "\n" << (passed ? "Passed!" : "Failed!") << "\n";}

// Helper to print board for testing
void printBoard(Board& board) {
    // Nested loop to iterate through game board
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            Cell* cell = board.getCell(x, y);
            if (cell->isFlagged()) {    // Checks if cell is flagged and if so, mark it
                std::cout << " F ";
            } else if (cell->isRevealed()) {    // Checks if the cell is revealed then check if it is safe or a mine
                if (cell->isMine()) {
                    std::cout << " * ";  // Mine marked with asterisk
                } else {
                    int num = cell->getNumber();    // Prints cell number, if 0, prints a dot for empty space
                    std::cout << " " << (num == 0 ? '.' : char('0' + num)) << " ";
                }
            } else {
                std::cout << " ? ";     // Unknown cell
            }
        }
        std::cout << std::endl;
    }
}

// Test case 1: Tests board initialization and mine placement to verify if board creates correct dimensions and mine count
void testBoardInitialization() {
    std::cout << "\nTest 1: Board Initialization" << "\n";

    Board board(9, 9, 10);      // Create an easy board with 10 mines

    // Tests for proper dimensions
    printTestResult("Board width = 9", board.getWidth() == 9);
    printTestResult("Board height = 9", board.getHeight() == 9);
    printTestResult("Total mines = 10", board.getTotalMines() == 10);
    printTestResult("Total cells = 81", board.getTotalCells() == 81);

    // Tests if all cells are covered
    bool allCovered = true;
    // Nested loop to itterate through entire board
    for (int x = 0; x < board.getWidth(); x++) {
        for (int y = 0; y < board.getHeight(); y++) {
            if (!board.getCell(x, y)->isCovered()) {
                allCovered = false;
                break;
            }
        }
    }
    printTestResult("All cells start are COVERED", allCovered);

    // Checks if mines are placed on first click and if they were plaed properly
    board.placeMines(5, 5);  // Gen AI: Safe cell at (5,5)
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

// Test case 2: Tests cell revealing and flood fill mechanics
void testRevealMechanics() {
    std::cout << "\nTest 2: Reveal Mechanics" << "\n";

    Board board(9, 9, 10);
    board.placeMines(5, 5);
    board.calculateNumbers();

    bool hitMine = board.revealCell(5, 5);
    printTestResult("Reveal safe cell returns false (no mine)", !hitMine);
    printTestResult("Cell becomes REVEALED", board.getCell(5, 5)->isRevealed());

}

// Test case 3: Tests random number generator
void testRandomGenerator() {
    std::cout << "\nTest 2: Random Generator" << "\n";

    // Tests integer range [1, 10]
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

// Test Case 4: Tests network protocol formatting and parsing to verify leaderboard protocol commands
/** Gen AI, "I'm struggling with the arguments to test the network functionality. Can you help me
    send a leaderboard score submission, fetch a score, and parse the data?" */
void testNetworkProtocol() {
    std::cout << "\nTest 4: Network Protocol" << "\n";

    // Submit testing
    std::string submitCmd = LeaderboardProtocol::formatSubmit("Alice", 45, "Easy");
    printTestResult("SUBMIT format: \"SUBMIT:Alice:45:Easy\\n\"", submitCmd == "SUBMIT:Alice:45:Easy\n");

    // Fetch testing
    std::string getCmd = LeaderboardProtocol::formatGet("Medium");
    printTestResult("GET format: \"GET:Medium\\n\"",getCmd == "GET:Medium\n");

    // Parse testing
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

// Test 5: Tests for full game integration
void testGameFlow() {
    std::cout << "\nTest 5: Game Flow Integration" << "\n";

    // Create a new game state for testing
    GameState gameState;
    gameState.startNewGame(Difficulty::EASY);

    printTestResult("Game starts in PLAYING state", gameState.isPlaying());
    printTestResult("First move flag is true", gameState.isFirstMove());

    // Simulates the first click at (5,5)
    gameState.handleReveal(5, 5);
    printTestResult("After first click, timer started", gameState.getTimer().hasStarted());
    printTestResult("First move flag becomes false", !gameState.isFirstMove());

    // "Solves the game," but not really because it's all in the back end.
    Board& board = gameState.getBoard();
    int revealed = 0;
    
    for (int x = 0; x < board.getWidth(); x++) {       // Nested loop to click all safe cells
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

// ======================== MAIN ========================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Minesweeper Test Suite v1.0         " << std::endl;
    std::cout << "========================================" << std::endl;

    // Gen AI: Run all test cases
    testBoardInitialization();
    testRevealMechanics();

    testRandomGenerator();
    testNetworkProtocol();
    testGameFlow();

    std::cout << "\n========================================" << std::endl;
    std::cout << "           All Tests Complete           " << std::endl;
    std::cout << "========================================" << std::endl;

    Minesweeper::Game game;// runs the game

    game.initialize(
        Minesweeper::MIN_WINDOW_WIDTH,
        Minesweeper::MIN_WINDOW_HEIGHT
    );

    game.run();

    return 0;
}
