#include "Map.h"

using namespace sfSnake;

Map::Map(int width, int height) : width_(width), height_(height), grid_(width, std::vector<bool>(height, false)) {}

void Map::setObstacle(int x, int y, bool isObstacle) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
        grid_[x][y] = isObstacle;
}

bool Map::isObstacle(int x, int y) const {
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
        return grid_[x][y];
    return true; // 越界视为障碍
}

void Map::clear() {
    for (auto& col : grid_)
        std::fill(col.begin(), col.end(), false);
}