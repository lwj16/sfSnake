#include <SFML/Graphics.hpp>

#include <memory>

#include "GameScreen.h"
#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

MenuScreen::MenuScreen()
{
	font_.loadFromFile("Fonts/game_over.ttf");
	text_.setFont(font_);
	text_.setString(
		"\n\n\n\n\n\n\n\n\nPress [SPACE] to play"
		"\n\nPress [ESC] to quit");

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

    // 背景色切换：B键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
        if (bgColor_ == BgColor::White)
            bgColor_ = BgColor::Black;
        else if (bgColor_ == BgColor::Black)
            bgColor_ = BgColor::Brown;
        else
            bgColor_ = BgColor::White;
    }

    // 网格开关：G键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
        showGrid_ = !showGrid_;
    }

    // 网格颜色切换：C键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        if (gridColor_ == GridColor::White)
            gridColor_ = GridColor::Black;
        else if (gridColor_ == GridColor::Black)
            gridColor_ = GridColor::Brown;
        else
            gridColor_ = GridColor::White;
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

	sf::Text info;
	info.setFont(font_);
	info.setCharacterSize(24);
	info.setFillColor(sf::Color::Blue);
	std::string bgName = getBgColorName();
	std::string gridName = getGridColorName();
	std::string gridStatus = showGrid_ ? "Opened" : "Closed";
	info.setString(
		"[B]:Set BgColor  [G]:Enable Grid  [C]:Set GridColor\n"
		"BackGround: " + bgName +
		"  Grid: " + gridStatus +
		"  GridColor: " + gridName
	);
	info.setPosition(20, Game::Height - 60);
	window.draw(info);
}