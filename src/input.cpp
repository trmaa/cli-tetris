#include <sys/select.h>
#include "input.hpp"

input_handler::input_handler() {
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

input_handler::~input_handler() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

char input_handler::get_input() const {
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0) {
        char c;
        read(STDIN_FILENO, &c, 1);
        return c;
    }
    return 0;
}
