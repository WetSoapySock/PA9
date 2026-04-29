#include "../include/graphics/SpriteSheet.h"

Minesweeper::SpriteSheet::SpriteSheet()
{
    cellWidth = 0;
    cellHeight = 0;
    loaded = false;
}

bool Minesweeper::SpriteSheet::loadFromFile(const std::string& filename, int cellW, int cellH)
{
    cellWidth = cellW;
    cellHeight = cellH;

    if (!texture.loadFromFile(filename))
    {
        loaded = false;
        return false;
    }

    loaded = true;
    setupDefaultSprites();

    return true;
}

bool Minesweeper::SpriteSheet::loadDefaultTileset()
{
    return loadFromFile("assets/minesweeper_tiles.png", 32, 32);
}

sf::Sprite Minesweeper::SpriteSheet::getSprite(const std::string& name) const
{
    auto found = spriteRects.find(name);

    if (loaded && found != spriteRects.end())
    {
        return sf::Sprite(texture, found->second);
    }

    return sf::Sprite(texture);
}

sf::Sprite Minesweeper::SpriteSheet::getSprite(int index) const
{
    if (!loaded || cellWidth <= 0 || cellHeight <= 0)
    {
        return sf::Sprite(texture);
    }

    int textureWidth = static_cast<int>(texture.getSize().x);
    int columns = textureWidth / cellWidth;

    if (columns <= 0)
    {
        return sf::Sprite(texture);
    }

    int x = (index % columns) * cellWidth;
    int y = (index / columns) * cellHeight;

    sf::IntRect rect(
        sf::Vector2i(x, y),
        sf::Vector2i(cellWidth, cellHeight)
    );

    return sf::Sprite(texture, rect);
}

void Minesweeper::SpriteSheet::defineSprite(const std::string& name, int x, int y)
{
    spriteRects[name] = sf::IntRect(
        sf::Vector2i(x * cellWidth, y * cellHeight),
        sf::Vector2i(cellWidth, cellHeight)
    );
}

void Minesweeper::SpriteSheet::setupDefaultSprites()
{
    spriteRects.clear();

    defineSprite("covered", 0, 0);
    defineSprite("revealed", 1, 0);
    defineSprite("flag", 2, 0);
    defineSprite("mine", 3, 0);

    defineSprite("empty", 1, 0);
    defineSprite("number1", 0, 1);
    defineSprite("number2", 1, 1);
    defineSprite("number3", 2, 1);
    defineSprite("number4", 3, 1);
    defineSprite("number5", 4, 1);
    defineSprite("number6", 5, 1);
    defineSprite("number7", 6, 1);
    defineSprite("number8", 7, 1);
}