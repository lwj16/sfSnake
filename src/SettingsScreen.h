#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "ColorDefs.h"

namespace sfSnake {
class SettingsScreen : public Screen {
public:
    SettingsScreen(BgColor bg, bool showGrid, GridColor grid);

    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

    BgColor getBgColor() const;
    bool getShowGrid() const;
    GridColor getGridColor() const;

    std::string getBgColorName() const;
    std::string getGridColorName() const;

private:
    sf::Font font_;
    sf::Text snakeText_;
	sf::Text text_;
    
    sf::Text info_;
    BgColor bgColor_;
    bool showGrid_;
    GridColor gridColor_;
};
}

#endif