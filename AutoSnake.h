#ifndef AUTOSNAKE_H
#define AUTOSNAKE_H

#include <vector>
#include <SFML/Graphics.hpp>

#include "SnakeNode.h"
#include "Fruit.h"
#include "Map.h"

namespace sfSnake {
class AutoSnake {
public:
	AutoSnake();

	void update(sf::Time delta, const Map& map, const std::vector<Fruit>& fruits);
	void render(sf::RenderWindow& window);

	void checkFruitCollisions(std::vector<Fruit>& fruits);

	bool hitSelf() const;

	unsigned getSize() const;

	std::vector<SnakeNode> nodes;

	std::vector<sf::Vector2i> findPathAStar(const Map& map, sf::Vector2i start, sf::Vector2i goal);

private:
	void move();
	void grow();
	void checkEdgeCollisions();
	void checkSelfCollisions();
	void initNodes();

	bool hitSelf_;

    enum class Direction { Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight };

	sf::Vector2f position_;
	Direction direction_;

	static const int InitialSize;
};
}

#endif