#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "Game.h"
#include "GameScreen.h"
#include "MenuScreen.h"
#include "GameOverScreen.h"

using namespace sfSnake;

GameOverScreen::GameOverScreen(int score, BgColor bg, bool showGrid, GridColor grid): score_(score), bgColor_(bg), showGrid_(showGrid), gridColor_(grid)
{
    font_.loadFromFile("../src/Fonts/game_over.ttf");
    text_.setFont(font_);
    text_.setString("Your score: " + std::to_string(score) + "!"
        "\n\nPress [SPACE] to retry"
        "\n\nPress [ESC] to quit");
    text_.setFillColor(sf::Color::Red);

    sf::FloatRect textBounds = text_.getLocalBounds();
    text_.setOrigin(textBounds.left + textBounds.width / 2,
        textBounds.top + textBounds.height / 2);
    text_.setPosition(Game::Width / 2, Game::Height / 2);
}

void GameOverScreen::handleInput(sf::RenderWindow& window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        Game::Screen = std::make_shared<MenuScreen>(bgColor_, showGrid_, gridColor_);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
}

void GameOverScreen::update(sf::Time delta)
{

}

void GameOverScreen::render(sf::RenderWindow& window)
{
	window.draw(text_);
}