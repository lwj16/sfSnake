#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>
#include <cmath>

#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;

Snake::Snake() : direction_(Direction::Up), hitSelf_(false), useMouseDirection_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/pickup.wav");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);
}

void Snake::initNodes()
{
    nodes.clear();
    for (int i = 0; i < Snake::InitialSize; ++i)
    {
        SnakeNode::NodeType type = (i == 0) ? SnakeNode::NodeType::Head : SnakeNode::NodeType::Body;
        SnakeNode::SnakeType snake = SnakeNode::SnakeType::Mankind;
        nodes.push_back(SnakeNode(
            sf::Vector2f(
                Game::Width / 3 - SnakeNode::Width / 2,
                Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i)
            ),
            type, snake
        ));
    }
}

void Snake::handleInput(sf::RenderWindow& window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction_ = Direction::Up;
        useMouseDirection_ = false;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction_ = Direction::Down;
        useMouseDirection_ = false;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction_ = Direction::Left;
        useMouseDirection_ = false;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction_ = Direction::Right;
        useMouseDirection_ = false;
    }

    // 鼠标左键按下时，设置方向向量
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f headPos = nodes[0].getPosition();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

        sf::Vector2f dir = mouseWorldPos - headPos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.1f) {
            mouseDirection_ = sf::Vector2f(dir.x / len, dir.y / len); // 单位向量
            useMouseDirection_ = true;
        }
    }
}

void Snake::update(sf::Time delta)
{
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit>& fruits)
{
    auto toRemove = fruits.end();
    int growCount = 0;

    for (auto it = fruits.begin(); it != fruits.end(); ++it)
    {
        if (it->getBounds().intersects(nodes[0].getBounds())) {
            toRemove = it;
            growCount = it->getGrowLength();
        }
    }

    if (toRemove != fruits.end())
    {
        pickupSound_.play();
        for (int i = 0; i < growCount; ++i)
            grow();
        fruits.erase(toRemove);
    }
}

void Snake::grow()
{
    sf::Vector2f tailPos = nodes.back().getPosition();
    sf::Vector2f newNodePos;

    if (useMouseDirection_) {
        // 反向延长一个单位
        newNodePos = tailPos - sf::Vector2f(mouseDirection_.x * SnakeNode::Width, mouseDirection_.y * SnakeNode::Height);
    } else {
        switch (direction_)
        {
        case Direction::Up:
            newNodePos = tailPos + sf::Vector2f(0, SnakeNode::Height);
            break;
        case Direction::Down:
            newNodePos = tailPos + sf::Vector2f(0, -SnakeNode::Height);
            break;
        case Direction::Left:
            newNodePos = tailPos + sf::Vector2f(SnakeNode::Width, 0);
            break;
        case Direction::Right:
            newNodePos = tailPos + sf::Vector2f(-SnakeNode::Width, 0);
            break;
        }
    }
    nodes.push_back(SnakeNode(newNodePos, SnakeNode::NodeType::Body, SnakeNode::SnakeType::Mankind));
}

unsigned Snake::getSize() const
{
	return nodes.size();
}

bool Snake::hitSelf() const
{
	return hitSelf_;
}

void Snake::checkSelfCollisions()
{
	SnakeNode& headNode = nodes[0];

	for (decltype(nodes.size()) i = 1; i < nodes.size(); ++i)
	{
		if (headNode.getBounds().intersects(nodes[i].getBounds()))
		{
			dieSound_.play();
			sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
			hitSelf_ = true;
		}
	}
}

void Snake::checkEdgeCollisions()
{
	SnakeNode& headNode = nodes[0];

	if (headNode.getPosition().x <= 0)
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	else if (headNode.getPosition().x >= Game::Width)
		headNode.setPosition(0, headNode.getPosition().y);
	else if (headNode.getPosition().y <= 0)
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	else if (headNode.getPosition().y >= Game::Height)
		headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move()
{
    for (decltype(nodes.size()) i = nodes.size() - 1; i > 0; --i)
    {
        sf::Vector2f prevPos = nodes[i - 1].getPosition();
        sf::Vector2f currPos = nodes[i].getPosition();
        sf::Vector2f dir = prevPos - currPos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir = sf::Vector2f(dir.x / len, dir.y / len);
        nodes[i].setDirection(dir);
        nodes[i].setPosition(prevPos);
    }

    sf::Vector2f headDir;
    if (useMouseDirection_)
        headDir = mouseDirection_;
    else
    {
        switch (direction_)
        {
        case Direction::Up:    headDir = {0, -1}; break;
        case Direction::Down:  headDir = {0, 1};  break;
        case Direction::Left:  headDir = {-1, 0}; break;
        case Direction::Right: headDir = {1, 0};  break;
        }
    }
    nodes[0].setDirection(headDir);

    if (useMouseDirection_)
        nodes[0].move(mouseDirection_.x * SnakeNode::Width, mouseDirection_.y * SnakeNode::Height);
    else
    {
        switch (direction_)
        {
        case Direction::Up:    nodes[0].move(0, -SnakeNode::Height); break;
        case Direction::Down:  nodes[0].move(0, SnakeNode::Height);  break;
        case Direction::Left:  nodes[0].move(-SnakeNode::Width, 0);  break;
        case Direction::Right: nodes[0].move(SnakeNode::Width, 0);   break;
        }
    }
}

void Snake::render(sf::RenderWindow& window)
{
	for (auto& node : nodes)
		node.render(window);
}
