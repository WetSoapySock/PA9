#ifndef PA9_RENDERER_H
#define PA9_RENDERER_H

#pragma once
#include <SFML/Graphics.h>
#include "../core/Board.h"
#include "SpriteSheet.h"
#include "UIComponent.h"
#include <memory>
#include <vector>

namespace Minesweeper {
    class Renderer {
    private:
        sf::RenderWindow window;
        SpriteSheet spriteSheet;
        std::vector<std::unique_ptr<UIComponent>> uiComponents;

        // Board rendering
        int boardOffsetX;
        int boardOffsetY;

        // Visual settings
        bool showGridLines;
        bool useCheckerboardPattern;

    public:
        Renderer();
        ~Renderer() = default;

        bool initialize(int width, int height, const std::string& title);
        void clear();
        void display();
        bool isOpen() const { return window.isOpen(); }
        void close() { window.close(); }

        // Board rendering
        void renderBoard(const Board& board);
        void renderCell(const Cell& cell, int x, int y);

        // UI rendering
        void addUIComponent(std::unique_ptr<UIComponent> component);
        void renderUI();

        // Settings
        void setBoardOffset(int x, int y);
        void setShowGridLines(bool show) { showGridLines = show; }
        void setCheckerboardPattern(bool use) { useCheckerboardPattern = use; }

        // Event handling
        bool pollEvent(sf::Event& event);

        // Getters
        sf::RenderWindow& getWindow() { return window; }
        const sf::RenderWindow& getWindow() const { return window; }

    private:
        void calculateBoardOffset(int boardWidth, int boardHeight);
        sf::Color getCellColor(const Cell& cell, int x, int y) const;
    };
}

#endif //PA9_RENDERER_H
