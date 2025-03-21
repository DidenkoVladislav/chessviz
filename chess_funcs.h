#ifndef __CHESS_FUCNS__
#define __CHESS_FUCNS__

#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8
#define KING 0x2654

enum
{
    empty = -1,
    king = 0,
    queen = 1,
    rook = 2,
    bishop = 3,
    knight = 4,
    pawn = 5,
    white = 0,
    black = 1
};

typedef unsigned int uint;

// Структура шахматной фигуры
typedef struct
{
    int type;
    int color;
} CHP;

// Структура шахматной доски
typedef struct
{
    CHP **board;
    int **moves_matrix;
    int **enemy_moves_matrix;
    bool player;
    int move_counter;
    int cur_x1, cur_y1;
    int black_king_x, black_king_y;
    int white_king_x, white_king_y;
    bool is_enpassant;
    int enpas_x, enpas_y;
    bool can_white_short_castle, can_white_long_castle;
    bool can_black_short_castle, can_black_long_castle;
    bool is_test;
} ChessBoard;

void start_game(int argc, char const *argv[]);

void chessboard_constructor(ChessBoard *chb);
void print_move(int x, int y);
CHP **create_board(void);
int **create_moves_matrix(void);
void set_default_board(ChessBoard *chb);
void show_board(ChessBoard *chb);
int select_figure(ChessBoard *chb);
void create_correct_moves_matrix(ChessBoard *chb, int x1, int y1);
void make_long_castle(ChessBoard *chb);
void make_short_castle(ChessBoard *chb);
void move_figure(CHP **board, int x1, int y1, int x2, int y2);
int select_target(ChessBoard *chb);
void reset_moves_matrix(int **moves_matrix);
bool iskingsafe(ChessBoard *chb, int x1, int y1, int color, int x2, int y2);

void destroy_chessboard(ChessBoard *chb);

void exit_failure(ChessBoard *chb);

#endif
