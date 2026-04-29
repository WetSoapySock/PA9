#include "../include/input/MouseHandler.h"

Minesweeper::MouseHandler::MouseHandler()
{
    buttons[0] = false;
    buttons[1] = false;

    currentPosition = sf::Vector2i(0, 0);
    previousPosition = sf::Vector2i(0, 0);
    movementDelta = sf::Vector2i(0, 0);

    withinWindow = true;
    enabled = true;
}

void Minesweeper::MouseHandler::update(const sf::Event& event)
{
    if (!enabled)
    {
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        processButtonPress(event);
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        processButtonRelease(event);
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        processMouseMove(event);
    }
    else if (event.type == sf::Event::MouseEntered)
    {
        withinWindow = true;
    }
    else if (event.type == sf::Event::MouseLeft)
    {
        withinWindow = false;
    }
}

void Minesweeper::MouseHandler::update()
{
    movementDelta = currentPosition - previousPosition;
    previousPosition = currentPosition;
}

void Minesweeper::MouseHandler::reset()
{
    buttons[0] = false;
    buttons[1] = false;

    currentPosition = sf::Vector2i(0, 0);
    previousPosition = sf::Vector2i(0, 0);
    movementDelta = sf::Vector2i(0, 0);

    withinWindow = true;
}

int Minesweeper::MouseHandler::onButtonPressed(MouseButton button, MouseCallback callback)
{
    callbacks.push_back(CallbackEntry(MouseEventType::BUTTON_PRESSED, button, callback));

    return static_cast<int>(callbacks.size()) - 1;
}

int Minesweeper::MouseHandler::onButtonReleased(MouseButton button, MouseCallback callback)
{
    callbacks.push_back(CallbackEntry(MouseEventType::BUTTON_RELEASED, button, callback));

    return static_cast<int>(callbacks.size()) - 1;
}

int Minesweeper::MouseHandler::onMouseMove(MouseCallback callback)
{
    callbacks.push_back(CallbackEntry(MouseEventType::MOVED, MouseButton::NONE, callback));

    return static_cast<int>(callbacks.size()) - 1;
}

sf::Vector2i Minesweeper::MouseHandler::screenToGrid(const sf::Vector2i& screenPos,
    int boardOffsetX,
    int boardOffsetY,
    int cellSize) const
{
    if (cellSize <= 0)
    {
        return sf::Vector2i(-1, -1);
    }

    int adjustedX = screenPos.x - boardOffsetX;
    int adjustedY = screenPos.y - boardOffsetY;

    if (adjustedX < 0 || adjustedY < 0)
    {
        return sf::Vector2i(-1, -1);
    }

    int gridX = adjustedX / cellSize;
    int gridY = adjustedY / cellSize;

    return sf::Vector2i(gridX, gridY);
}

sf::Vector2i Minesweeper::MouseHandler::getGridPosition(int boardOffsetX,
    int boardOffsetY,
    int cellSize,
    int boardWidth,
    int boardHeight) const
{
    sf::Vector2i gridPosition = screenToGrid(currentPosition,
        boardOffsetX,
        boardOffsetY,
        cellSize);

    if (gridPosition.x < 0 ||
        gridPosition.y < 0 ||
        gridPosition.x >= boardWidth ||
        gridPosition.y >= boardHeight)
    {
        return sf::Vector2i(-1, -1);
    }

    return gridPosition;
}

void Minesweeper::MouseHandler::invokeCallbacks(const MouseEvent& event)
{
    for (int i = 0; i < callbacks.size(); i++)
    {
        bool typeMatches = callbacks[i].type == event.type;
        bool buttonMatches = callbacks[i].button == event.button ||
            callbacks[i].button == MouseButton::NONE;

        if (typeMatches && buttonMatches)
        {
            callbacks[i].callback(event);
        }
    }
}

Minesweeper::MouseButton Minesweeper::MouseHandler::sfmlButtonToEnum(sf::Mouse::Button button) const
{
    if (button == sf::Mouse::Left)
    {
        return MouseButton::LEFT;
    }
    else if (button == sf::Mouse::Right)
    {
        return MouseButton::RIGHT;
    }

    return MouseButton::NONE;
}

void Minesweeper::MouseHandler::processButtonPress(const sf::Event& event)
{
    currentPosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);

    MouseButton button = sfmlButtonToEnum(event.mouseButton.button);

    if (button == MouseButton::LEFT)
    {
        buttons[0] = true;
    }
    else if (button == MouseButton::RIGHT)
    {
        buttons[1] = true;
    }

    dispatchButtonEvent(button, true);
}

void Minesweeper::MouseHandler::processButtonRelease(const sf::Event& event)
{
    currentPosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);

    MouseButton button = sfmlButtonToEnum(event.mouseButton.button);

    if (button == MouseButton::LEFT)
    {
        buttons[0] = false;
    }
    else if (button == MouseButton::RIGHT)
    {
        buttons[1] = false;
    }

    dispatchButtonEvent(button, false);
}

void Minesweeper::MouseHandler::processMouseMove(const sf::Event& event)
{
    previousPosition = currentPosition;
    currentPosition = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    movementDelta = currentPosition - previousPosition;

    dispatchMoveEvent();
}

void Minesweeper::MouseHandler::dispatchButtonEvent(MouseButton button, bool pressed)
{
    MouseEvent event;

    if (pressed)
    {
        event.type = MouseEventType::BUTTON_PRESSED;
    }
    else
    {
        event.type = MouseEventType::BUTTON_RELEASED;
    }

    event.button = button;
    event.position = currentPosition;

    invokeCallbacks(event);
}

void Minesweeper::MouseHandler::dispatchMoveEvent()
{
    MouseEvent event;

    event.type = MouseEventType::MOVED;
    event.button = MouseButton::NONE;
    event.position = currentPosition;

    invokeCallbacks(event);
}