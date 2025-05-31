#include <SFML/Graphics.hpp>
#include <cmath>

#include "SnakeNode.h"

using namespace sfSnake;

const float SnakeNode::Width = 10.f;
const float SnakeNode::Height = 10.f;

namespace
{
    sf::Texture mankindHeadTexture;
    sf::Texture robotHeadTexture;
    
    void loadTextures() {
        mankindHeadTexture.loadFromFile("../src/Images/MankindHead.png");
        robotHeadTexture.loadFromFile("../src/Images/RobotHead.png");
    }
}

SnakeNode::SnakeNode(sf::Vector2f position, NodeType type, SnakeType snake): position_(position), type_(type), snake_(snake), direction_(0.f, -1.f)
{
    loadTextures();
    if(snake_ == SnakeType::Mankind) {
        if (type_ == NodeType::Head) {
            headSprite_.setTexture(mankindHeadTexture);
            headSprite_.setPosition(position_);
            headSprite_.setScale(
                2.f * SnakeNode::Width / mankindHeadTexture.getSize().x,
                2.f * SnakeNode::Height / mankindHeadTexture.getSize().y
            );
		    headSprite_.setOrigin(mankindHeadTexture.getSize().x / 2.f, mankindHeadTexture.getSize().y / 2.f);
    		headSprite_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
        } else {
		    circle_.setRadius(SnakeNode::Width / 2.f);
    		circle_.setFillColor(sf::Color(254, 217, 34));
	    	circle_.setOrigin(circle_.getRadius(), circle_.getRadius());
		    circle_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);

    		shape_.setSize(sf::Vector2f(SnakeNode::Width / 4, SnakeNode::Height));
	    	shape_.setFillColor(sf::Color::Black);
		    shape_.setOrigin(shape_.getSize().x / 2.f, shape_.getSize().y / 2.f);
    		shape_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
        }
    }
    if(snake_ == SnakeType::Robot) {
        if (type_ == NodeType::Head) {
            headSprite_.setTexture(robotHeadTexture);
            headSprite_.setPosition(position_);
            headSprite_.setScale(
                2.f * SnakeNode::Width / robotHeadTexture.getSize().x,
                2.f * SnakeNode::Height / robotHeadTexture.getSize().y
            );
		    headSprite_.setOrigin(robotHeadTexture.getSize().x / 2.f, robotHeadTexture.getSize().y / 2.f);
    		headSprite_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
        } else {
		    circle_.setRadius(SnakeNode::Width / 2.f);
            circle_.setFillColor(sf::Color(135, 212, 208));

            circle_.setOutlineColor(sf::Color::Black);
            circle_.setOutlineThickness(0.5);

            circle_.setOrigin(circle_.getRadius(), circle_.getRadius());
            circle_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
        }
    }
}

void SnakeNode::setPosition(sf::Vector2f position)
{
    position_ = position;
    if (type_ == NodeType::Head) {
        headSprite_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
    } else {
        shape_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
        circle_.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
    }
}

void SnakeNode::setPosition(float x, float y)
{
    setPosition(sf::Vector2f(x, y));
}

void SnakeNode::move(float xOffset, float yOffset)
{
    setPosition(position_.x + xOffset, position_.y + yOffset);
}

sf::FloatRect SnakeNode::getBounds() const
{
    sf::FloatRect bounds;
    float scale;
    if (type_ == NodeType::Head) {
        bounds = headSprite_.getGlobalBounds();
        scale = 0.3f;
    }
    else {
        bounds = circle_.getGlobalBounds();
        scale = 0.5f;
    }
    float newWidth = bounds.width * scale;
    float newHeight = bounds.height * scale;
    float newLeft = bounds.left + (bounds.width - newWidth) / 2.f;
    float newTop = bounds.top + (bounds.height - newHeight) / 2.f;
    return sf::FloatRect(newLeft, newTop, newWidth, newHeight);
}

sf::Vector2f SnakeNode::getPosition() const
{
	return position_;
}

void SnakeNode::setDirection(const sf::Vector2f& dir) 
{
	direction_ = dir; 
}

void SnakeNode::render(sf::RenderWindow& window)
{
    if (type_ == NodeType::Head) {
		float angle = std::atan2(direction_.y, direction_.x) * 180.f / 3.1415926f;
        headSprite_.setRotation(angle);
        window.draw(headSprite_);
    } else {
        float angle = std::atan2(direction_.y, direction_.x) * 180.f / 3.1415926f;
        shape_.setRotation(angle);
		window.draw(circle_);
        window.draw(shape_);
    }
/*
    sf::FloatRect bounds = getBounds();
    sf::RectangleShape collisionBox(sf::Vector2f(bounds.width, bounds.height));
    collisionBox.setPosition(bounds.left, bounds.top);
    collisionBox.setFillColor(sf::Color::Transparent);
    collisionBox.setOutlineColor(sf::Color::Red);
    collisionBox.setOutlineThickness(1.f);
    window.draw(collisionBox);
*/
}