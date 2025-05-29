#include <SFML/Graphics.hpp>
#include "Fruit.h"

using namespace sfSnake;

const float Fruit::Radius = 5.f;

sf::Color Fruit::getColorByType(FruitType type) {
    switch (type) {
        case FruitType::Black: return sf::Color(30, 30, 30);
        case FruitType::Brown: return sf::Color(139, 69, 19);
        case FruitType::Red:   return sf::Color::Red;
        case FruitType::Blue:  return sf::Color::Blue;
        case FruitType::Green: return sf::Color::Green;
    }
    return sf::Color::White;
}

Fruit::Fruit(sf::Vector2f position, FruitType type) : type_(type) {
    shape_.setPosition(position);
    shape_.setRadius(Fruit::Radius);
    shape_.setFillColor(getColorByType(type));
    switch (type) {
        case FruitType::Black:
        case FruitType::Brown:
            growLength_ = 0;
            break;
        case FruitType::Red:
            growLength_ = 3;
            break;
        case FruitType::Blue:
            growLength_ = 2;
            break;
        case FruitType::Green:
            growLength_ = 1;
            break;
    }
}

void Fruit::render(sf::RenderWindow& window) {
    window.draw(shape_);
}

sf::FloatRect Fruit::getBounds() const {
    return shape_.getGlobalBounds();
}

FruitType Fruit::getType() const {
    return type_;
}

int Fruit::getGrowLength() const {
    return growLength_;
}