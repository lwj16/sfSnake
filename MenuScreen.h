#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "SetColor.h"

namespace sfSnake
{
class MenuScreen : public Screen
{
public:
	MenuScreen();

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

	BgColor bgColor_ = BgColor::White;
    bool showGrid_ = false;
    GridColor gridColor_ = GridColor::Black;
};
}

#endif