#include <stdint.h>
#include <stdio.h>

typedef uint64_t bitboard;

typedef struct {
    bitboard p1;
    bitboard p2;
} board;

void printBoard(board b) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            int bit = col + row * 7;
            if (b.p1 & (1ULL << bit))
                printf(" X ");
            else if (b.p2 & (1ULL << bit))
                printf(" O ");
            else
                printf(" . ");
        }
        printf("\n");
    }
    printf("-----------------------------\n");
    printf(" 0  1  2  3  4  5  6\n");
}

int isValidMove(board b, int row, int col) {
    int bitIndex = col + row * 7;
    bitboard mask = (bitboard)1 << bitIndex;
    return col < 7 && col >= 0 && row < 6 && row >= 0 && !((b.p1 & mask) || (b.p2 & mask));
}

int main(void) {
    bitboard human_bb, machine_bb;
        human_bb = 0;
        machine_bb = 0;

    int col = 2;
    int row = 5;
    int bitIndex = col + row * 7;
    human_bb |= ((bitboard)1 << bitIndex);

    col = 4;
    row = 5;
    bitIndex = col + row * 7;void printBoard(){
    system("clear");
    printf(" ┌───────────────┐\n │ 0 1 2 3 4 5 6 │\n");
    for(int r=0;r<ROWS;r++){
        printf(" │ ");
        for(int c=0;c<COLS;c++){
            if(board[r][c]==MACHINE) printf("\x1b[31mX\x1b[0m ");
            else if(board[r][c]==HUMAN) printf("\x1b[34mO\x1b[0m ");
            else printf(". ");
        }
        printf("│\n");
    }
    printf(" └───────────────┘\n\n");
}

int chooseMode(){
    int choice;
    printf("\x1b[35m===== CONNECT 4 =====\x1b[0m\n\x1b[36m1. Human vs MACHINE\n2. Human vs Human\x1b[0m\n");
    while(1){ printf("Enter choice: "); fflush(stdout);
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }
        if(choice==1) return 1;
        if(choice==2) return 0;
    }
}

int chooseFirst(){
    int choice;
    printf("\x1b[36mWho goes first?\x1b[0m\n1. Human\n2. MACHINE\n");
    while(1){ printf("Enter 1 or 2: "); fflush(stdout);
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }
        if(choice==1) return HUMAN;
        if(choice==2) return MACHINE;
    }
}

    machine_bb |= ((bitboard)1 << bitIndex);

    machine_bb &= ~((bitboard)1 << bitIndex);

    board a = {human_bb, machine_bb};
    printBoard(a);
    
    printf("%i\n", isValidMove(a, 5, 4));
}