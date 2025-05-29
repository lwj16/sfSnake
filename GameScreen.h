#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "Snake.h"
#include "Fruit.h"
#include "ColorDefs.h"

namespace sfSnake
{
class GameScreen : public Screen
{
public:
	GameScreen(BgColor bg = BgColor::White, bool showGrid = false, GridColor grid = GridColor::Black);

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;

	void generateFruit();

	void setBackground(BgColor color);
    void setGrid(bool show, GridColor color);

private:
	Snake snake_;
	std::vector<Fruit> fruit_;

	BgColor bgColor_ = BgColor::White;
    bool showGrid_ = false;
    GridColor gridColor_ = GridColor::Black;
};
}

#endif