void printBoard(){
    system("clear");
    printf(" ┌───────────────┐\n │ 0 1 2 3 4 5 6 │\n");
    for(int row=0;row<ROWS;row++){
        printf(" │ ");
        for(int col=0;col<COLS;col++){
            int bitIndex = col + row * COLS;
            if(machine_bb & (1ULL << bitIndex)) printf("\x1b[31mX\x1b[0m ");
            else if(human_bb & (1ULL << bitIndex)) printf("\x1b[34mO\x1b[0m ");
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
