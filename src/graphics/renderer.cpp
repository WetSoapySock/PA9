#include "../include/graphics/Renderer.h"
#include "../include/utils/Constants.h"

Minesweeper::Renderer::Renderer()
{
    boardOffsetX = 0;
    boardOffsetY = 0;
    showGridLines = true;
    useCheckerboardPattern = true;

    fontLoaded = false;
}

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

void Minesweeper::Renderer::clear()
{
    window.clear(Colors::BACKGROUND);
}

void Minesweeper::Renderer::display()
{
    window.display();
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

    // 🔥 Draw number
    renderCellNumber(cell, x, y);
}

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

void Minesweeper::Renderer::addUIComponent(std::unique_ptr<UIComponent> component)
{
    uiComponents.push_back(std::move(component));
}

void Minesweeper::Renderer::renderUI()
{
    for (auto& comp : uiComponents)
    {
        if (comp)
            comp->draw(window);
    }
}