#pragma once
#include <SFML/Window/Event.h>
#include <SFML/System/Vector2.h>
#include <functional>
#include <unordered_map>
#include <vector>

/** AI Gen, "Create an InputHandler class that manages callbacks for
    SFML events like key presses, window events, and generic input." */

namespace Minesweeper {
    class InputHandler {
        public:
        // AI Gen, Callback type for any SFML event
        using Callback = std::function<void(const sf::Event&)>;
    private:
        // AI Gen, Map of event types to vectors of callbacks
        std::unordered_map<sf::Event::EventType, std::vector<Callback>> callbacks;
        bool enabled;
    public:
        // Constructor and Destructor
        InputHandler();
        ~InputHandler() = default;

        // AI Gen, Process an SFML event and trigger registered callbacks
        void processEvent(const sf::Event& event);

        // AI Gen, Registers a callback for a specific event type
        void registerCallback(sf::Event::EventType type, Callback callback);
        void clearCallbacks();

        // AI Gen, Enable/disable input processing
        void setEnabled(bool enable) {enabled = enable;}

        // AI Gen, Check if input handler is enabled
        bool isEnabled() const {return enabled;}
    };
}
