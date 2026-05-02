#include "../include/graphics/spriteSheet.h"

/*
 Function: SpriteSheet (constructor)
 Description: Initializes sprite sheet with zero dimensions and sets loaded flag to false.
              Called before loading any texture file.
*/
Minesweeper::SpriteSheet::SpriteSheet()
{
    cellWidth = 0;
    cellHeight = 0;
    loaded = false;
}

/*
 Function: loadFromFile
 Description: Loads a sprite sheet texture from the specified file and sets the
              dimensions for individual tiles. Sets up default sprite mappings if load succeeds.
              Returns true if texture loaded successfully, false otherwise.
              Parameters: filename = path to image file, cellW, cellH = tile dimensions in pixels
*/
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

/*
 Function: loadDefaultTileset
 Description: Convenience method that loads the default Minesweeper tileset
              from "assets/minesweeper_tiles.png" with 32x32 pixel tiles.
              Returns true if load succeeds, false otherwise.
*/
bool Minesweeper::SpriteSheet::loadDefaultTileset()
{
    return loadFromFile("assets/minesweeper_tiles.png", 32, 32);
}

/*
 Function: getSprite (by name)
 Description: Returns a sprite for the named tile (e.g., "covered", "flag", "mine").
              If the name is not found or texture not loaded, returns an empty sprite.
              Parameters: name = string identifier for the desired sprite
*/
sf::Sprite Minesweeper::SpriteSheet::getSprite(const std::string& name) const
{
    auto found = spriteRects.find(name);

    if (loaded && found != spriteRects.end())
    {
        return sf::Sprite(texture, found->second);
    }

    return sf::Sprite(texture);
}

/*
 Function: getSprite (by index)
 Description: Returns a sprite by index number for tile sheet layouts.
              Calculates X and Y position in the sprite sheet based on index,
              cell width/height, and number of columns in the texture.
              Parameters: index = zero-based tile index in left-to-right, top-to-bottom order
*/
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

/*
 Function: defineSprite
 Description: Maps a name to a specific tile position in the sprite sheet.
              Uses grid coordinates (x,y) where each grid cell is cellWidth x cellHeight pixels.
              Parameters: name = identifier for the sprite, x, y = grid coordinates of tile
*/
void Minesweeper::SpriteSheet::defineSprite(const std::string& name, int x, int y)
{
    spriteRects[name] = sf::IntRect(
        sf::Vector2i(x * cellWidth, y * cellHeight),
        sf::Vector2i(cellWidth, cellHeight)
    );
}

/*
 Function: setupDefaultSprites
 Description: Internal helper that defines the standard sprite mappings for Minesweeper.
              Sets up covered, revealed, flag, mine, empty, and number tiles (1-8).
              Called automatically after loading a texture file.
*/
void Minesweeper::SpriteSheet::setupDefaultSprites()
{
    spriteRects.clear();

    // Row 0: Basic tiles
    defineSprite("covered", 0, 0);
    defineSprite("revealed", 1, 0);
    defineSprite("flag", 2, 0);
    defineSprite("mine", 3, 0);

    // Row 1: Number tiles
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
