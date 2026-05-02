#ifndef PA9_RENDERER_H
#define PA9_RENDERER_H

#pragma once
#include <SFML/Graphics.hpp>              
#include "../core/board.h"                
#include "spriteSheet.h"                  
#include "UIComponents.h"                 
#include <memory>                         
#include <vector>                         
#include <iostream>

/** Gen AI, "How do we configurate a class we need for SFML rendering? 
    We need a Renderer class that can draw the game board, cells, and UI components. 
    It should handle loading a sprite sheet for the tiles and have methods to render 
    the board and individual cells based on their state. **/

namespace Minesweeper {
    class Renderer {
    private:
        // Gen AI, SFML utilities
        sf::RenderWindow window;                              
        SpriteSheet spriteSheet;                              
        std::vector<std::unique_ptr<UIComponent>> uiComponents; 

        int boardOffsetX;          
        int boardOffsetY;          
        bool showGridLines;        
        bool useCheckerboardPattern;    // Checkerboard pattern

        sf::Font font;             // UI font
        bool fontLoaded;           // Font load status
    public:
        // Constructor and destructor
        Renderer();                
        ~Renderer() = default;    

        bool initialize(int width, int height, const std::string& title); // Initialize window
        void clear();              
        void display();           
        bool isOpen() const {return window.isOpen();}
        void close() {window.close();}

        void renderBoard(const Board& board);      // Renders all cells
        void renderCell(const Cell& cell, int x, int y); // Render a single cell 

        void addUIComponent(std::unique_ptr<UIComponent> component); // Add UI element
        void renderUI();                                 // Draw UI slements

        void setShowGridLines(bool show) {showGridLines = show;}
        void setCheckerboardPattern(bool use) {useCheckerboardPattern = use;}

        sf::RenderWindow& getWindow() {return window;}
        const sf::RenderWindow& getWindow() const {return window;}
        sf::Font& getFont() {return font;}
        const sf::Font& getFont() const {return font;}
    private:
        void calculateBoardOffset(int boardWidth, int boardHeight); // Center board
        sf::Color getCellColor(const Cell& cell, int x, int y) const; // Cell colors

        void renderCellNumber(const Cell& cell, int x, int y);      
        sf::Color getNumberColor(int number) const;                
    };
}

#endif //PA9_RENDERER_H
