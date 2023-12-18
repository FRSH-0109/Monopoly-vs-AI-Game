#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Field.hpp"

class Board {
    unsigned int file_number_;
    std::vector<Field> board_;

    public:
    Board(std::string file_path);
};

#endif