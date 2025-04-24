#include "pieces.hpp"
#include <vector>

piece::piece(piece_type t, int start_x, int start_y) : type(t), x(start_x), y(start_y) {
    switch (type) {
        case I: shape = {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}; color_code = 36; break;
        case J: shape = {{2,0,0}, {2,2,2}, {0,0,0}}; color_code = 34; break;
        case L: shape = {{0,0,3}, {3,3,3}, {0,0,0}}; color_code = 33; break;
        case O: shape = {{4,4}, {4,4}}; color_code = 32; break;
        case S: shape = {{0,5,5}, {5,5,0}, {0,0,0}}; color_code = 35; break;
        case T: shape = {{0,6,0}, {6,6,6}, {0,0,0}}; color_code = 31; break;
        case Z: shape = {{7,7,0}, {0,7,7}, {0,0,0}}; color_code = 37; break;
    }
}

void piece::rotate() {
    std::vector<std::vector<int>> rotated(shape[0].size(), std::vector<int>(shape.size()));
    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            rotated[j][shape.size() - 1 - i] = shape[i][j];
        }
    }
    shape = rotated;
}

std::vector<std::pair<int, int>> piece::get_occupied_cells() const {
    std::vector<std::pair<int, int>> cells;
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j] != 0) {
                cells.emplace_back(x + j, y + i);
            }
        }
    }
    return cells;
}

