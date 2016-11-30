/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// blank tile position indexes
int blank_row, blank_column;

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // Variable storing the maximum number of tiles
    int tile_num = d * d - 1;
    
    // Declare indexes outside the loops for later use
    int i, j;
    // Populate the board with number tiles 
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            board[i][j] = tile_num--;
        }
    }
    
    // Swap position of tile 1 and 2 if the size of the border is an even number
    if (d % 2 == 0)
    {
        // i and j needs to be decremented because of the for loops increments 
        board[i - 1][j - 3] = 1;
        board[i - 1][j - 2] = 2;
    }
    // Set position of blank tile
    blank_row = d - 1;
    blank_column = d - 1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    int i, j;
    // Draw the columns
    for (i = 0; i < d; i++)
    {
        // Draw the rows
        for (j = 0; j < d; j++)
        {
            // Change appeareance of 0 tile with "_" character
            if (board[i][j] == 0)
            {
                printf("_\t");
            }
            else
            {
                printf("%i\t", board[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Check the near valid tiles values
    // Check left
    if (blank_row - 1 > -1 && board[blank_row - 1][blank_column] == tile)
    {
        board[blank_row - 1][blank_column] = board[blank_row][blank_column];
        board[blank_row][blank_column] = tile;
        blank_row--;
        return true;
    }
    // Check right
    else if (blank_row + 1 < d && board[blank_row + 1][blank_column] == tile)
    {
        board[blank_row + 1][blank_column] = board[blank_row][blank_column];
        board[blank_row][blank_column] = tile;
        blank_row++;
        return true;
    }
    // Check up
    else if (blank_column - 1 > -1 && board[blank_row][blank_column - 1] == tile)
    {
        board[blank_row][blank_column - 1] = board[blank_row][blank_column];
        board[blank_row][blank_column] = tile;
        blank_column--;
        return true;
    }
    // Check down
    else if (blank_column + 1 < d && board[blank_row][blank_column + 1] == tile)
    {
        board[blank_row][blank_column + 1] = board[blank_row][blank_column];
        board[blank_row][blank_column] = tile;
        blank_column++;
        return true;
    }
    // Ensure the movement margins
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // Variable that stores only the numbered tiles
    int max_tiles = d * d - 1;
    // Variable that stores all the tiles including the blank one
    int all_tiles = d * d;
    // Count the good tiles
    int count = 0;
    
    // Check if position of tiles is correct
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == all_tiles - max_tiles)
            {
                count++;
            }
            max_tiles--;
        }
    }
    
    // True if all tiles are in the right position
    return (count == all_tiles - 1) ? true : false; 
}