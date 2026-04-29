#include "../include/graphics/Renderer.h"
#include "../include/utils/Constants.h"

Minesweeper::Renderer::Renderer()
{
    boardOffsetX = 0;
    boardOffsetY = 0;

    showGridLines = true;
    useCheckerboardPattern = true;
}

bool Minesweeper::Renderer::initialize(int width, int height, const std::string& title)
{
    window.create(sf::VideoMode(sf::Vector2u(width, height)), title);

    if (!window.isOpen())
    {
        return false;
    }

    return true;
}

void Minesweeper::Renderer::clear()
{
    window.clear(Colors::BACKGROUND);
}

void Minesweeper::Renderer::display()
{
    window.display();
}

void Minesweeper::Renderer::setBoardOffset(int x, int y)
{
    boardOffsetX = x;
    boardOffsetY = y;
}

void Minesweeper::Renderer::calculateBoardOffset(int boardWidth, int boardHeight)
{
    int boardPixelWidth = boardWidth * CELL_SIZE;
    int boardPixelHeight = boardHeight * CELL_SIZE;

    boardOffsetX = (window.getSize().x - boardPixelWidth) / 2;
    boardOffsetY = (window.getSize().y - boardPixelHeight) / 2;
}

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

void Minesweeper::Renderer::renderCell(const Cell& cell, int x, int y)
{
    sf::RectangleShape cellShape;

    cellShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    cellShape.setPosition(sf::Vector2f(
        static_cast<float>(boardOffsetX + x * CELL_SIZE),
        static_cast<float>(boardOffsetY + y * CELL_SIZE)
    ));

    cellShape.setFillColor(getCellColor(cell, x, y));

    if (showGridLines)
    {
        cellShape.setOutlineThickness(1.0f);
        cellShape.setOutlineColor(sf::Color::Black);
    }

    window.draw(cellShape);

    // Temporary/simple rendering:
    // This draws mines and numbers using colored blocks/text would require a font.
    // If your SpriteSheet is ready, we can replace this with sprite rendering later.
}

sf::Color Minesweeper::Renderer::getCellColor(const Cell& cell, int x, int y) const
{
    if (cell.isRevealed())
    {
        if (cell.isMine())
        {
            return Colors::ERROR;
        }

        return Colors::REVEALED;
    }

    if (cell.isFlagged())
    {
        return Colors::WARNING;
    }

    if (useCheckerboardPattern)
    {
        if ((x + y) % 2 == 0)
        {
            return Colors::COVERED_LIGHT;
        }
        else
        {
            return Colors::COVERED_DARK;
        }
    }

    return Colors::COVERED_LIGHT;
}

void Minesweeper::Renderer::addUIComponent(std::unique_ptr<UIComponent> component)
{
    uiComponents.push_back(std::move(component));
}

void Minesweeper::Renderer::renderUI()
{
    for (int i = 0; i < uiComponents.size(); i++)
    {
        if (uiComponents[i] != nullptr)
        {
            uiComponents[i]->draw(window);
        }
    }
}