#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SFML/Graphics.hpp>

namespace sfSnake {
class Map {
public:
    Map(int width, int height);

    // 设置障碍
    void setObstacle(int x, int y, bool isObstacle = true);

    // 判断是否为障碍
    bool isObstacle(int x, int y) const;

    // 清空所有障碍
    void clear();

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    int width_, height_;
    std::vector<std::vector<bool>> grid_; // true为障碍，false为可行空间
};
}

#endif