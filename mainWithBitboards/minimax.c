int minimax(int depth,int alpha,int beta,int maximizing){
    if(checkWin(MACHINE)) return 100000;
    if(checkWin(HUMAN)) return -100000;
    if(depth==0) return evaluate();

    int order[COLS]={3,2,4,1,5,0,6};

    if(maximizing){
        int maxEval = INT_MIN;
        for(int i=0;i<COLS;i++){
            int col = order[i];
            if(isValidMove(0, col)){
                makeMove(col,MACHINE);
                int eval = minimax(depth-1,alpha,beta,0);
                undoMove(col);
                if(eval>maxEval) maxEval=eval;
                if(eval>alpha) alpha=eval;
                if(beta<=alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for(int i=0;i<COLS;i++){
            int col = order[i];
            if(isValidMove(0, col)){
                makeMove(col,HUMAN);
                int eval = minimax(depth-1,alpha,beta,1);
                undoMove(col);
                if(eval<minEval) minEval=eval;
                if(eval<beta) beta=eval;
                if(beta<=alpha) break;
            }
        }
        return minEval;
    }
}

void* spinnerThreadFunc(void* arg){
    const char spinnerChars[]="|/-\\";
    int index=0, barWidth=30;
    while(spinnerRunning){
        pthread_mutex_lock(&mutex);
        int filled = (currentDepth*barWidth)/MAX_DEPTH;
        pthread_mutex_unlock(&mutex);

        printf("\r[");
        printf("\x1b[32m"); for(int i=0;i<filled;i++) printf("#");
        printf("\x1b[33m"); for(int i=filled;i<barWidth;i++) printf("-");
        printf("\x1b[0m");

        printf("] Depth %d %c", currentDepth, spinnerChars[index]);
        fflush(stdout);

        index = (index+1)%4;
        usleep(100000);
    }
    return NULL;
}

int bestMove(){
    int bestCol=3, bestScore=INT_MIN;
    spinnerRunning=1; currentDepth=0;
    pthread_t spinner;
    pthread_create(&spinner,NULL,spinnerThreadFunc,NULL);

    clock_t start=clock();
    for(int depth=1;depth<=MAX_DEPTH;depth++){
        pthread_mutex_lock(&mutex); currentDepth=depth; pthread_mutex_unlock(&mutex);

        for(int col=0;col<COLS;col++){
            if(isValidMove(0, col)){
                makeMove(col,MACHINE);
                int score=minimax(depth-1,INT_MIN,INT_MAX,0);
                undoMove(col);
                if(score>bestScore){ bestScore=score; bestCol=col; }
            }
        }
    }
    clock_t end=clock();
    spinnerRunning=0; pthread_join(spinner,NULL);

    printf("\r["); for(int i=0;i<30;i++) printf("\x1b[32m#\x1b[0m");
    printf("] Depth %d done! (%.3f sec)\n",MAX_DEPTH,(double)(end-start)/CLOCKS_PER_SEC);
    return bestCol;
}