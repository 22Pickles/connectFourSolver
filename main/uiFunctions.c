void printBoard(){
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
