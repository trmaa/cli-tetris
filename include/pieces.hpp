#pragma once
#include <vector>
#include <utility>

enum piece_type {
    I, J, L, O, S, T, Z
};

struct piece {
    piece_type type;
    std::vector<std::vector<int>> shape;
    int x;
    int y;
    int color_code;

    piece(piece_type t, int start_x, int start_y);
    void rotate();
    std::vector<std::pair<int, int>> get_occupied_cells() const;
};
