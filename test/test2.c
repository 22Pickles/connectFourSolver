#include <stdint.h>
#include <stdio.h>

typedef uint64_t bitboard;

typedef struct {
    bitboard p1;
    bitboard p2;
} board;

int main(void) {
    bitboard human_bb, machine_bb;
        human_bb = 0;
        machine_bb = 0;

    int col = 2;
    int row = 5;
    int bitIndex = col + row * 7;
    int var = 9; //human_bb >> bitIndex;

    printf("%i\n", var);
}