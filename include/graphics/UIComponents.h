#ifndef PA9_UICOMPONENTS_H
#define PA9_UICOMPONENTS_H

#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <memory>

namespace Minesweeper {
    class UIComponent {
    public:
        virtual ~UIComponent() = default;
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void update() = 0;
        virtual bool handleEvent(const sf::Event& event) = 0;
    };

    class Button : public UIComponent {
    private:
        sf::RectangleShape shape;
        std::unique_ptr<sf::Text> label;
        std::function<void()> onClick;
        bool hovered;
        bool pressed;

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
        void updateAppearance();
    };

    class TextLabel : public UIComponent {
    private:
        std::unique_ptr<sf::Text> text;;

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
        TextLabel label;
        int value;
        std::string prefix;

    public:
        Counter();
        Counter(const std::string& prefix, const sf::Font& font, int size);

        void setValue(int val);
        void increment() { setValue(value + 1); }
        void decrement() { setValue(value - 1); }

        void draw(sf::RenderWindow& window) override;
        void update() override;
        bool handleEvent(const sf::Event& event) override { return false; }

    private:
        void updateLabel();
    };
}

#endif //PA9_UICOMPONENTS_H
