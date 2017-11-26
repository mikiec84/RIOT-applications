/*
 * Based on the example code by Daniel Ángel Jiménez, which is placed in the
 * public domain: http://www.cs.utexas.edu/users/djimenez/utsa/cs1713-3/c/.
 */

#include <stdio.h>

#include "random.h"
#include "u8g2.h"
#include "xtimer.h"

/* game settings */
#define BOARD_WIDTH     (102)
#define BOARD_HEIGHT    (32)

#define BOARD_CLUSTERS  (32)
#define BOARD_SEED      (98374912)

/* board (and newboard) are declared in advance */
uint8_t	board[BOARD_WIDTH][BOARD_HEIGHT];
uint8_t newboard[BOARD_WIDTH][BOARD_HEIGHT];

/* initialize a new board */
void initialize_board(uint8_t board[][BOARD_HEIGHT]) {
    int	i, j, k;

    /* clear the board */
    for (i = 0; i < BOARD_WIDTH; i++) {
        for (j = 0; j < BOARD_HEIGHT; j++) {
            board[i][j] = 0;
        }
    }

    /* add some random clusters */
    for (k = 0; k < BOARD_CLUSTERS; k++) {
        i = random_uint32_range(0, BOARD_WIDTH);
        j = random_uint32_range(0, BOARD_HEIGHT);

        board[i][j] = 1;
        board[(i + 1) % BOARD_WIDTH][j] = 1;
        board[i][(j + 1) % BOARD_HEIGHT] = 1;
        board[(i + 1) % BOARD_WIDTH][(j + 1) % BOARD_HEIGHT] = 1;
    }
}

/* add to a width index, wrapping around like a cylinder */
int xadd(int i, int a) {
    i += a;

    while (i < 0) {
        i += BOARD_WIDTH;
    }
    while (i >= BOARD_WIDTH) {
        i -= BOARD_WIDTH;
    }

    return i;
}

/* add to a height index, wrapping around */
int yadd(int i, int a) {
    i += a;

    while (i < 0) {
        i += BOARD_HEIGHT;
    }
    while (i >= BOARD_HEIGHT) {
        i -= BOARD_HEIGHT;
    }

    return i;
}

/* return the number of on cells adjacent to the i,j cell */
int adjacent_to(uint8_t board[][BOARD_HEIGHT], int i, int j) {
    int	k, l, count;

    count = 0;

    /* go around the cell */
    for (k = -1; k <= 1; k++) {
        for (l = -1; l <= 1; l++) {
            /* only count if at least one of k,l isn't zero */
            if (k || l) {
                if (board[xadd(i,k)][yadd(j,l)]) {
                    count++;
                }
            }
        }
    }

    return count;
}

/* play one iteration of the game */
void play(uint8_t board[][BOARD_HEIGHT]) {
    int	i, j, a;

    /* for each cell, apply the rules of Life */
    for (i = 0; i < BOARD_WIDTH; i++) {
        for (j = 0; j < BOARD_HEIGHT; j++) {
            a = adjacent_to(board, i, j);

            if (a == 2) {
                newboard[i][j] = board[i][j];
            }
            if (a == 3) {
                newboard[i][j] = 1;
            }
            if (a < 2) {
                newboard[i][j] = 0;
            }
            if (a > 3) {
                newboard[i][j] = 0;
            }
        }
    }

    /* copy the new board back into the old board */
    for (i = 0; i < BOARD_WIDTH; i++) {
        for (j=0; j < BOARD_HEIGHT; j++) {
            board[i][j] = newboard[i][j];
        }
    }
}

/* print the life board */
void print(uint8_t board[][BOARD_HEIGHT], u8g2_t* u8g2) {
    int	i, j;

    /* print each row and column position */
    u8g2_FirstPage(u8g2);

    do {
        for (j = 0; j < BOARD_HEIGHT; j++) {
            for (i = 0; i < BOARD_WIDTH; i++) {
                if (board[i][j] == 1) {
                    u8g2_DrawPixel(u8g2, i, j);
                }
            }
        }
    } while (u8g2_NextPage(u8g2));

    /* transfer screen buffer to stdout */
    utf8_show();
}

/* main program */
int main(void) {
    u8g2_t u8g2;

    /* initialize the display */
    u8g2_SetupBuffer_Utf8(&u8g2, U8G2_R0);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    /* explicitly seed the prng */
    random_init(BOARD_SEED);

    /* prepare a board */
    initialize_board(board);

    /* keep on playing the game */
    while (1) {
        print(board, &u8g2);
        play(board);

        xtimer_usleep(100 * 1000);
    }

    /* should not end up here */
    return 1;
}
