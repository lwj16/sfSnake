#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>
#include <cmath>
#include <queue>
#include <map>
#include <set>
#include <cmath>

#include "AutoSnake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"
#include "Map.h"

using namespace sfSnake;

const int AutoSnake::InitialSize = 5;

AutoSnake::AutoSnake() : direction_(Direction::Up), hitSelf_(false)
{
	initNodes();
}

void AutoSnake::initNodes()
{
    nodes.clear();
    for (int i = 0; i < AutoSnake::InitialSize; ++i)
    {
        SnakeNode::NodeType type = (i == 0) ? SnakeNode::NodeType::Head : SnakeNode::NodeType::Body;
        SnakeNode::SnakeType snake = SnakeNode::SnakeType::Robot;
        nodes.push_back(SnakeNode(
            sf::Vector2f(
                Game::Width / 3 * 2 - SnakeNode::Width / 2,
                Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i)
            ),
            type, snake
        ));
    }
}

struct Node {
    int x, y;
    float g, h;
    Node* parent;
    Node(int x, int y, float g, float h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}
    float f() const { return g + h; }
    bool operator<(const Node& other) const { return f() > other.f(); } // 小顶堆
};

std::vector<sf::Vector2i> AutoSnake::findPathAStar(
    const Map& map, sf::Vector2i start, sf::Vector2i goal)
{
    int w = map.getWidth(), h = map.getHeight();
    std::priority_queue<Node> open;
    std::set<std::pair<int, int>> closed;
    open.emplace(start.x, start.y, 0, std::hypot(goal.x - start.x, goal.y - start.y));

    std::map<std::pair<int, int>, Node*> allNodes;

    while (!open.empty()) {
        Node curr = open.top(); open.pop();
        if ((curr.x == goal.x && curr.y == goal.y) || curr.g >= 5) {
            // 回溯路径
            std::vector<sf::Vector2i> path;
            Node* p = &curr;
            while (p) {
                path.push_back(sf::Vector2i(p->x, p->y));
                p = p->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        closed.insert({curr.x, curr.y});
        static const int dx[8] = {0,0,1,-1,1,1,-1,-1}, dy[8] = {1,-1,0,0,1,-1,1,-1};
        for (int d = 0; d < 8; ++d) {
            int nx = curr.x + dx[d], ny = curr.y + dy[d];
            if (nx < 0 || ny < 0 || nx >= w || ny >= h) continue;
            if (map.isObstacle(nx, ny)) continue;
            if (closed.count({nx, ny})) continue;
            float ng = curr.g + ((d < 4) ? 1.f : 1.414f);
            float nh = std::hypot(goal.x - nx, goal.y - ny);
            Node* next = new Node(nx, ny, ng, nh, new Node(curr.x, curr.y, curr.g, curr.h, curr.parent));
            open.push(*next);
            allNodes[{nx, ny}] = next;
        }
    }
    return {}; // 无路可走
}

void AutoSnake::update(sf::Time delta, const Map& map, const std::vector<Fruit>& fruits)
{
    // 获取蛇头格点
    sf::Vector2f headPos = nodes[0].getPosition();
    sf::Vector2i start(
        static_cast<int>(headPos.x / SnakeNode::Width),
        static_cast<int>(headPos.y / SnakeNode::Height)
    );
    // 找到最近的水果
    sf::Vector2i goal;
    float minDist = 1e9;
    for (const auto& fruit : fruits) {
        sf::Vector2f fpos = fruit.getBounds().getPosition();
        sf::Vector2i fgrid(
            static_cast<int>(fpos.x / SnakeNode::Width),
            static_cast<int>(fpos.y / SnakeNode::Height)
        );
        float dist = std::hypot(fgrid.x - start.x, fgrid.y - start.y);
        if (dist < minDist) {
            minDist = dist;
            goal = fgrid;
        }
    }
    // A*
    auto path = findPathAStar(map, start, goal);
    if (path.size() > 1) {
        sf::Vector2i next = path[1];
        sf::Vector2i delta = next - start;
        if (delta.x == 0 && delta.y == -1)
            direction_ = Direction::Up;
        else if (delta.x == 0 && delta.y == 1)
            direction_ = Direction::Down;
        else if (delta.x == -1 && delta.y == 0)
            direction_ = Direction::Left;
        else if (delta.x == 1 && delta.y == 0)
            direction_ = Direction::Right;
        else if (delta.x == -1 && delta.y == -1)
            direction_ = Direction::UpLeft;
        else if (delta.x == 1 && delta.y == -1)
            direction_ = Direction::UpRight;
        else if (delta.x == -1 && delta.y == 1)
            direction_ = Direction::DownLeft;
        else if (delta.x == 1 && delta.y == 1)
            direction_ = Direction::DownRight;
    }
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

void AutoSnake::checkFruitCollisions(std::vector<Fruit>& fruits)
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
        for (int i = 0; i < growCount; ++i)
            grow();
        fruits.erase(toRemove);
    }
}

void AutoSnake::grow()
{
    sf::Vector2f tailPos = nodes.back().getPosition();
    sf::Vector2f newNodePos;

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
    case Direction::UpLeft:
        newNodePos = tailPos + sf::Vector2f(SnakeNode::Width, SnakeNode::Height);
        break;
    case Direction::UpRight:
        newNodePos = tailPos + sf::Vector2f(-SnakeNode::Width, SnakeNode::Height);
        break;
    case Direction::DownLeft:
        newNodePos = tailPos + sf::Vector2f(SnakeNode::Width, -SnakeNode::Height);
        break;
    case Direction::DownRight:
        newNodePos = tailPos + sf::Vector2f(-SnakeNode::Width, -SnakeNode::Height);
        break;
    }
    nodes.push_back(SnakeNode(newNodePos, SnakeNode::NodeType::Body, SnakeNode::SnakeType::Robot));
}

unsigned AutoSnake::getSize() const
{
	return nodes.size();
}

bool AutoSnake::hitSelf() const
{
	return hitSelf_;
}

void AutoSnake::checkSelfCollisions()
{
	SnakeNode& headNode = nodes[0];

	for (decltype(nodes.size()) i = 1; i < nodes.size(); ++i)
	{
		if (headNode.getBounds().intersects(nodes[i].getBounds()))
		{
			hitSelf_ = true;
		}
	}
}

void AutoSnake::checkEdgeCollisions()
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

void AutoSnake::move()
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
    switch (direction_)
    {
    case Direction::Up:        headDir = {0, -1}; break;
    case Direction::Down:      headDir = {0, 1};  break;
    case Direction::Left:      headDir = {-1, 0}; break;
    case Direction::Right:     headDir = {1, 0};  break;
    case Direction::UpLeft:    headDir = {-1, -1}; break;
    case Direction::UpRight:   headDir = {1, -1}; break;
    case Direction::DownLeft:  headDir = {-1, 1}; break;
    case Direction::DownRight: headDir = {1, 1};  break;
    }
    // 单位化
    float len = std::sqrt(headDir.x * headDir.x + headDir.y * headDir.y);
    if (len > 0.01f)
        headDir = sf::Vector2f(headDir.x / len, headDir.y / len);

    nodes[0].setDirection(headDir);

    nodes[0].move(headDir.x * SnakeNode::Width, headDir.y * SnakeNode::Height);
}

void AutoSnake::render(sf::RenderWindow& window)
{
	for (auto& node : nodes)
		node.render(window);
}
