#include <SFML/Graphics.hpp>

#include <random>
#include <memory>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;

GameScreen::GameScreen(BgColor bg, bool showGrid, GridColor grid) : bgColor_(bg), showGrid_(showGrid), gridColor_(grid), snake_()
{

}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	snake_.handleInput(window);
}

void GameScreen::update(sf::Time delta)
{
	if (fruit_.size() == 0)
		generateFruit();

	snake_.update(delta);
	snake_.checkFruitCollisions(fruit_);

	if (snake_.hitSelf())
		Game::Screen = std::make_shared<GameOverScreen>(snake_.getSize(), bgColor_, showGrid_, gridColor_);
}

void GameScreen::render(sf::RenderWindow& window)
{
	sf::Color bg;
    switch (bgColor_) {
        case BgColor::White: bg = sf::Color(235, 239, 242); break;
        case BgColor::Black: bg = sf::Color(57, 58, 59); break;
        case BgColor::Brown: bg = sf::Color(158, 88, 28); break;
    }
    window.clear(bg);

	if (showGrid_) {
        sf::Color grid;
        switch (gridColor_) {
            case GridColor::White: grid = sf::Color(235, 239, 242); break;
            case GridColor::Black: grid = sf::Color(57, 58, 59); break;
            case GridColor::Brown: grid = sf::Color(158, 88, 28); break;
        }
        int gridWidth = Game::Width / SnakeNode::Width;
        int gridHeight = Game::Height / SnakeNode::Height;
        for (int i = 0; i <= gridWidth; ++i) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(i * SnakeNode::Width, 0), grid),
                sf::Vertex(sf::Vector2f(i * SnakeNode::Width, Game::Height), grid)
            };
            window.draw(line, 2, sf::Lines);
        }
        for (int j = 0; j <= gridHeight; ++j) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(0, j * SnakeNode::Height), grid),
                sf::Vertex(sf::Vector2f(Game::Width, j * SnakeNode::Height), grid)
            };
            window.draw(line, 2, sf::Lines);
        }
    }

	snake_.render(window);
 
	for (auto fruit : fruit_)
		fruit.render(window);
}

void GameScreen::setBackground(BgColor color)
{
    bgColor_ = color;
}

void GameScreen::setGrid(bool show, GridColor color)
{
    showGrid_ = show;
    gridColor_ = color;
}

void GameScreen::generateFruit()
{
    static std::default_random_engine engine(static_cast<unsigned>(time(NULL)));

    int gridWidth = Game::Width / SnakeNode::Width;
    int gridHeight = Game::Height / SnakeNode::Height;
    static std::uniform_int_distribution<int> xDistribution(0, gridWidth - 1);
    static std::uniform_int_distribution<int> yDistribution(0, gridHeight - 1);

    int x = xDistribution(engine) * SnakeNode::Width;
    int y = yDistribution(engine) * SnakeNode::Height;

    // 25%黑/棕，75%红/蓝/绿
    static std::uniform_int_distribution<int> typeDist(0, 99);
    int r = typeDist(engine);
    FruitType type;
    if (r < 12) type = FruitType::Black;         // 12%
    else if (r < 25) type = FruitType::Brown;    // 13%
    else {
        static std::uniform_int_distribution<int> colorDist(0, 2);
        int c = colorDist(engine);
        if (c == 0) type = FruitType::Red;
        else if (c == 1) type = FruitType::Blue;
        else type = FruitType::Green;
    }

    fruit_.push_back(Fruit(sf::Vector2f(x, y), type));
}