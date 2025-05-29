#include <SFML/Graphics.hpp>

#include <memory>

#include "GameScreen.h"
#include "SettingsScreen.h"
#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

SettingsScreen::SettingsScreen(BgColor bg, bool showGrid, GridColor grid): bgColor_(bg), showGrid_(showGrid), gridColor_(grid)
{
    font_.loadFromFile("Fonts/game_over.ttf");
	text_.setFont(font_);
    std::string bgName = getBgColorName();
	std::string gridName = getGridColorName();
	std::string gridStatus = showGrid_ ? "Opened" : "Closed";
	text_.setString(
		"\n\n\n\n[B] : Set BgColor : " + bgName +
		"\n\n[G] : Enable Grid: " + gridStatus +
		"\n\n[C] : Set GridColor: " + gridName +
        "\n\n[ESC] / [SPACE] : EXIT"
	);
	snakeText_.setFont(font_);
	snakeText_.setString("Settings");
	snakeText_.setFillColor(sf::Color(12, 145, 175));
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

void SettingsScreen::handleInput(sf::RenderWindow& window)
{
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        // 这里需要把 bgColor_、showGrid_、gridColor_ 传回 MenuScreen
        Game::Screen = std::make_shared<MenuScreen>(bgColor_, showGrid_, gridColor_);
    }
}

void SettingsScreen::update(sf::Time delta)
{
	
}

BgColor SettingsScreen::getBgColor() const { return bgColor_; }
bool SettingsScreen::getShowGrid() const { return showGrid_; }
GridColor SettingsScreen::getGridColor() const { return gridColor_; }

std::string SettingsScreen::getBgColorName() const {
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

std::string SettingsScreen::getGridColorName() const {
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

void SettingsScreen::render(sf::RenderWindow& window)
{
    window.draw(snakeText_);
    std::string bgName = getBgColorName();
	std::string gridName = getGridColorName();
	std::string gridStatus = showGrid_ ? "Opened" : "Closed";
	text_.setString(
		"\n\n\n\n[B] : Set BgColor : " + bgName +
		"\n\n[G] : Enable Grid: " + gridStatus +
		"\n\n[C] : Set GridColor: " + gridName +
        "\n\n[ESC] / [SPACE] : EXIT"
	);
	window.draw(text_);
}