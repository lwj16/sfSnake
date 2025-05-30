#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "ColorDefs.h"

namespace sfSnake
{
class GameOverScreen : public Screen
{
public:
	GameOverScreen(int score, BgColor bg, bool showGrid, GridColor grid);

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;

private:
	sf::Font font_;
	sf::Text text_;

	std::size_t score_;
    BgColor bgColor_;
    bool showGrid_;
    GridColor gridColor_;
};
}

#endif