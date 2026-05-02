#include "../include/graphics/UIComponents.h"

/*
 Function: Button (default constructor)
 Description: Initializes button with default state: not hovered, not pressed,
              no click callback, and null label pointer.
*/
Minesweeper::Button::Button()
{
    hovered = false;
    pressed = false;
    onClick = nullptr;
    label = nullptr;
}

/*
 Function: Button (parameterized constructor)
 Description: Creates a button with specified text, font, position, and size.
              Centers the text within the button bounds automatically.
              Parameters: text = button label, font = SFML font object,
              position = top-left coordinates, size = width and height in pixels
*/
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

/*
 Function: setText
 Description: Changes the button's displayed text. Updates the internal
              label string while preserving all other text properties.
              Parameters: text = new button label string
*/
void Minesweeper::Button::setText(const std::string& text)
{
    if (label != nullptr)
    {
        label->setString(text);
    }
}

/*
 Function: setPosition
 Description: Moves the button to a new position on screen. Updates both
              the button shape and its centered text label.
              Parameters: position = new top-left coordinates
*/
void Minesweeper::Button::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);

    if (label != nullptr)
    {
        label->setPosition(position);
    }
}

/*
 Function: setSize
 Description: Resizes the button rectangle. Does not automatically re-center text.
              Parameters: size = new width and height in pixels
*/
void Minesweeper::Button::setSize(const sf::Vector2f& size)
{
    shape.setSize(size);
}

/*
 Function: setColor
 Description: Changes the button's background color.
              Parameters: color = new SFML color for button fill
*/
void Minesweeper::Button::setColor(const sf::Color& color)
{
    shape.setFillColor(color);
}

/*
 Function: draw
 Description: Renders the button to the window. Draws the shape rectangle
              first, then the text label on top.
              Parameters: window = SFML render window to draw on
*/
void Minesweeper::Button::draw(sf::RenderWindow& window)
{
    window.draw(shape);

    if (label != nullptr)
    {
        window.draw(*label);
    }
}

/*
 Function: update
 Description: Updates button state each frame. Currently just updates
              appearance based on hover/pressed state.
*/
void Minesweeper::Button::update()
{
    updateAppearance();
}

/*
 Function: handleEvent
 Description: Processes mouse events for button interaction. Handles hover detection,
              press state tracking, and click callbacks. Returns true if event was consumed.
              Parameters: event = SFML event to process (mouse move, press, release)
*/
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

/*
 Function: updateAppearance
 Description: Internal helper that changes button color based on state.
              Pressed = dark gray, Hovered = light gray, Normal = medium gray.
*/
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

/*
 Function: TextLabel (default constructor)
 Description: Initializes empty text label with null text pointer.
*/
Minesweeper::TextLabel::TextLabel()
{
    text = nullptr;
}

/*
 Function: TextLabel (parameterized constructor)
 Description: Creates a text label with specified content, font, and size.
              Sets default text color to white.
              Parameters: content = displayed string, font = SFML font object, size = font size in pixels
*/
Minesweeper::TextLabel::TextLabel(const std::string& content,
    const sf::Font& font,
    int size)
{
    text = std::make_unique<sf::Text>(font, content, size);
    text->setFillColor(sf::Color::White);
}

/*
 Function: setString
 Description: Changes the displayed text content of the label.
              Parameters: str = new text string to display
*/
void Minesweeper::TextLabel::setString(const std::string& str)
{
    if (text != nullptr)
    {
        text->setString(str);
    }
}

/*
 Function: setPosition
 Description: Moves the text label to a new screen position.
              Parameters: position = new top-left coordinates for text
*/
void Minesweeper::TextLabel::setPosition(const sf::Vector2f& position)
{
    if (text != nullptr)
    {
        text->setPosition(position);
    }
}

/*
 Function: setColor
 Description: Changes the text color of the label.
              Parameters: color = new SFML color for the text
*/
void Minesweeper::TextLabel::setColor(const sf::Color& color)
{
    if (text != nullptr)
    {
        text->setFillColor(color);
    }
}

/*
 Function: draw
 Description: Renders the text label to the window.
              Parameters: window = SFML render window to draw on
*/
void Minesweeper::TextLabel::draw(sf::RenderWindow& window)
{
    if (text != nullptr)
    {
        window.draw(*text);
    }
}

/*
 Function: update
 Description: Placeholder update function for text label. No state to update.
*/
void Minesweeper::TextLabel::update()
{
    // Nothing needed for now
}

// ======================== COUNTER CLASS ========================

/*
 Function: Counter (default constructor)
 Description: Initializes counter with zero value and empty prefix.
*/
Minesweeper::Counter::Counter()
{
    value = 0;
    prefix = "";
}

/*
 Function: Counter (parameterized constructor)
 Description: Creates a counter display with prefix (e.g., "Mines: ") and
              initial value of 0. Uses the provided font and size.
              Parameters: newPrefix = text before the number, font = SFML font, size = font size
*/
Minesweeper::Counter::Counter(const std::string& newPrefix,
    const sf::Font& font,
    int size)
    : label(newPrefix + "0", font, size)
{
    value = 0;
    prefix = newPrefix;
}

/*
 Function: setValue
 Description: Updates the counter's numeric value and refreshes the display text.
              Parameters: val = new integer value to display
*/
void Minesweeper::Counter::setValue(int val)
{
    value = val;
    updateLabel();
}

/*
 Function: draw
 Description: Renders the counter label to the window.
              Parameters: window = SFML render window to draw on
*/
void Minesweeper::Counter::draw(sf::RenderWindow& window)
{
    label.draw(window);
}

/*
 Function: update
 Description: Updates the counter state each frame. Delegates to label update.
*/
void Minesweeper::Counter::update()
{
    label.update();
}

/*
 Function: updateLabel
 Description: Internal helper that refreshes the display text by combining
              prefix with current value (e.g., "Mines: 10").
*/
void Minesweeper::Counter::updateLabel()
{
    label.setString(prefix + std::to_string(value));
}
