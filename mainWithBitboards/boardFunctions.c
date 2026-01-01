void initBoard() {
    human_bb = 0;
    machine_bb = 0;
}

int isValidMove(int row, int col) {
    int bitIndex = col + row * COLS;
    bitboard mask = (bitboard)1 << bitIndex;

    return col < 7 && col >= 0 && row < 6 && row >= 0 && !((human_bb & mask) || (machine_bb & mask));
}

int makeMove(int col, int player) {
    for(int row = ROWS-1; row >= 0; row--){
        int bitIndex = col + row * COLS;
        bitboard mask = (bitboard)1 << bitIndex; 
        if (!((human_bb & mask) || (machine_bb & mask))){ //if empty
            if (player == HUMAN) {
                human_bb |= ((bitboard)1 << bitIndex);
            } else {
                machine_bb |= ((bitboard)1 << bitIndex);
            }
            return row; 
        } 
    }
    return -1;
}

void undoMove(int col){
    for(int row = 0; row < ROWS; row++){
        int bitIndex = col + row * COLS;
        bitboard mask = (bitboard)1 << bitIndex; 
        if(((human_bb & mask) || (machine_bb & mask))){
            human_bb &= ~((bitboard)1 << bitIndex);
            machine_bb &= ~((bitboard)1 << bitIndex);
            return;
        }
    }
}

int isFull() {
    for(int col=0;col<COLS;col++)
        if(isValidMove(0, col)) return 0;
    return 1;
}

int checkWin(int player){
    bitboard player_board;
    if (player == HUMAN) {
        player_board = human_bb;
    } else {
        player_board = machine_bb;
    }

    bitboard m;

     // Vertical
    m = player_board & (player_board >> 1);
    if (m & (m >> 2))
        return 1;

    // Horizontal
    m = player_board & (player_board >> 7);
    if (m & (m >> 14))
        return 1;

    // Diagonal / 
    m = player_board & (player_board >> 6);
    if (m & (m >> 12))
        return 1;

    // Diagonal \ 
    m = player_board & (player_board >> 8);
    if (m & (m >> 16))
        return 1;

    return 0;
}
