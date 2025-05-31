#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
enum class FruitType { Black, Brown, Red, Blue, Green };

class Fruit
{
public:
	Fruit(sf::Vector2f position = sf::Vector2f(0, 0), FruitType type = FruitType::Red);

	void render(sf::RenderWindow& window);

	sf::FloatRect getBounds() const;

	FruitType getType() const;
    int getGrowLength() const;

private:
	sf::CircleShape shape_;
	
	static const float Radius;

	FruitType type_;
    int growLength_;
    static sf::Color getColorByType(FruitType type);
};
}

#endif