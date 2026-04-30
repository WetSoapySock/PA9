#ifndef PA9_UICOMPONENTS_H
#define PA9_UICOMPONENTS_H

#pragma once
#include <SFML/Graphics.hpp>
// AI Gen, Libraries
#include <functional>
#include <string>
#include <memory>

/** Gen AI, "Now that we have our sprites, we need to tie them together
    with the UI. Help create methods for that so we can implment their definitions later" */

namespace Minesweeper {

    class UIComponent {
    public:
        virtual ~UIComponent() = default; // Virtual destructor
        virtual void draw(sf::RenderWindow& window) = 0;       // Render
        virtual void update() = 0;                             // Update state
        virtual bool handleEvent(const sf::Event& event) = 0;  // Handle input
    };
    class Button : public UIComponent {
    private:
        sf::RectangleShape shape;                    // Button background
        std::unique_ptr<sf::Text> label;             // Button text
        std::function<void()> onClick;               // Click callback
        bool hovered;                                // Mouse over flag
        bool pressed;                                // Pressed flag

    public:
        Button();
        Button(const std::string& text, const sf::Font& font,
               const sf::Vector2f& position, const sf::Vector2f& size);

        void setOnClick(std::function<void()> callback) { onClick = callback; }
        void setText(const std::string& text);
        void setPosition(const sf::Vector2f& position);
        void setSize(const sf::Vector2f& size);
        void setColor(const sf::Color& color);

        void draw(sf::RenderWindow& window) override;
        void update() override;
        bool handleEvent(const sf::Event& event) override;

    private:
        void updateAppearance();  // Gen AI: Update colors based on state
    };

    class TextLabel : public UIComponent {
    private:
        std::unique_ptr<sf::Text> text;  // Gen AI: SFML text

    public:
        TextLabel();
        TextLabel(const std::string& content, const sf::Font& font, int size);

        void setString(const std::string& str);
        void setPosition(const sf::Vector2f& position);
        void setColor(const sf::Color& color);

        void draw(sf::RenderWindow& window) override;
        void update() override;
        bool handleEvent(const sf::Event& event) override { return false; }
    };

    class Counter : public UIComponent {
    private:
        TextLabel label;          // Label for display
        int value;                // Current value
        std::string prefix;       // Text prefix

    public:
        Counter();
        Counter(const std::string& prefix, const sf::Font& font, int size);

        void setValue(int val);
        void increment() {setValue(value + 1);}
        void decrement() {setValue(value - 1);}

        void draw(sf::RenderWindow& window) override;
        void update() override;
        bool handleEvent(const sf::Event& event) override {return false;}
    private:
        void updateLabel();  // Format prefix + value
    };
}

#endif //PA9_UICOMPONENTS_H
