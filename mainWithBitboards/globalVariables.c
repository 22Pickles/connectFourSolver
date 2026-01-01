bitboard human_bb = 0, machine_bb = 0;
volatile int spinnerRunning = 0;
volatile int currentDepth = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;