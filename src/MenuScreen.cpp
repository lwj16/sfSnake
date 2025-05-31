#include <SFML/Graphics.hpp>

#include <memory>

#include "GameScreen.h"
#include "MenuScreen.h"
#include "SettingsScreen.h"
#include "Game.h"

using namespace sfSnake;

MenuScreen::MenuScreen(BgColor bg, bool showGrid, GridColor grid) : bgColor_(bg), showGrid_(showGrid), gridColor_(grid)
{
	font_.loadFromFile("../src/Fonts/game_over.ttf");
	text_.setFont(font_);
	text_.setString(
		"\n\n\n\n\n\n\n\n\nPress [SPACE] to play"
		"\n\nPress [ESC] to quit"
		"\n\nPress [S] to settings");

	snakeText_.setFont(font_);
	snakeText_.setString("Snake!");
	snakeText_.setFillColor(sf::Color(0, 255, 0));
	snakeText_.setCharacterSize(64);
	snakeText_.setStyle(sf::Text::Bold);

	sf::FloatRect textBounds = text_.getLocalBounds();
	text_.setOrigin(textBounds.left + textBounds.width / 2,
		textBounds.top + textBounds.height / 2);
	text_.setPosition(Game::Width / 2, Game::Height / 2);

	sf::FloatRect snakeTextBounds = snakeText_.getLocalBounds();
	snakeText_.setOrigin(snakeTextBounds.left + snakeTextBounds.width / 2,
		snakeTextBounds.top + snakeTextBounds.height / 2);
	snakeText_.setPosition(Game::Width / 2, Game::Height / 4);
}

void MenuScreen::handleInput(sf::RenderWindow& window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        Game::Screen = std::make_shared<GameScreen>(bgColor_, showGrid_, gridColor_);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        Game::Screen = std::make_shared<SettingsScreen>(bgColor_, showGrid_, gridColor_);
    }
}

void MenuScreen::update(sf::Time delta)
{
	static bool movingLeft = false;
	static bool movingRight = true;

	if (movingRight)
	{
		snakeText_.rotate(delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == 10)
		{
			movingRight = false;
			movingLeft = true;
		}
	}

	if (movingLeft)
	{
		snakeText_.rotate(-delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == (360 - 10))
		{
			movingLeft = false;
			movingRight = true;
		}
	}
}

BgColor MenuScreen::getBgColor() const { return bgColor_; }
bool MenuScreen::getShowGrid() const { return showGrid_; }
GridColor MenuScreen::getGridColor() const { return gridColor_; }

std::string MenuScreen::getBgColorName() const {
	switch (bgColor_) {
        case BgColor::White:
            return "White";
        case BgColor::Black:
            return "Black";
        case BgColor::Brown:
            return "Brown";
        default:
            return "Unknown";
    }
}

std::string MenuScreen::getGridColorName() const {
	switch (gridColor_) {
        case GridColor::White:
            return "White";
        case GridColor::Black:
            return "Black";
        case GridColor::Brown:
            return "Brown";
        default:
            return "Unknown";
    }
}

void MenuScreen::render(sf::RenderWindow& window)
{
	window.draw(text_);
	window.draw(snakeText_);
}