#include <stdint.h>

void makeBitBoard(board) {
    *human_bb = 0;
    *machine_bb = 0;

    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            int bitIndex = row * 7 + col;
            uint64_t bit = 1ULL << bitIndex;

            if (board[row][col] == HUMAN) {
                *human_bb |= bit;
            } else if (board[row][col] == MACHINE) {
                *machine_bb |= bit;
            }
        }
    }
}