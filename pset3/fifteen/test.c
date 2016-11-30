#include <stdio.h>
#include <cs50.h>

int main ()
{
    // Create a empty board with the 
    int board [9][9];
    int d = GetInt();
    // Variable storing the numbers of tiles
    int tile_num = d*d - 1;
    int i, j;
    // Populate the board, d size dimension
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            board[i][j] = tile_num--;
            printf("%i\t", board[i][j]);
        }
        printf("\n");
    }
    // Swap position of 1 and 2 tile depending on size parity of the board
    if (d % 2 == 0)
    {
        // i and j needs to be decremented by 1 to replace in the actual position
        board[i - 1][j - 3] = 1;
        board[i - 1][j - 2] = 2;
    }
     return 0;
}