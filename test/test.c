#include <stdint.h>
#include <stdio.h>


typedef uint64_t bitBoard;

typedef struct {
    bitBoard player1;
    bitBoard player2;
} board;

void printBoard(board b) {
    for (int row = 5; row >= 0; row--) {
        for (int col = 0; col < 7; col++) {
            int bit = col * 7 + row;
            if (b.player1 & (1ULL << bit))
                printf(" X ");
            else if (b.player2 & (1ULL << bit))
                printf(" O ");
            else
                printf(" . ");
        }
        printf("\n");
    }
    printf("-----------------------------\n");
    printf(" 0  1  2  3  4  5  6\n");
}

void setPiece(bitBoard *bb, int col, int row) {
    *bb |= (1ULL << (col *7 + row));
}

int main(void) {
    board b = {0, 0};

    /* Example position */
    setPiece(&b.player1, 3, 0); // Player 1 in column 3
    setPiece(&b.player2, 3, 1); // Player 2 above
    setPiece(&b.player1, 2, 0);
    setPiece(&b.player1, 4, 0);

    printBoard(b);

    return 0;
}