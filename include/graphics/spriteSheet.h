#ifndef PA9_SPRITESHEET_H
#define PA9_SPRITESHEET_H

#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <string>

namespace Minesweeper {
    class SpriteSheet {
    private:
        sf::Texture texture;
        std::unordered_map<std::string, sf::IntRect> spriteRects;
        int cellWidth;
        int cellHeight;
        bool loaded;

    public:
        SpriteSheet();
        ~SpriteSheet() = default;

        bool loadFromFile(const std::string& filename, int cellW, int cellH);
        bool loadDefaultTileset();

        sf::Sprite getSprite(const std::string& name) const;
        sf::Sprite getSprite(int index) const;

        void defineSprite(const std::string& name, int x, int y);

        bool isLoaded() const { return loaded; }
        int getCellWidth() const { return cellWidth; }
        int getCellHeight() const { return cellHeight; }

    private:
        void setupDefaultSprites();
    };
}

#endif //PA9_SPRITESHEET_H
