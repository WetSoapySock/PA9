#include "../include/graphics/renderer.h"
#include "../include/utils/constants.h"

/*
 Function: Renderer (constructor)
 Description: Initializes renderer with default settings. Sets board offsets to zero,
              enables grid lines and checkerboard pattern (Google Minesweeper style),
              and marks font as not loaded until initialize() is called.
*/
Minesweeper::Renderer::Renderer()
{
    boardOffsetX = 0;
    boardOffsetY = 0;
    showGridLines = true;
    useCheckerboardPattern = true;

    fontLoaded = false;
}

/*
 Function: initialize
 Description: Creates the SFML window with specified dimensions and title.
              Attempts to load the Arial font from assets directory.
              Returns true if window was created successfully.
              Parameters: width, height = window dimensions in pixels, title = window title text
*/
bool Minesweeper::Renderer::initialize(int width, int height, const std::string& title)
{
    window.create(sf::VideoMode(sf::Vector2u(width, height)), title);

    if (!window.isOpen())
    {
        return false;
    }

    fontLoaded = font.openFromFile("assets/arial.ttf");

    return true;
}

/*
 Function: clear
 Description: Clears the window with the background color defined in constants.
              Called at the beginning of each frame before rendering.
*/
void Minesweeper::Renderer::clear()
{
    window.clear(Colors::BACKGROUND);
}

/*
 Function: display
 Description: Presents the rendered frame to the screen. Called after all
              drawing operations are complete for the current frame.
*/
void Minesweeper::Renderer::display()
{
    window.display();
}

/*
 Function: calculateBoardOffset
 Description: Computes the X and Y offsets needed to center the game board
              within the window. Uses CELL_SIZE constant to convert grid
              dimensions to pixel dimensions.
              Parameters: boardWidth, boardHeight = dimensions in cells
*/
void Minesweeper::Renderer::calculateBoardOffset(int boardWidth, int boardHeight)
{
    int boardPixelWidth = boardWidth * CELL_SIZE;
    int boardPixelHeight = boardHeight * CELL_SIZE;

    boardOffsetX = (window.getSize().x - boardPixelWidth) / 2;
    boardOffsetY = (window.getSize().y - boardPixelHeight) / 2;
}

/*
 Function: renderBoard
 Description: Renders the entire game board by iterating through all cells
              and calling renderCell() for each. Also centers the board
              by calculating offsets before rendering.
              Parameters: board = reference to the game board to render
*/
void Minesweeper::Renderer::renderBoard(const Board& board)
{
    calculateBoardOffset(board.getWidth(), board.getHeight());

    for (int y = 0; y < board.getHeight(); y++)
    {
        for (int x = 0; x < board.getWidth(); x++)
        {
            const Cell* cell = board.getCell(x, y);
            if (cell != nullptr)
            {
                renderCell(*cell, x, y);
            }
        }
    }
}

/*
 Function: renderCell
 Description: Renders a single cell at the specified grid coordinates.
              Creates a rectangle shape, sets its position, color, and outline.
              Also calls renderCellNumber() to draw any number on revealed cells.
              Parameters: cell = reference to cell to render, x, y = grid coordinates
*/
void Minesweeper::Renderer::renderCell(const Cell& cell, int x, int y)
{
    sf::RectangleShape cellShape;
    cellShape.setSize(sf::Vector2f((float)CELL_SIZE, (float)CELL_SIZE));

    cellShape.setPosition(sf::Vector2f(
        (float)(boardOffsetX + x * CELL_SIZE),
        (float)(boardOffsetY + y * CELL_SIZE)
    ));

    cellShape.setFillColor(getCellColor(cell, x, y));

    if (showGridLines)
    {
        cellShape.setOutlineThickness(1.0f);
        cellShape.setOutlineColor(sf::Color::Black);
    }

    window.draw(cellShape);

    renderCellNumber(cell, x, y);
}

/*
 Function: getCellColor
 Description: Determines the color of a cell based on its state and position.
              Revealed cells are off-white, mines show red, flagged cells show orange.
              Covered cells use checkerboard pattern (alternating light/dark gray).
              Parameters: cell = reference to cell, x, y = coordinates for checkerboard pattern
*/
sf::Color Minesweeper::Renderer::getCellColor(const Cell& cell, int x, int y) const
{
    if (cell.isRevealed())
    {
        if (cell.isMine())
            return Colors::ERROR;

        return Colors::REVEALED;
    }

    if (cell.isFlagged())
        return Colors::WARNING;

    if (useCheckerboardPattern)
    {
        return ((x + y) % 2 == 0)
            ? Colors::COVERED_LIGHT
            : Colors::COVERED_DARK;
    }

    return Colors::COVERED_LIGHT;
}

/*
 Function: renderCellNumber
 Description: Draws a number on a revealed non-mine cell. Numbers 1-8 are displayed
              using classic Minesweeper colors. Empty cells (number 0) show nothing.
              Parameters: cell = reference to cell, x, y = grid coordinates for positioning
*/
void Minesweeper::Renderer::renderCellNumber(const Cell& cell, int x, int y)
{
    if (!fontLoaded || !cell.isRevealed() || cell.isMine())
        return;

    int number = cell.getNumber();
    if (number <= 0)
    {
        return;
    }
        
    sf::Text text(font, std::to_string(number), 20);
    text.setFillColor(getNumberColor(number));
    text.setStyle(sf::Text::Bold);

    float px = (float)(boardOffsetX + x * CELL_SIZE);
    float py = (float)(boardOffsetY + y * CELL_SIZE);

    text.setPosition(sf::Vector2f(px + 10, py + 5));

    window.draw(text);
}

/*
 Function: getNumberColor
 Description: Returns the classic Minesweeper color for each number 1-8.
              Colors: 1=Blue, 2=Green, 3=Red, 4=Dark Blue, 5=Maroon,
              6=Cyan, 7=Black, 8=Dark Gray. Default returns Black.
              Parameters: number = the number to get color for (1-8)
*/
sf::Color Minesweeper::Renderer::getNumberColor(int number) const
{
    switch (number)
    {
    case 1: return Colors::NUMBER_1;
    case 2: return Colors::NUMBER_2;
    case 3: return Colors::NUMBER_3;
    case 4: return Colors::NUMBER_4;
    case 5: return Colors::NUMBER_5;
    case 6: return Colors::NUMBER_6;
    case 7: return Colors::NUMBER_7;
    case 8: return Colors::NUMBER_8;
    default: return sf::Color::Black;
    }
}

/*
 Function: addUIComponent
 Description: Adds a UI component (button, label, counter) to the renderer.
              Components are stored as unique_ptrs for automatic memory management.
              Parameters: component = unique_ptr to the UI component to add
*/
void Minesweeper::Renderer::addUIComponent(std::unique_ptr<UIComponent> component)
{
    uiComponents.push_back(std::move(component));
}

/*
 Function: renderUI
 Description: Renders all registered UI components by calling draw() on each.
              Components are drawn in the order they were added (back to front).
*/
void Minesweeper::Renderer::renderUI()
{
    for (auto& comp : uiComponents)
    {
        if (comp)
            comp->draw(window);
    }
}
