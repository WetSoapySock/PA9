#ifndef PA9_SPRITESHEET_H
#define PA9_SPRITESHEET_H

#pragma once
// SFML Libraries
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Gen AI, Libraries
#include <unordered_map>
#include <string>

/** Gen AI, "We need some help with SFML nad rendering graphics.
    Can you help us create a SpriteSheet class that can load a
    sprite sheet image and define named regions for each sprite?
    We also want it to handle loading a default tileset for our Minesweeper game." **/

namespace Minesweeper {
    class SpriteSheet {
    private:
        // Gen AI, Sprite sheet images
        sf::Texture texture;

        // Gen AI: Named sprite regions
        std::unordered_map<std::string, sf::IntRect> spriteRects;
        int cellWidth; // Tile width in pixels
        int cellHeight;  // Tile height in pixels
        bool loaded; // Load successful flag

    public:
        SpriteSheet();
        ~SpriteSheet() = default;

        // Gen AI, Loads sheet
        bool loadFromFile(const std::string& filename, int cellW, int cellH);

        // Gen AI, Loads images/tiles.jpg with 32x32 cells
        bool loadDefaultTileset();

        sf::Sprite getSprite(const std::string& name) const;  // Sprite by name
        sf::Sprite getSprite(int index) const;                // Sprite by index

        // Gen AI, Maps name to grid pos
        void defineSprite(const std::string& name, int x, int y);

        bool isLoaded() const {return loaded;}
        int getCellWidth() const {return cellWidth;}
        int getCellHeight() const {return cellHeight;}
    private:
        // Gen AI, Init sprite mappings
        void setupDefaultSprites(); 
    };
}

#endif //PA9_SPRITESHEET_H
