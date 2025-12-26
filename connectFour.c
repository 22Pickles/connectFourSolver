/******************************
        Connect Four Game
        by 22Pickles
******************************/

/****** Imports ******/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

/****** Constants ******/
#define ROWS 6
#define COLS 7
#define MAX_DEPTH 9

#define MACHINE 1
#define HUMAN -1
#define EMPTY 0

/****** Global Variables ******/
int board[ROWS][COLS];
volatile int spinner_running = 0;
volatile int current_depth = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/******************************
        BOARD FUNCTIONS
******************************/
void init_board() {
    for(int r=0; r<ROWS; r++)
        for(int c=0; c<COLS; c++)
            board[r][c] = EMPTY;
}

int is_valid_move(int col){ 
    return col >= 0 && col < COLS && board[0][col] == EMPTY; 
}

int make_move(int col, int player){
    for(int r = ROWS-1; r >= 0; r--){ 
        if(board[r][col] == EMPTY){
            board[r][col] = player; 
            return r; 
        } 
    }
    return -1;
}

void undo_move(int col){
    for(int r = 0; r < ROWS; r++){
        if(board[r][col] != EMPTY){
            board[r][col] = EMPTY;
            return;
        }
    }
}

int is_full() {
    for(int c=0;c<COLS;c++)
        if(is_valid_move(c)) return 0;
    return 1;
}

