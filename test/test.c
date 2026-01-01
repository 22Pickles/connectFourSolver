#include <stdint.h>
#include <stdio.h>

#define ROWS 6
#define COLS 7
#define MAX_DEPTH 9

#define MACHINE 1
#define HUMAN -1
#define EMPTY 0



typedef uint64_t bitBoard;

typedef struct {
    bitBoard player1;
    bitBoard player2;
} board;

void printBoard(board b) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int bit = col + row * COLS;
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
    *bb |= (1ULL << (col *COLS + row));
}

int makeMove(int gameBoard[ROWS][COLS], int col, int player){
    for(int r = ROWS-1; r >= 0; r--){ 
        if(gameBoard[r][col] == EMPTY){
            gameBoard[r][col] = player; 
            return r; 
        } 
    }
    return -1;
}

board makeBitBoard(int gameBoard[ROWS][COLS],
                             uint64_t *human_bb,
                             uint64_t *machine_bb)
{
    *human_bb = 0;
    *machine_bb = 0;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int bitIndex = row * COLS + col;
            bitBoard bit = 1ULL << bitIndex;

            if (gameBoard[row][col] == HUMAN) {
                *human_bb |= bit;
            } else if (gameBoard[row][col] == MACHINE) {
                *machine_bb |= bit;
            }
        }
    }
    board a = {
        *human_bb,
        *machine_bb
    };
    return a;
}

void printGameBoard(int gameBoard[ROWS][COLS]){
    printf(" ┌───────────────┐\n │ 0 1 2 3 4 5 6 │\n");
    for(int r=0;r<ROWS;r++){
        printf(" │ ");
        for(int c=0;c<COLS;c++){
            if(gameBoard[r][c]==MACHINE) printf("\x1b[31mX\x1b[0m ");
            else if(gameBoard[r][c]==HUMAN) printf("\x1b[34mO\x1b[0m ");
            else printf(". ");
        }
        printf("│\n");
    }
    printf(" └───────────────┘\n\n");
}

int main(void) {
    int gameBoard[ROWS][COLS];
    makeMove(gameBoard, 3, 1);
    printGameBoard(gameBoard);
    bitBoard human_bb, machine_bb;


    board b;
    b = makeBitBoard(gameBoard, &human_bb, &machine_bb);
    printBoard(b);

    return 0;
}