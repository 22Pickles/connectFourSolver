void playGame(){
    int mode = chooseMode();
    int turn = (mode==1)? chooseFirst() : HUMAN;
    int humanWins=0,machineWins=0,ties=0;
    int playAgain=1;

    while(playAgain){
        initBoard();
        while(1){
            printBoard();
            if((mode==1 && turn==HUMAN) || mode==0){
                char line[32]; int col;
                printf("Your move (0-6): "); fflush(stdout);
                if(!fgets(line,sizeof(line),stdin)) continue;
                char *end; col=strtol(line,&end,10);
                if(end==line || !isValidMove(0, col)){ printf("Invalid move.\n"); continue; }
                makeMove(col,(turn==HUMAN)?HUMAN:MACHINE);
                if(checkWin((turn==HUMAN)?HUMAN:MACHINE)){
                    printBoard(); 
                    printf("\x1b[34mPlayer 1 wins!\x1b[0m\n"); humanWins++;
                    break;
                }
            } else {
                int col = bestMove();
                makeMove(col,MACHINE);
                if(checkWin(MACHINE)){ printBoard(); printf("\x1b[31mMACHINE wins!\x1b[0m\n"); machineWins++; break; }
            }

            if(isFull()){ printBoard(); printf("\x1b[33mIt's a tie!\x1b[0m\n"); ties++; break; }
            turn = (mode==0)? ((turn==HUMAN)?MACHINE:HUMAN) : -turn;
        }

        printf("\nScoreboard: \x1b[34mHuman %d\x1b[0m | \x1b[31mMACHINE %d\x1b[0m | \x1b[33mTies %d\x1b[0m\n",humanWins,machineWins,ties);
        int choice; printf("Play again? (1=Yes,0=No): "); fflush(stdout);
        if(scanf("%d",&choice)!=1) break;
        playAgain=choice; while(getchar()!='\n');
    }
    printf("\x1b[36mThanks for playing!\x1b[0m\n");
}