/**
 * fifteen.c
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
    FILE *file = fopen("log.txt", "w");
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
        int tile = get_int();
        
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
    int value = (d * d) - 1;
    
    // In case of even dimensions, we'll need to swap 1 and 2
    bool wonky = d % 2 == 0;
    
    for (int x = 0; x < d; x++)
    {
        for (int y = 0; y < d; y++)
        {
            if(wonky)
            {
                if(value == 1)
                    board[x][y] = 2;
                else if(value ==2)
                    board[x][y] = 1;
                else
                    board[x][y] = value;
            }
            else
                board[x][y] = value;
            
            value --;
        }
    }
    
    printf("Board initialized. PS: Eat a sandwich if you're hungry, dear.\n");
    usleep(500000);
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    
    printf("\n");
    
    for (int x = 0; x < d; x++)
    {
        for (int y = 0; y < d; y++)
        {
            if(board[x][y] == 0)
                printf("  _");
            else
                printf(" %2d", board[x][y]);
        }
        
        printf ("\n");
    }

    printf ("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    if (tile < 1 || tile > (d * d) - 1)
        return false;

    // find 0 position
    int ox;
    int oy;
    
    for (int x = 0; x < d; x++)
    {
        for (int y=0; y < d; y++)
        {
            if (board[x][y] == 0)
            {
                ox = x;
                oy = y;
            }
        }
    }
    
    // check adjacent tiles to see if they contain the target
    int tx;
    int ty;
    
    for (int x = 0; x < 4; x++)
    {
        switch (x)
        {
            case 0: // To the left!
                tx = ox - 1;
                ty = oy;
                break;
            case 1: // To the right!
                tx = ox + 1;
                ty = oy;
                break;
            case 2: // To the ground!
                tx = ox;
                ty = oy + 1;
                break;
            case 3: // To the sky!
                tx = ox;
                ty = oy - 1;
                break;
        }
        
        if (0 <= tx && tx < d && 0 <= ty && ty < d)
        {
            if (board[tx][ty] == tile)
            {
                // bingo bango! let's swap
                board[ox][oy] = tile;
                board[tx][ty] = 0;
                
                return true;
            }
        }
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // Winning configuration is 1, 2, 3, ..., MAX, 0
    
    int value = 1;
    int total = d * d;
    
    for (int x = 0; x < d; x++)
    {
        for (int y = 0; y < d; y++)
        {
            if (board[x][y] != value % total)
            {
                printf("Wanted to see %i at %i,%i but got %i instead! (value: %i, total: %i)\n", value % total, x, y, board[x][y], value, total);
                return false;
            }
            
            value++;
        }
    }
    
    return true;
}
