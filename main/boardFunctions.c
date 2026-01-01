void initBoard() {
    for(int r=0; r<ROWS; r++)
        for(int c=0; c<COLS; c++)
            board[r][c] = EMPTY;
}

int isValidMove(int col){ 
    return col >= 0 && col < COLS && board[0][col] == EMPTY; 
}

int makeMove(int col, int player){
    for(int r = ROWS-1; r >= 0; r--){ 
        if(board[r][col] == EMPTY){
            board[r][col] = player; 
            return r; 
        } 
    }
    return -1;
}

void undoMove(int col){
    for(int r = 0; r < ROWS; r++){
        if(board[r][col] != EMPTY){
            board[r][col] = EMPTY;
            return;
        }
    }
}

int isFull() {
    for(int c=0;c<COLS;c++)
        if(isValidMove(c)) return 0;
    return 1;
}

int checkWin(int player){
    // Horizontal, vertical, diagonal checks
    for(int r=0;r<ROWS;r++)
        for(int c=0;c<COLS;c++){
            if(c+3<COLS && board[r][c]==player && board[r][c+1]==player && board[r][c+2]==player && board[r][c+3]==player) return 1;
            if(r+3<ROWS){
                if(board[r][c]==player && board[r+1][c]==player && board[r+2][c]==player && board[r+3][c]==player) return 1;
                if(c+3<COLS && board[r][c]==player && board[r+1][c+1]==player && board[r+2][c+2]==player && board[r+3][c+3]==player) return 1;
                if(c-3>=0 && board[r][c]==player && board[r+1][c-1]==player && board[r+2][c-2]==player && board[r+3][c-3]==player) return 1;
            }
        }
    return 0;
}
