#ifndef PA9_MOUSEHANDLER_H
#define PA9_MOUSEHANDLER_H

#pragma once
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

/** AI Gen, Create a MouseHandler class for a Minesweeper game that:
    Tracks left and right mouse button states
    Tracks the current mouse position and movement
    Converts screen coordinates to grid positions for Minesweeper
    Tracks if mouse position is within the game window
    Handles button press/release events **/

namespace Minesweeper {
    enum class MouseButton {
        LEFT = 0,   // Reveal cell
        RIGHT = 1,  // Place/remove flag
        NONE = 2    // Default state
    };

    enum class MouseEventType {
        BUTTON_PRESSED,     // Click
        BUTTON_RELEASED,    // Release
        MOVED               // Mouse movement
    };

    struct MouseEvent {
        MouseEventType type;      // What kind of even occured
        MouseButton button;       // Left or right click

        // AI Gen, "How can we create a vector space and tracking for mouse input?"
        sf::Vector2i position;    // Mouse pos w/ screen coords

        // AI Gen, "How can we create a vector space and tracking for mouse input?"
        MouseEvent()
            : type(MouseEventType::BUTTON_PRESSED)
            , button(MouseButton::NONE)
            , position(0, 0) {}
    };

    // AI Gen, "How can we communicate with the computer and user when giving inputs in our game?"
    using MouseCallback = std::function<void(const MouseEvent&)>;
    class MouseHandler {
    public:
        // Constructor and destructor
        MouseHandler();
        ~MouseHandler() = default;

        // Disable copy
        MouseHandler(const MouseHandler&) = delete;
        MouseHandler& operator=(const MouseHandler&) = delete;

        // AI Gen, Updates mouse state from SFML event
        void update(const sf::Event& event);
        void update();
        void reset();

        // AI Gen, Register callback for mouse button inputs.
        int onButtonPressed(MouseButton button, MouseCallback callback);

        // AI Gen, Register callback for mouse button release.
        int onButtonReleased(MouseButton button, MouseCallback callback);

        // AI Gen, Register callback for mouse movements
        int onMouseMove(MouseCallback callback);

        // AI Gen, Getters for current mouse state
        sf::Vector2i getPosition() const {return currentPosition;}
        sf::Vector2i getDelta() const {return movementDelta;}

        bool isLeftPressed() const {return buttons[0];}
        bool isRightPressed() const {return buttons[1];}
        bool isAnyButtonPressed() const {return isLeftPressed() || isRightPressed();}

        // AI Gen, Screen to grid coordinate conversion
        /**
         * @param screenPos Screen position in pixels
         * @param boardOffsetX Board X offset on screen
         * @param boardOffsetY Board Y offset on screen
         * @param cellSize Size of each cell in pixels
         * @return Grid coordinates or (-1,-1) if out of bounds */
        sf::Vector2i screenToGrid(const sf::Vector2i& screenPos,
                                  int boardOffsetX, int boardOffsetY,
                                  int cellSize) const;

        // AI Gen, Get current grid position based on board placement
        /**
         * @param boardOffsetX Board X offset on screen
         * @param boardOffsetY Board Y offset on screen
         * @param cellSize Size of each cell in pixels
         * @param boardWidth Width of board in cells
         * @param boardHeight Height of board in cells
         * @return Grid coordinates or (-1,-1) if out of bounds */
        sf::Vector2i getGridPosition(int boardOffsetX, int boardOffsetY,
                                     int cellSize, int boardWidth, int boardHeight) const;

        // AI Gen, Enable/disable mouse handling
        void setEnabled(bool enable) {enabled = enable;}
        bool isEnabled() const {return enabled;}

    private:
        // AI Gen, Button state array, index 0 = left, 1 = right, 2 = none
        bool buttons[2] = {false, false};

        // AI Gen, Position tracking
        sf::Vector2i currentPosition {0, 0};
        sf::Vector2i previousPosition {0, 0};
        sf::Vector2i movementDelta {0, 0};

        // AI Gen, Window focus tracking
        bool withinWindow = true;  // Assumes cursor is within window
        bool enabled = true;

        // AI-Gen, Callback storage
        struct CallbackEntry {
            MouseEventType type;
            MouseButton button;
            MouseCallback callback;

            // AI-Gen, Constructor for easy creation
            CallbackEntry(MouseEventType t, MouseButton b, MouseCallback c)
                : type(t), button(b), callback(c) {}
        };

        //  AI Gen, Stores all registered callbacks
        std::vector<CallbackEntry> callbacks;

        // AI Gen, Invoke matching callbacks for event
        void invokeCallbacks(const MouseEvent& event);

        // AI Gen, Convert SFML button to internal enum
        MouseButton sfmlButtonToEnum(sf::Mouse::Button button) const;

        // AI Gen, Process button press event
        void processButtonPress(const sf::Event& event);

        // AI Gen, Process button release event
        void processButtonRelease(const sf::Event& event);

        // AI Gen, Process mouse movements
        void processMouseMove(const sf::Event& event);

        // AI Gen, Dispatch button event to callbacks
        void dispatchButtonEvent(MouseButton button, bool pressed);

        // AI Gen, Dispatch movement event to callbacks
        void dispatchMoveEvent();
    };
}

#endif //PA9_MOUSEHANDLER_H
