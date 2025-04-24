#include "grid.hpp"
#include "pieces.hpp"
#include <iostream>

grid::grid(int w, int h) : width(w), height(h), current_state(PAUSE) {
    clear();
}

void grid::clear() {
    cells = std::vector<std::vector<std::string>>(height, std::vector<std::string>(width, "|__"));
}

void grid::draw(const piece& current_piece) const {
    system("clear");
    
    auto temp_grid = cells;
    
    for (const auto& cell : current_piece.get_occupied_cells()) {
        if (cell.second >= 0 && cell.second < height && cell.first >= 0 && cell.first < width) {
            temp_grid[cell.second][cell.first] = "\033[" + std::to_string(current_piece.color_code) + "m[*]\033[0m";
        }
    }
    
    for (const auto& row : temp_grid) {
        for (const auto& cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

bool grid::is_cell_empty(int x, int y) const {
    if (y < 0) return true;
    if (y >= height || x < 0 || x >= width) return false;
    return cells[y][x] == "|__";
}
