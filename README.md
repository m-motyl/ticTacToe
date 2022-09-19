# Tic-tac-toe game with shared memory and semaphores

One program for two separate players (two terminals) implementing tic-tac-toe game. 
The board is placed in shared memory and synchronization of acces is ensured using semaphores.
Program is universal for both players - the initial competition for the first move wins this
process, which first creates any of the semaphores or shared memory.
The program checks the correctness of moves, display appropriate messages to both players
and recognizes the game states.

## remarks
* run program in two separate terminals
