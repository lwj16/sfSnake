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

    SnakeNode(sf::Vector2f position = sf::Vector2f(0, 0), NodeType type = NodeType::Body);

    void setPosition(sf::Vector2f position);
    void setPosition(float x, float y);
    void move(float xOffset, float yOffset);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void render(sf::RenderWindow& window);

	static const float Width;
	static const float Height;

    static void loadHeadTexture(const std::string& path);

	void setDirection(const sf::Vector2f& dir);

private:
    sf::Vector2f position_;
    NodeType type_;
    sf::RectangleShape shape_;
    sf::CircleShape circle_;
    static sf::Texture headTexture_;
    sf::Sprite headSprite_;
	sf::Vector2f direction_;
};
}

#endif