int check_win(int player){
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

/******************************
        EVALUATION FUNCTIONS
******************************/
int score_window(int a,int b,int c,int d){
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

int evaluate_center() {
    int score=0;
    for(int r=0;r<ROWS;r++)
        for(int c=0;c<COLS;c++){
            int weight = 3 - abs(3-c);
            if(board[r][c]==MACHINE) score += weight;
            else if(board[r][c]==HUMAN) score -= weight;
        }
    return score;
}

int evaluate_windows() {
    int score=0;
    for(int r=0;r<ROWS;r++)
        for(int c=0;c<COLS;c++){
            if(c+3<COLS) score += score_window(board[r][c],board[r][c+1],board[r][c+2],board[r][c+3]);
            if(r+3<ROWS) score += score_window(board[r][c],board[r+1][c],board[r+2][c],board[r+3][c]);
            if(r+3<ROWS && c+3<COLS) score += score_window(board[r][c],board[r+1][c+1],board[r+2][c+2],board[r+3][c+3]);
            if(r+3<ROWS && c-3>=0) score += score_window(board[r][c],board[r+1][c-1],board[r+2][c-2],board[r+3][c-3]);
        }
    return score;
}

int evaluate() {
    return evaluate_center() + evaluate_windows();
}

/******************************
        MINIMAX
******************************/
int minimax(int depth,int alpha,int beta,int maximizing){
    if(check_win(MACHINE)) return 100000;
    if(check_win(HUMAN)) return -100000;
    if(depth==0) return evaluate();

    int order[COLS]={3,2,4,1,5,0,6};

    if(maximizing){
        int maxEval = INT_MIN;
        for(int i=0;i<COLS;i++){
            int c = order[i];
            if(is_valid_move(c)){
                make_move(c,MACHINE);
                int eval = minimax(depth-1,alpha,beta,0);
                undo_move(c);
                if(eval>maxEval) maxEval=eval;
                if(eval>alpha) alpha=eval;
                if(beta<=alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for(int i=0;i<COLS;i++){
            int c = order[i];
            if(is_valid_move(c)){
                make_move(c,HUMAN);
                int eval = minimax(depth-1,alpha,beta,1);
                undo_move(c);
                if(eval<minEval) minEval=eval;
                if(eval<beta) beta=eval;
                if(beta<=alpha) break;
            }
        }
        return minEval;
    }
}

void* spinner_thread_func(void* arg){
    const char spinner_chars[]="|/-\\";
    int index=0, bar_width=30;
    while(spinner_running){
        pthread_mutex_lock(&mutex);
        int filled = (current_depth*bar_width)/MAX_DEPTH;
        pthread_mutex_unlock(&mutex);

        printf("\r[");
        printf("\x1b[32m"); for(int i=0;i<filled;i++) printf("#");
        printf("\x1b[33m"); for(int i=filled;i<bar_width;i++) printf("-");
        printf("\x1b[0m");

        printf("] Depth %d %c", current_depth, spinner_chars[index]);
        fflush(stdout);

        index = (index+1)%4;
        usleep(100000);
    }
    return NULL;
}

int best_move(){
    int bestCol=3, bestScore=INT_MIN;
    spinner_running=1; current_depth=0;
    pthread_t spinner;
    pthread_create(&spinner,NULL,spinner_thread_func,NULL);

    clock_t start=clock();
    for(int depth=1;depth<=MAX_DEPTH;depth++){
        pthread_mutex_lock(&mutex); current_depth=depth; pthread_mutex_unlock(&mutex);

        for(int c=0;c<COLS;c++){
            if(is_valid_move(c)){
                make_move(c,MACHINE);
                int score=minimax(depth-1,INT_MIN,INT_MAX,0);
                undo_move(c);
                if(score>bestScore){ bestScore=score; bestCol=c; }
            }
        }
    }
    clock_t end=clock();
    spinner_running=0; pthread_join(spinner,NULL);

    printf("\r["); for(int i=0;i<30;i++) printf("\x1b[32m#\x1b[0m");
    printf("] Depth %d done! (%.3f sec)\n",MAX_DEPTH,(double)(end-start)/CLOCKS_PER_SEC);
    return bestCol;
}

/******************************
        UI FUNCTIONS
******************************/
void print_board(){
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

int choose_mode(){
    int choice;
    printf("\x1b[35m===== CONNECT 4 =====\x1b[0m\n\x1b[36m1. Human vs MACHINE\n2. Human vs Human\x1b[0m\n");
    while(1){ printf("Enter choice: "); fflush(stdout);
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }
        if(choice==1) return 1;
        if(choice==2) return 0;
    }
}

int choose_first(){
    int choice;
    printf("\x1b[36mWho goes first?\x1b[0m\n1. Human\n2. MACHINE\n");
    while(1){ printf("Enter 1 or 2: "); fflush(stdout);
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }
        if(choice==1) return HUMAN;
        if(choice==2) return MACHINE;
    }
}

/******************************
        GAME LOOP
******************************/
void play_game(){
    int mode = choose_mode();
    int turn = (mode==1)? choose_first() : HUMAN;
    int human_wins=0,machine_wins=0,ties=0;
    int play_again=1;

    while(play_again){
        init_board();
        while(1){
            print_board();
            if((mode==1 && turn==HUMAN) || mode==0){
                char line[32]; int col;
                printf("Your move (0-6): "); fflush(stdout);
                if(!fgets(line,sizeof(line),stdin)) continue;
                char *end; col=strtol(line,&end,10);
                if(end==line || !is_valid_move(col)){ printf("Invalid move.\n"); continue; }
                make_move(col,(turn==HUMAN)?HUMAN:MACHINE);
                if(check_win((turn==HUMAN)?HUMAN:MACHINE)){
                    print_board(); 
                    printf("\x1b[34mPlayer 1 wins!\x1b[0m\n"); human_wins++;
                    break;
                }
            } else {
                int col = best_move();
                make_move(col,MACHINE);
                if(check_win(MACHINE)){ print_board(); printf("\x1b[31mMACHINE wins!\x1b[0m\n"); machine_wins++; break; }
            }

            if(is_full()){ print_board(); printf("\x1b[33mIt's a tie!\x1b[0m\n"); ties++; break; }
            turn = (mode==0)? ((turn==HUMAN)?MACHINE:HUMAN) : -turn;
        }

        printf("\nScoreboard: \x1b[34mHuman %d\x1b[0m | \x1b[31mMACHINE %d\x1b[0m | \x1b[33mTies %d\x1b[0m\n",human_wins,machine_wins,ties);
        int choice; printf("Play again? (1=Yes,0=No): "); fflush(stdout);
        if(scanf("%d",&choice)!=1) break;
        play_again=choice; while(getchar()!='\n');
    }
    printf("\x1b[36mThanks for playing!\x1b[0m\n");
}

int main(){
    play_game();
    return 0;
}
