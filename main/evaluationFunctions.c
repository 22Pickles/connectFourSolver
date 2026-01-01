int scoreWindow(int a,int b,int c,int d){
    int machine=0, opp=0, empty=0;
    int vals[4]={a,b,c,d};
    for(int i=0;i<4;i++){
        if(vals[i]==MACHINE) machine++;
        else if(vals[i]==HUMAN) opp++;
        else empty++;
    }
    if(machine==4) return 10000;
    if(machine==3 && empty==1) return 100;
    if(machine==2 && empty==2) return 10;
    if(opp==4) return -10000;
    if(opp==3 && empty==1) return -80;
    if(opp==2 && empty==2) return -5;
    return 0;
}

int evaluateCenter() {
    int score=0;
    for(int r=0;r<ROWS;r++)
        for(int c=0;c<COLS;c++){
            int weight = 3 - abs(3-c);
            if(board[r][c]==MACHINE) score += weight;
            else if(board[r][c]==HUMAN) score -= weight;
        }
    return score;
}

int evaluateWindows() {
    int score=0;
    for(int r=0;r<ROWS;r++)
        for(int c=0;c<COLS;c++){
            if(c+3<COLS) score += scoreWindow(board[r][c],board[r][c+1],board[r][c+2],board[r][c+3]);
            if(r+3<ROWS) score += scoreWindow(board[r][c],board[r+1][c],board[r+2][c],board[r+3][c]);
            if(r+3<ROWS && c+3<COLS) score += scoreWindow(board[r][c],board[r+1][c+1],board[r+2][c+2],board[r+3][c+3]);
            if(r+3<ROWS && c-3>=0) score += scoreWindow(board[r][c],board[r+1][c-1],board[r+2][c-2],board[r+3][c-3]);
        }
    return score;
}

int evaluate() {
    return evaluateCenter() + evaluateWindows();
}
