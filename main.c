#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define ROWS 6
#define COLS 7
#define MAX_DEPTH 9


#define AI 1
#define HUMAN -1
#define EMPTY 0
#define HVH 0

int board[ROWS][COLS] = {0};
volatile int spinner_running = 0;
volatile int current_depth = 0;

/* ---------- Utility ---------- */

int is_valid_move(int col){ return col>=0 && col<COLS && board[0][col]==EMPTY; }

int make_move(int col,int player){
    for(int r=ROWS-1;r>=0;r--){ if(board[r][col]==EMPTY){ board[r][col]=player; return r; } }
    return -1;
}

void undo_move(int col){
    for(int r=0;r<ROWS;r++){ if(board[r][col]!=EMPTY){ board[r][col]=EMPTY; return; } }
}

void print_board(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf(" ┌───────────────┐\n");
    printf(" │ 0 1 2 3 4 5 6 │\n");
    for (int r = 0; r < ROWS; r++) {
        printf(" │ ");
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == AI) printf("\x1b[31mX\x1b[0m ");
            else if (board[r][c] == HUMAN) printf("\x1b[34mO\x1b[0m ");
            else printf(". ");
        }
        printf("│\n");
    }
    printf(" └───────────────┘\n\n");
}

/* ---------- Win Check ---------- */

int check_win(int player){
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

/* ---------- Evaluation ---------- */

int score_window(int a,int b,int c,int d){
    int ai=0,opp=0,empty=0;
    int vals[4]={a,b,c,d};
    for(int i=0;i<4;i++){
        if(vals[i]==AI) ai++;
        else if(vals[i]==HUMAN) opp++;
        else empty++;
    }
    if(ai==4) return 10000;
    if(ai==3 && empty==1) return 100;
    if(ai==2 && empty==2) return 10;
    if(opp==4) return -10000;
    if(opp==3 && empty==1) return -80;
    if(opp==2 && empty==2) return -5;
    return 0;
}

int evaluate(){
    int score = 0;

    // Center column bonus
    for(int r=0;r<ROWS;r++){
        for(int c=0;c<COLS;c++){
            int weight = 3 - abs(3-c);
            if(board[r][c]==AI) score += weight;
            else if(board[r][c]==HUMAN) score -= weight;
        }
    }

    // Evaluate windows
    for(int r=0;r<ROWS;r++){
        for(int c=0;c<COLS;c++){
            if(c+3<COLS) score += score_window(board[r][c],board[r][c+1],board[r][c+2],board[r][c+3]);
            if(r+3<ROWS) score += score_window(board[r][c],board[r+1][c],board[r+2][c],board[r+3][c]);
            if(r+3<ROWS && c+3<COLS) score += score_window(board[r][c],board[r+1][c+1],board[r+2][c+2],board[r+3][c+3]);
            if(r+3<ROWS && c-3>=0) score += score_window(board[r][c],board[r+1][c-1],board[r+2][c-2],board[r+3][c-3]);
        }
    }

    return score;
}

/* ---------- Minimax ---------- */

int minimax(int depth,int alpha,int beta,int maximizing){
    if(check_win(AI)) return 100000;
    if(check_win(HUMAN)) return -100000;
    if(depth==0) return evaluate();
    int order[COLS]={3,2,4,1,5,0,6};
    if(maximizing){
        int maxEval=INT_MIN;
        for(int i=0;i<COLS;i++){
            int c=order[i];
            if(is_valid_move(c)){
                make_move(c,AI);
                int eval=minimax(depth-1,alpha,beta,0);
                undo_move(c);
                if(eval>maxEval) maxEval=eval;
                if(eval>alpha) alpha=eval;
                if(beta<=alpha) break;
            }
        }
        return maxEval;
    }else{
        int minEval=INT_MAX;
        for(int i=0;i<COLS;i++){
            int c=order[i];
            if(is_valid_move(c)){
                make_move(c,HUMAN);
                int eval=minimax(depth-1,alpha,beta,1);
                undo_move(c);
                if(eval<minEval) minEval=eval;
                if(eval<beta) beta=eval;
                if(beta<=alpha) break;
            }
        }
        return minEval;
    }
}

/* ---------- Spinner and Bar Threads ---------- */

void* spinner_func(void* arg){
    const char spinner_chars[] = "|/-\\";
    int index=0;
    while(spinner_running){
        printf("\r\x1b[36mAI thinking... %c\x1b[0m",spinner_chars[index]);
        fflush(stdout);
        index=(index+1)%4;
        usleep(100000);
    }
    return NULL;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* spinner_bar_thread(void* arg){
    const char spinner_chars[] = "|/-\\";
    int spinner_index = 0;
    int bar_width = 30;

    while(spinner_running){
        pthread_mutex_lock(&mutex);
        int filled = (current_depth * bar_width) / MAX_DEPTH;
        pthread_mutex_unlock(&mutex);

        // Build bar
        printf("\r[");
        printf("\x1b[32m"); // green filled
        for(int i=0;i<filled;i++) printf("#");
        printf("\x1b[33m"); // yellow remaining
        for(int i=filled;i<bar_width;i++) printf("-");
        printf("\x1b[0m"); // reset

        // Spinner
        printf("] Depth %d %c", current_depth, spinner_chars[spinner_index]);
        fflush(stdout);

        spinner_index = (spinner_index + 1) % 4;
        usleep(100000); // 0.1s
    }
    return NULL;
}

/* ---------- Best Move ---------- */


int best_move(){
    int bestCol = 3;
    int bestScore = INT_MIN;

    spinner_running = 1;
    current_depth = 0;

    pthread_t spinner_thread;
    pthread_create(&spinner_thread, NULL, spinner_bar_thread, NULL);

    clock_t start = clock();
    for(int depth=1; depth<=MAX_DEPTH; depth++){
        pthread_mutex_lock(&mutex);
        current_depth = depth;
        pthread_mutex_unlock(&mutex);

        // Simulate AI checking each column (replace with real minimax)
        for(int c=0;c<COLS;c++){
            if(is_valid_move(c)){
                make_move(c, AI);                     // simulate AI move
                int score = minimax(depth-1, INT_MIN, INT_MAX, 0);  // call minimax for opponent
                undo_move(c);                         // undo move
                if(score > bestScore){
                    bestScore = score;
                    bestCol = c;
                }
            }
        }
    }
    clock_t end = clock();
    double elapsed = (double)(end-start)/CLOCKS_PER_SEC;

    spinner_running = 0;
    pthread_join(spinner_thread, NULL);

    // Final filled bar
    printf("\r[");
    for(int i=0;i<30;i++) printf("\x1b[32m#\x1b[0m");
    printf("] Depth %d done! (%.3f sec)\n", MAX_DEPTH, elapsed);

    return bestCol;
}

/* ---------- Menu ---------- */

int choose_mode(){
    int choice;
    printf("\x1b[35m================ CONNECT 4 =================\x1b[0m\n");
    printf("\x1b[36m1. Human vs AI\n2. Human vs Human\x1b[0m\n");
    while(1){
        printf("Enter choice: "); fflush(stdout);
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }
        if(choice==1) return 1;
        if(choice==2) return 0;
    }
}

int choose_first(){
    int choice;
    printf("\x1b[36mWho goes first?\x1b[0m\n1. Human\n2. AI\n");
    while(1){
        printf("Enter 1 or 2: "); fflush(stdout);
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }
        if(choice==1) return HUMAN;
        if(choice==2) return AI;
    }
}

