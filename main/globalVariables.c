int board[ROWS][COLS];
volatile int spinnerRunning = 0;
volatile int currentDepth = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;