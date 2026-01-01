void initBoard() {
    bitBoard human_bb, machine_bb;
    *human_bb = 0;
    *machine_bb = 0;
}

int isValidMove(int col, row) {
    return col >= 0 && col < COLS && board[row][col] == EMPTY;
}