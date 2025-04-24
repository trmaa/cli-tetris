#include "grid.hpp"
#include "pieces.hpp"
#include "input.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;

piece create_random_piece() {
    static std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> dist(0, 6);
    piece_type random_type = static_cast<piece_type>(dist(rng));
    return piece(random_type, GRID_WIDTH / 2 - 1, 0);
}

bool can_move(const piece& p, const grid& g, int dx, int dy) {
    for (const auto& cell : p.get_occupied_cells()) {
        int new_x = cell.first + dx;
        int new_y = cell.second + dy;
        if (new_x < 0 || new_x >= g.width || new_y >= g.height || 
            (!g.is_cell_empty(new_x, new_y) && new_y >= 0)) {
            return false;
        }
    }
    return true;
}

void merge_piece_to_grid(piece& p, grid& g) {
    for (const auto& cell : p.get_occupied_cells()) {
        if (cell.second >= 0) {
            g.cells[cell.second][cell.first] = "\033[" + std::to_string(p.color_code) + "m[*]\033[0m";
        }
    }
}

int check_completed_lines(grid& g) {
    int completed_lines = 0;
    for (int y = g.height - 1; y >= 0; --y) {
        bool line_complete = true;
        for (int x = 0; x < g.width; ++x) {
            if (g.is_cell_empty(x, y)) {
                line_complete = false;
                break;
            }
        }
        if (line_complete) {
            completed_lines++;
            for (int ny = y; ny > 0; --ny) {
                g.cells[ny] = g.cells[ny - 1];
            }
            g.cells[0] = std::vector<std::string>(g.width, "|__");
            y++;
        }
    }
    return completed_lines;
}

void handle_game_logic(piece& current_piece, piece& next_piece, grid& game_grid, int& score) {
    if (can_move(current_piece, game_grid, 0, 1)) {
        current_piece.y++;
    } else {
        merge_piece_to_grid(current_piece, game_grid);
        score += check_completed_lines(game_grid) * 100;
        current_piece = next_piece;
        next_piece = create_random_piece();
        if (!can_move(current_piece, game_grid, 0, 0)) {
            game_grid.current_state = PAUSE;
            game_grid.clear();
        }
    }
}

void handle_input(char input, piece& current_piece, grid& game_grid) {
    switch (input) {
        case 'a': if (can_move(current_piece, game_grid, -1, 0)) current_piece.x--; break;
        case 'd': if (can_move(current_piece, game_grid, 1, 0)) current_piece.x++; break;
        case 's': break; // Speed handled in game loop
        case 'w': current_piece.rotate(); 
                  if (!can_move(current_piece, game_grid, 0, 0)) {
                      current_piece.rotate(); // Rotate back if invalid
                  }
                  break;
        case 27: game_grid.current_state = PAUSE; break;
    }
}

void game_loop() {
    grid game_grid(GRID_WIDTH, GRID_HEIGHT);
    input_handler input;
    int score = 0;
    piece current_piece = create_random_piece();
    piece next_piece = create_random_piece();
    bool speed_boost = false;

    while (true) {
        game_grid.draw(current_piece);
        std::cout << "Score: " << score << std::endl;
        std::cout << "Controls: WASD to move (hold S for speed), ESC to pause" << std::endl;

        if (game_grid.current_state == PLAY) {
            char user_input = input.get_input();
            if (user_input != 0) {
                speed_boost = (user_input == 's');
                handle_input(user_input, current_piece, game_grid);
                game_grid.draw(current_piece);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            int current_speed = speed_boost ? 200 : 500;
            handle_game_logic(current_piece, next_piece, game_grid, score);
            std::this_thread::sleep_for(std::chrono::milliseconds(current_speed));
        } else {
            std::cout << "PAUSED - Press ENTER to start, ESC to exit" << std::endl;
            char user_input = input.get_input();
            if (user_input == '\n') {
                game_grid.current_state = PLAY;
                game_grid.clear();
                score = 0;
                current_piece = create_random_piece();
                next_piece = create_random_piece();
            } else if (user_input == 27) {
                break;
            }
        }
    }
}

int main() {
    game_loop();
}
