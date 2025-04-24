#pragma once
#include <termios.h>
#include <unistd.h>

struct input_handler {
    termios old_tio;
    termios new_tio;

    input_handler();
    ~input_handler();
    char get_input() const;
};
