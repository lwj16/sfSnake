#ifndef SNAKE_NODE_H
#define SNAKE_NODE_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
enum class NodeType { Head, Body };

class SnakeNode
{
public:
    enum class NodeType { Head, Body };
    enum class SnakeType { Mankind, Robot };

    SnakeNode(sf::Vector2f position = sf::Vector2f(0, 0), NodeType type = NodeType::Body, SnakeType snake = SnakeType::Mankind);

    void setPosition(sf::Vector2f position);
    void setPosition(float x, float y);
    void move(float xOffset, float yOffset);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void render(sf::RenderWindow& window);

	static const float Width;
	static const float Height;

	void setDirection(const sf::Vector2f& dir);

private:
    sf::Vector2f position_;
    NodeType type_;
    SnakeType snake_;
    sf::RectangleShape shape_;
    sf::CircleShape circle_;
    bool textureLoaded_;
    sf::Sprite headSprite_;
	sf::Vector2f direction_;
};
}

#endif