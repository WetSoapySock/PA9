#ifndef PA9_MOUSEHANDLER_H
#define PA9_MOUSEHANDLER_H

#pragma once
#include <SFML/Window/Mouse.h>
#include <SFML/Window/Event.h>
#include <SFML/System/Vector2.h>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Minesweeper {
    enum class MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
        XBUTTON1 = 3,
        XBUTTON2 = 4,
        NONE = 5
    };

    enum class MouseEventType {
        BUTTON_PRESSED,
        BUTTON_RELEASED,
        MOVED,
        WHEEL_SCROLLED,
        ENTERED_WINDOW,
        LEFT_WINDOW
    };

    struct MouseEvent {
        MouseEventType type;
        MouseButton button;
        sf::Vector2i position;
        sf::Vector2i delta;
        float wheelDelta;  // For scroll events
        bool isShiftPressed;
        bool isCtrlPressed;
        bool isAltPressed;

        MouseEvent()
            : type(MouseEventType::BUTTON_PRESSED)
            , button(MouseButton::NONE)
            , position(0, 0)
            , delta(0, 0)
            , wheelDelta(0.0f)
            , isShiftPressed(false)
            , isCtrlPressed(false)
            , isAltPressed(false) {}
    };

    using MouseCallback = std::function<void(const MouseEvent&)>;

    class MouseHandler {
    public:
        MouseHandler();
        ~MouseHandler() = default;

        // Disable copy
        MouseHandler(const MouseHandler&) = delete;
        MouseHandler& operator=(const MouseHandler&) = delete;

        void update(const sf::Event& event);
        void update();
        void reset();

        /**
         * @brief Register callback for specific mouse button press
         * @param button Button to listen for
         * @param callback Function to call when button is pressed
         * @return Callback ID for removal
         */
        int onButtonPressed(MouseButton button, MouseCallback callback);

        /**
         * @brief Register callback for specific mouse button release
         * @param button Button to listen for
         * @param callback Function to call when button is released
         * @return Callback ID for removal
         */
        int onButtonReleased(MouseButton button, MouseCallback callback);

        /**
         * @brief Register callback for mouse movement
         * @param callback Function to call when mouse moves
         * @return Callback ID for removal
         */
        int onMouseMove(MouseCallback callback);

        /**
         * @brief Register callback for mouse wheel scroll
         * @param callback Function to call when wheel is scrolled
         * @return Callback ID for removal
         */
        int onWheelScroll(MouseCallback callback);

        /**
         * @brief Register callback for any mouse event
         * @param callback Function to call for any mouse event
         * @return Callback ID for removal
         */
        int onAnyEvent(MouseCallback callback);

        /**
         * @brief Remove a registered callback
         * @param callbackId ID returned from registration
         * @return true if callback was found and removed
         */
        bool removeCallback(int callbackId);

        /**
         * @brief Clear all registered callbacks
         */
        void clearCallbacks();

        // Getters for current mouse state
        sf::Vector2i getPosition() const { return currentPosition; }
        sf::Vector2i getDelta() const { return movementDelta; }
        sf::Vector2i getPreviousPosition() const { return previousPosition; }

        bool isLeftPressed() const { return buttons[static_cast<int>(MouseButton::LEFT)]; }
        bool isRightPressed() const { return buttons[static_cast<int>(MouseButton::RIGHT)]; }
        bool isMiddlePressed() const { return buttons[static_cast<int>(MouseButton::MIDDLE)]; }
        bool isButtonPressed(MouseButton button) const;

        bool isWithinWindow() const { return withinWindow; }
        bool isDragging() const { return isLeftPressed() || isRightPressed() || isMiddlePressed(); }

        /**
         * @brief Get current scroll delta (accumulated since last frame)
         * @return Scroll delta value
         */
        float getScrollDelta() const { return scrollDelta; }

        // Screen to grid coordinate conversion
        /**
         * @brief Convert screen coordinates to grid coordinates
         * @param screenPos Screen position in pixels
         * @param boardOffsetX Board X offset on screen
         * @param boardOffsetY Board Y offset on screen
         * @param cellSize Size of each cell in pixels
         * @return Grid coordinates or (-1,-1) if out of bounds
         */
        sf::Vector2i screenToGrid(const sf::Vector2i& screenPos,
                                  int boardOffsetX, int boardOffsetY,
                                  int cellSize) const;

        /**
         * @brief Get current grid position based on board placement
         * @param boardOffsetX Board X offset on screen
         * @param boardOffsetY Board Y offset on screen
         * @param cellSize Size of each cell in pixels
         * @param boardWidth Width of board in cells
         * @param boardHeight Height of board in cells
         * @return Grid coordinates or (-1,-1) if out of bounds
         */
        sf::Vector2i getGridPosition(int boardOffsetX, int boardOffsetY,
                                     int cellSize, int boardWidth, int boardHeight) const;

        /**
         * @brief Check if current mouse position is within board area
         * @param boardOffsetX Board X offset on screen
         * @param boardOffsetY Board Y offset on screen
         * @param boardWidthPx Board width in pixels
         * @param boardHeightPx Board height in pixels
         * @return true if mouse is over the board
         */
        bool isOverBoard(int boardOffsetX, int boardOffsetY,
                        int boardWidthPx, int boardHeightPx) const;

        /**
         * @brief Enable/disable mouse handling
         */
        void setEnabled(bool enable) { enabled = enable; }
        bool isEnabled() const { return enabled; }

        /**
         * @brief Set double-click detection parameters
         * @param maxTimeMs Maximum time between clicks for double-click (milliseconds)
         * @param maxDistancePx Maximum pixel distance for double-click
         */
        void setDoubleClickParameters(int maxTimeMs, int maxDistancePx);

        /**
         * @brief Check if double-click occurred on specific button
         * @param button Button to check
         * @return true if double-click detected
         */
        bool isDoubleClick(MouseButton button) const;

        /**
         * @brief Reset double-click detection state
         */
        void resetDoubleClick();

    private:
        // Button states (true = pressed)
        bool buttons[5];

        // Position tracking
        sf::Vector2i currentPosition;
        sf::Vector2i previousPosition;
        sf::Vector2i movementDelta;

        // Scroll tracking
        float scrollDelta;
        float scrollAccumulator;

        // State flags
        bool withinWindow;
        bool enabled;

        // Double-click detection
        struct DoubleClickData {
            sf::Vector2i lastPosition;
            sf::Clock lastClickTime;
            int maxTimeMs;
            int maxDistancePx;
            bool lastClickButton[5];

            DoubleClickData()
                : lastPosition(0, 0)
                , maxTimeMs(300)  // 300ms default
                , maxDistancePx(5) // 5 pixels default
            {
                for (int i = 0; i < 5; i++) {
                    lastClickButton[i] = false;
                }
            }
        } doubleClickData;

        // Callback management
        struct CallbackEntry {
            int id;
            MouseEventType type;
            MouseButton button;  // For button-specific callbacks
            MouseCallback callback;
        };

        std::vector<CallbackEntry> callbacks;
        int nextCallbackId;

        // Helper methods
        void invokeCallbacks(const MouseEvent& event);
        MouseButton sfmlButtonToEnum(sf::Mouse::Button button) const;
        MouseEventType sfmlEventToType(const sf::Event& event) const;
        void updateModifierKeys(MouseEvent& event);
        void updateScrollAccumulator();
        void checkDoubleClick(const MouseEvent& event);

        // Process specific event types
        void processButtonPress(const sf::Event& event);
        void processButtonRelease(const sf::Event& event);
        void processMouseMove(const sf::Event& event);
        void processWheelScroll(const sf::Event& event);
        void processMouseEnter();
        void processMouseLeave();

        // Internal event dispatcher
        void dispatchButtonEvent(MouseButton button, bool pressed);
        void dispatchMoveEvent();
        void dispatchScrollEvent();
    };

} // namespace Minesweeper

#endif //PA9_MOUSEHANDLER_H
