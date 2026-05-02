#include "../include/input/mouseHandler.h"

/*
 Function: MouseHandler (constructor)
 Description: Initializes mouse state with no buttons pressed, position at (0,0),
              delta at (0,0), mouse within window, and handler enabled.
*/
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

/*
 Function: update (event-based)
 Description: Processes SFML mouse events including button presses/releases,
              mouse movement, and window enter/leave events. Updates internal
              state and triggers registered callbacks.
              Parameters: event = SFML event to process
*/
void Minesweeper::MouseHandler::update(const sf::Event& event)
{
    if (!enabled)
    {
        return;
    }

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        processButtonPress(event);
    }
    else if (event.is<sf::Event::MouseButtonReleased>())
    {
        processButtonRelease(event);
    }
    else if (event.is<sf::Event::MouseMoved>())
    {
        processMouseMove(event);
    }
    else if (event.is<sf::Event::MouseEntered>())
    {
        withinWindow = true;
    }
    else if (event.is<sf::Event::MouseLeft>())
    {
        withinWindow = false;
    }
}

/*
 Function: update (continuous)
 Description: Updates mouse state each frame. Calculates movement delta
              based on position change since last frame. Called every frame
              regardless of whether mouse events occurred.
*/
void Minesweeper::MouseHandler::update()
{
    movementDelta = currentPosition - previousPosition;
    previousPosition = currentPosition;
}

/*
 Function: reset
 Description: Resets all mouse state to default values. Clears button presses,
              resets position and delta to zero, and sets withinWindow to true.
              Called when restarting game or resetting input.
*/
void Minesweeper::MouseHandler::reset()
{
    buttons[0] = false;
    buttons[1] = false;

    currentPosition = sf::Vector2i(0, 0);
    previousPosition = sf::Vector2i(0, 0);
    movementDelta = sf::Vector2i(0, 0);

    withinWindow = true;
}

/*
 Function: onButtonPressed
 Description: Registers a callback function to be invoked when a specific
              mouse button is pressed. Returns callback ID for potential removal.
              Parameters: button = LEFT or RIGHT, callback = function to call
*/
int Minesweeper::MouseHandler::onButtonPressed(MouseButton button, MouseCallback callback)
{
    callbacks.push_back(CallbackEntry(MouseEventType::BUTTON_PRESSED, button, callback));
    return static_cast<int>(callbacks.size()) - 1;
}

/*
 Function: onButtonReleased
 Description: Registers a callback function to be invoked when a specific
              mouse button is released. Returns callback ID for potential removal.
              Parameters: button = LEFT or RIGHT, callback = function to call
*/
int Minesweeper::MouseHandler::onButtonReleased(MouseButton button, MouseCallback callback)
{
    callbacks.push_back(CallbackEntry(MouseEventType::BUTTON_RELEASED, button, callback));
    return static_cast<int>(callbacks.size()) - 1;
}

/*
 Function: onMouseMove
 Description: Registers a callback function to be invoked when the mouse moves.
              Returns callback ID for potential removal.
              Parameters: callback = function to call with mouse position/delta
*/
int Minesweeper::MouseHandler::onMouseMove(MouseCallback callback)
{
    callbacks.push_back(CallbackEntry(MouseEventType::MOVED, MouseButton::NONE, callback));
    return static_cast<int>(callbacks.size()) - 1;
}

/*
 Function: screenToGrid
 Description: Converts screen pixel coordinates to grid cell coordinates.
              Returns (-1,-1) if coordinates are out of bounds or cellSize invalid.
              Parameters: screenPos = pixel coordinates, boardOffsetX/Y = board position,
              cellSize = size of each cell in pixels
*/
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

    return sf::Vector2i(adjustedX / cellSize, adjustedY / cellSize);
}

/*
 Function: getGridPosition
 Description: Convenience method that gets current mouse grid position.
              Combines getPosition() with screenToGrid() and validates
              against board dimensions. Returns (-1,-1) if out of bounds.
              Parameters: boardOffsetX/Y = board position, cellSize = cell size,
              boardWidth/Height = board dimensions in cells
*/
sf::Vector2i Minesweeper::MouseHandler::getGridPosition(int boardOffsetX,
    int boardOffsetY,
    int cellSize,
    int boardWidth,
    int boardHeight) const
{
    sf::Vector2i gridPosition = screenToGrid(currentPosition, boardOffsetX, boardOffsetY, cellSize);

    if (gridPosition.x < 0 || gridPosition.y < 0 ||
        gridPosition.x >= boardWidth || gridPosition.y >= boardHeight)
    {
        return sf::Vector2i(-1, -1);
    }

    return gridPosition;
}

/*
 Function: invokeCallbacks
 Description: Internal helper that executes all registered callbacks matching
              the event type and button. Called by dispatch methods.
              Parameters: event = mouse event containing type, button, position
*/
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

/*
 Function: sfmlButtonToEnum
 Description: Converts SFML mouse button type to internal MouseButton enum.
              Supports LEFT and RIGHT buttons; others map to NONE.
              Parameters: button = SFML mouse button value
*/
Minesweeper::MouseButton Minesweeper::MouseHandler::sfmlButtonToEnum(sf::Mouse::Button button) const
{
    if (button == sf::Mouse::Button::Left)
    {
        return MouseButton::LEFT;
    }
    else if (button == sf::Mouse::Button::Right)
    {
        return MouseButton::RIGHT;
    }

    return MouseButton::NONE;
}

/*
 Function: processButtonPress
 Description: Internal handler for mouse button press events. Updates button state,
              records current position, and dispatches press callbacks.
              Parameters: event = SFML event containing button information
*/
void Minesweeper::MouseHandler::processButtonPress(const sf::Event& event)
{
    const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

    if (mouseEvent == nullptr)
    {
        return;
    }

    currentPosition = mouseEvent->position;

    MouseButton button = sfmlButtonToEnum(mouseEvent->button);

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

/*
 Function: processButtonRelease
 Description: Internal handler for mouse button release events. Updates button state,
              records current position, and dispatches release callbacks.
              Parameters: event = SFML event containing button information
*/
void Minesweeper::MouseHandler::processButtonRelease(const sf::Event& event)
{
    const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();

    if (mouseEvent == nullptr)
    {
        return;
    }

    currentPosition = mouseEvent->position;

    MouseButton button = sfmlButtonToEnum(mouseEvent->button);

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

/*
 Function: processMouseMove
 Description: Internal handler for mouse movement events. Updates position,
              calculates delta, and dispatches move callbacks.
              Parameters: event = SFML event containing new mouse position
*/
void Minesweeper::MouseHandler::processMouseMove(const sf::Event& event)
{
    const auto* mouseEvent = event.getIf<sf::Event::MouseMoved>();

    if (mouseEvent == nullptr)
    {
        return;
    }

    previousPosition = currentPosition;
    currentPosition = mouseEvent->position;
    movementDelta = currentPosition - previousPosition;

    dispatchMoveEvent();
}

/*
 Function: dispatchButtonEvent
 Description: Internal helper that creates a MouseEvent structure and invokes
              callbacks for button press or release events.
              Parameters: button = which button, pressed = true for press, false for release
*/
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

/*
 Function: dispatchMoveEvent
 Description: Internal helper that creates a MouseEvent structure and invokes
              callbacks for mouse movement events.
*/
void Minesweeper::MouseHandler::dispatchMoveEvent()
{
    MouseEvent event;

    event.type = MouseEventType::MOVED;
    event.button = MouseButton::NONE;
    event.position = currentPosition;

    invokeCallbacks(event);
}
