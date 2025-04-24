#pragma once
#include <vector>
#include <string>
#include "pieces.hpp"

enum game_state {
    PAUSE,
    PLAY
};

struct grid {
    int width;
    int height;
    std::vector<std::vector<std::string>> cells;
    game_state current_state;

    grid(int w, int h);
    void clear();
    void draw(const piece& current_piece) const;
    bool is_cell_empty(int x, int y) const;
};
