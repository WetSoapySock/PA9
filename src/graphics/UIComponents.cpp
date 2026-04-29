#include "../include/graphics/UIComponents.h"

// ==================== Button ====================

Minesweeper::Button::Button()
{
    hovered = false;
    pressed = false;
    onClick = nullptr;
    label = nullptr;
}

Minesweeper::Button::Button(const std::string& text,
    const sf::Font& font,
    const sf::Vector2f& position,
    const sf::Vector2f& size)
{
    hovered = false;
    pressed = false;
    onClick = nullptr;

    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(170, 170, 170));

    label = std::make_unique<sf::Text>(font, text, 24);
    label->setFillColor(sf::Color::Black);

    sf::FloatRect bounds = label->getLocalBounds();

    label->setPosition(sf::Vector2f(
        position.x + (size.x - bounds.size.x) / 2,
        position.y + (size.y - bounds.size.y) / 2 - 5
    ));
}

void Minesweeper::Button::setText(const std::string& text)
{
    if (label != nullptr)
    {
        label->setString(text);
    }
}

void Minesweeper::Button::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);

    if (label != nullptr)
    {
        label->setPosition(position);
    }
}

void Minesweeper::Button::setSize(const sf::Vector2f& size)
{
    shape.setSize(size);
}

void Minesweeper::Button::setColor(const sf::Color& color)
{
    shape.setFillColor(color);
}

void Minesweeper::Button::draw(sf::RenderWindow& window)
{
    window.draw(shape);

    if (label != nullptr)
    {
        window.draw(*label);
    }
}

void Minesweeper::Button::update()
{
    updateAppearance();
}

bool Minesweeper::Button::handleEvent(const sf::Event& event)
{
    if (const auto* mouse = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mousePosition(
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        );

        hovered = shape.getGlobalBounds().contains(mousePosition);
        updateAppearance();
    }

    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f mousePosition(
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        );

        if (shape.getGlobalBounds().contains(mousePosition))
        {
            pressed = true;
            updateAppearance();
            return true;
        }
    }

    if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>())
    {
        sf::Vector2f mousePosition(
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        );

        bool wasPressed = pressed;
        pressed = false;
        updateAppearance();

        if (wasPressed && shape.getGlobalBounds().contains(mousePosition))
        {
            if (onClick != nullptr)
            {
                onClick();
            }

            return true;
        }
    }

    return false;
}

void Minesweeper::Button::updateAppearance()
{
    if (pressed)
    {
        shape.setFillColor(sf::Color(120, 120, 120));
    }
    else if (hovered)
    {
        shape.setFillColor(sf::Color(200, 200, 200));
    }
    else
    {
        shape.setFillColor(sf::Color(170, 170, 170));
    }
}

// ==================== TextLabel ====================

Minesweeper::TextLabel::TextLabel()
{
    text = nullptr;
}

Minesweeper::TextLabel::TextLabel(const std::string& content,
    const sf::Font& font,
    int size)
{
    text = std::make_unique<sf::Text>(font, content, size);
    text->setFillColor(sf::Color::White);
}

void Minesweeper::TextLabel::setString(const std::string& str)
{
    if (text != nullptr)
    {
        text->setString(str);
    }
}

void Minesweeper::TextLabel::setPosition(const sf::Vector2f& position)
{
    if (text != nullptr)
    {
        text->setPosition(position);
    }
}

void Minesweeper::TextLabel::setColor(const sf::Color& color)
{
    if (text != nullptr)
    {
        text->setFillColor(color);
    }
}

void Minesweeper::TextLabel::draw(sf::RenderWindow& window)
{
    if (text != nullptr)
    {
        window.draw(*text);
    }
}

void Minesweeper::TextLabel::update()
{
    // Nothing needed for now
}

// ==================== Counter ====================

Minesweeper::Counter::Counter()
{
    value = 0;
    prefix = "";
}

Minesweeper::Counter::Counter(const std::string& newPrefix,
    const sf::Font& font,
    int size)
    : label(newPrefix + "0", font, size)
{
    value = 0;
    prefix = newPrefix;
}

void Minesweeper::Counter::setValue(int val)
{
    value = val;
    updateLabel();
}

void Minesweeper::Counter::draw(sf::RenderWindow& window)
{
    label.draw(window);
}

void Minesweeper::Counter::update()
{
    label.update();
}

void Minesweeper::Counter::updateLabel()
{
    label.setString(prefix + std::to_string(value));
}