/* ---------- Game Loop ---------- */

int main(){
    int play_again=1;
    int human_wins=0,ai_wins=0,ties=0;

    while(play_again){
        for(int r=0;r<ROWS;r++)
            for(int c=0;c<COLS;c++) board[r][c]=EMPTY;

        int mode = choose_mode();
        int turn = (mode==1)? choose_first() : HUMAN;

        while(1){
            print_board();
            if((mode==1 && turn==HUMAN) || mode==0){ // human turn
                char line[32]; int col;
                printf("Your move (0-6): "); fflush(stdout);
                if(!fgets(line,sizeof(line),stdin)) continue;
                char *end; col=strtol(line,&end,10);
                if(end==line || !is_valid_move(col)){ printf("Invalid move.\n"); continue; }
                make_move(col,(turn==HUMAN)?HUMAN:AI);
                if(check_win((turn==HUMAN)?HUMAN:AI)){
                    print_board();
                    if(turn==HUMAN){ printf("\x1b[34mPlayer 1 wins!\x1b[0m\n"); human_wins++; }
                    else printf("\x1b[31mPlayer 2 wins!\x1b[0m\n");
                    break;
                }
            } else { // AI turn
                int col = best_move();
                make_move(col,AI);
                if(check_win(AI)){ print_board(); printf("\x1b[31mAI wins!\x1b[0m\n"); ai_wins++; break; }
            }

            int full=1;
            for(int c=0;c<COLS;c++) if(is_valid_move(c)) full=0;
            if(full){ print_board(); printf("\x1b[33mIt's a tie!\x1b[0m\n"); ties++; break; }

            turn = (mode==0)? ((turn==HUMAN)?AI:HUMAN) : -turn;
        }

        printf("\nScoreboard: \x1b[34mHuman %d\x1b[0m | \x1b[31mAI %d\x1b[0m | \x1b[33mTies %d\x1b[0m\n",human_wins,ai_wins,ties);
        int choice;
        printf("Play again? (1=Yes, 0=No): "); fflush(stdout);
        if(scanf("%d",&choice)!=1) break;
        play_again=choice;
        while(getchar()!='\n');
    }

    printf("\x1b[36mThanks for playing!\x1b[0m\n");
    return 0;
}
