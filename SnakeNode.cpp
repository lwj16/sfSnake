#include <SFML/Graphics.hpp>
#include <cmath>

#include "SnakeNode.h"

using namespace sfSnake;

const float SnakeNode::Width = 10.f;
const float SnakeNode::Height = 10.f;

sf::Texture SnakeNode::headTexture_;

void SnakeNode::loadHeadTexture(const std::string& path) {
    headTexture_.loadFromFile(path);
}

SnakeNode::SnakeNode(sf::Vector2f position, NodeType type): position_(position), type_(type), direction_(0.f, -1.f)
{
    if (type_ == NodeType::Head) {
		loadHeadTexture("Images/head.png");
        headSprite_.setTexture(headTexture_);
        headSprite_.setPosition(position_);
        headSprite_.setScale(
            2.f * SnakeNode::Width / headTexture_.getSize().x,
            2.f * SnakeNode::Height / headTexture_.getSize().y
        );
		headSprite_.setOrigin(headTexture_.getSize().x / 2.f, headTexture_.getSize().y / 2.f);
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
    if (type_ == NodeType::Head)
        bounds = headSprite_.getGlobalBounds();
    else
        bounds = shape_.getGlobalBounds();

    // 缩小碰撞体积，缩小到30%
    float scale = 0.3f;
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
}