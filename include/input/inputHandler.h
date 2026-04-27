#ifndef PA9_INPUTHANDLER_H
#define PA9_INPUTHANDLER_H

#pragma once
#include <SFML/Window.h>
#include <functional>
#include <unordered_map>

namespace Minesweeper {
    class InputHandler {
    public:
        using Callback = std::function<void(const sf::Event&)>;

    private:
        std::unordered_map<sf::Event::EventType, std::vector<Callback>> callbacks;
        bool enabled;

    public:
        InputHandler();
        ~InputHandler() = default;

        void processEvent(const sf::Event& event);
        void registerCallback(sf::Event::EventType type, Callback callback);
        void clearCallbacks();

        void setEnabled(bool enable) { enabled = enable; }
        bool isEnabled() const { return enabled; }
    };

    class MouseHandler {
    private:
        sf::Vector2i lastPosition;
        bool leftButtonPressed;
        bool rightButtonPressed;
        bool middleButtonPressed;

    public:
        MouseHandler();

        void update(const sf::Event& event);
        void reset();

        sf::Vector2i getPosition() const;
        sf::Vector2i getDelta() const;

        bool isLeftPressed() const { return leftButtonPressed; }
        bool isRightPressed() const { return rightButtonPressed; }
        bool isMiddlePressed() const { return middleButtonPressed; }

        // Convert screen to grid coordinates
        sf::Vector2i screenToGrid(const sf::Vector2i& screenPos,
                                  int boardOffsetX, int boardOffsetY,
                                  int cellSize) const;
    };
}

#endif //PA9_INPUTHANDLER_H
