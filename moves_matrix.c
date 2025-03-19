#include "chess_funcs.h"

void show_matrix(int **matrix)
{
    for (int y = BOARD_SIZE - 1, x; y >= 0; --y)
    {
        for (x = 0; x < BOARD_SIZE; ++x)
        {
            printf("%d ", matrix[x][y]);
        }
        puts("");
    }
}

void reset_moves_matrix(int **moves_matrix)
{
    for (int i = 0, j; i < BOARD_SIZE; ++i)
        for (j = 0; j < BOARD_SIZE; ++j)
            moves_matrix[i][j] = 0;
}

static void fill_moves_matrix_pawn(ChessBoard *chb, int **moves_matrix, int x, int y, int color)
{
    if (color == white)
    {
        if (y + 1 < BOARD_SIZE)
        {
            if (chb->board[x][y + 1].type == empty)
                moves_matrix[x][y + 1] = 1;
            if (x - 1 >= 0)
            {
                if (chb->board[x - 1][y + 1].color == black)
                    moves_matrix[x - 1][y + 1] = 2;
                if ((x - 1 == chb->enpas_x) && (chb->enpas_y == y))
                    moves_matrix[x - 1][y + 1] = 2;
            }
            if (x + 1 < BOARD_SIZE)
            {
                if (chb->board[x + 1][y + 1].color == black)
                    moves_matrix[x + 1][y + 1] = 2;
                if ((x + 1 == chb->enpas_x) && (chb->enpas_y == y))
                    moves_matrix[x + 1][y + 1] = 2;
            }
        }
        if ((y == 1) && (moves_matrix[x][2] == 1) && (chb->board[x][3].type == empty))
            moves_matrix[x][3] = 1;
    }
    else
    {
        if (y - 1 >= 0)
        {
            if (chb->board[x][y - 1].type == empty)
                moves_matrix[x][y - 1] = 1;
            if (x - 1 >= 0)
            {
                if (chb->board[x - 1][y - 1].color == white)
                    moves_matrix[x - 1][y - 1] = 2;
                if ((chb->is_enpassant) && (x - 1 == chb->enpas_x) && (chb->enpas_y == y))
                    moves_matrix[x - 1][y - 1] = 2;
            }
            if (x + 1 < BOARD_SIZE)
            {
                if (chb->board[x + 1][y - 1].color == white)
                    moves_matrix[x + 1][y - 1] = 2;
                if ((chb->is_enpassant) && (x + 1 == chb->enpas_x) && (chb->enpas_y == y))
                    moves_matrix[x + 1][y - 1] = 2;
            }
        }
        if ((y == 6) && (moves_matrix[x][5] == 1) && (chb->board[x][4].type == empty))
            moves_matrix[x][4] = 1;
    }
}

static void fill_moves_matrix_rook(ChessBoard *chb, int **moves_matrix, int x, int y, int color)
{
    int i;

    for (i = 1; y + i < BOARD_SIZE; ++i)
    {
        if (chb->board[x][y + i].type == empty)
        {
            moves_matrix[x][y + i] = 1;
        }
        else
        {
            break;
        }
    }
    if ((y + i < BOARD_SIZE) && (chb->board[x][y + i].color != color))
        moves_matrix[x][y + i] = 2;

    for (i = 1; y - i >= 0; ++i)
    {
        if (chb->board[x][y - i].type == empty)
        {
            moves_matrix[x][y - i] = 1;
        }
        else
        {
            break;
        }
    }
    if ((y - i >= 0) && (chb->board[x][y - i].color != color))
        moves_matrix[x][y - i] = 2;

    for (i = 1; x + i < BOARD_SIZE; ++i)
    {
        if (chb->board[x + i][y].type == empty)
        {
            moves_matrix[x + i][y] = 1;
        }
        else
        {
            break;
        }
    }
    if ((x + i < BOARD_SIZE) && (chb->board[x + i][y].color != color))
        moves_matrix[x + i][y] = 2;

    for (i = 1; x - i >= 0; ++i)
    {
        if (chb->board[x - i][y].type == empty)
        {
            moves_matrix[x - i][y] = 1;
        }
        else
        {
            break;
        }
    }
    if ((x - i >= 0) && (chb->board[x - i][y].color != color))
        moves_matrix[x - i][y] = 2;
}

static void fill_moves_matrix_knight(ChessBoard *chb, int **moves_matrix, int x, int y, int color)
{
    if ((x + 2 < BOARD_SIZE) && (y + 1 < BOARD_SIZE))
    {
        if (chb->board[x + 2][y + 1].type == empty)
        {
            moves_matrix[x + 2][y + 1] = 1;
        }
        else if (chb->board[x + 2][y + 1].color != color)
        {
            moves_matrix[x + 2][y + 1] = 2;
        }
    }

    if ((x + 2 < BOARD_SIZE) && (y - 1 >= 0))
    {
        if (chb->board[x + 2][y - 1].type == empty)
        {
            moves_matrix[x + 2][y - 1] = 1;
        }
        else if (chb->board[x + 2][y - 1].color != color)
        {
            moves_matrix[x + 2][y - 1] = 2;
        }
    }

    if ((x - 2 >= 0) && (y + 1 < BOARD_SIZE))
    {
        if (chb->board[x - 2][y + 1].type == empty)
        {
            moves_matrix[x - 2][y + 1] = 1;
        }
        else if (chb->board[x - 2][y + 1].color != color)
        {
            moves_matrix[x - 2][y + 1] = 2;
        }
    }

    if ((x - 2 >= 0) && (y - 1 >= 0))
    {
        if (chb->board[x - 2][y - 1].type == empty)
        {
            moves_matrix[x - 2][y - 1] = 1;
        }
        else if (chb->board[x - 2][y - 1].color != color)
        {
            moves_matrix[x - 2][y - 1] = 2;
        }
    }

    if ((x + 1 < BOARD_SIZE) && (y + 2 < BOARD_SIZE))
    {
        if (chb->board[x + 1][y + 2].type == empty)
        {
            moves_matrix[x + 1][y + 2] = 1;
        }
        else if (chb->board[x + 1][y + 2].color != color)
        {
            moves_matrix[x + 1][y + 2] = 2;
        }
    }

    if ((x + 1 < BOARD_SIZE) && (y - 2 >= 0))
    {
        if (chb->board[x + 1][y - 2].type == empty)
        {
            moves_matrix[x + 1][y - 2] = 1;
        }
        else if (chb->board[x + 1][y - 2].color != color)
        {
            moves_matrix[x + 1][y - 2] = 2;
        }
    }

    if ((x - 1 >= 0) && (y + 2 < BOARD_SIZE))
    {
        if (chb->board[x - 1][y + 2].type == empty)
        {
            moves_matrix[x - 1][y + 2] = 1;
        }
        else if (chb->board[x - 1][y + 2].color != color)
        {
            moves_matrix[x - 1][y + 2] = 2;
        }
    }

    if ((x - 1 >= 0) && (y - 2 >= 0))
    {
        if (chb->board[x - 1][y - 2].type == empty)
        {
            moves_matrix[x - 1][y - 2] = 1;
        }
        else if (chb->board[x - 1][y - 2].color != color)
        {
            moves_matrix[x - 1][y - 2] = 2;
        }
    }
}

static void fill_moves_matrix_bishop(ChessBoard *chb, int **moves_matrix, int x, int y, int color)
{
    int i;

    for (i = 1; (y + i < BOARD_SIZE) && (x + i < BOARD_SIZE); ++i)
    {
        if (chb->board[x + i][y + i].type == empty)
        {
            moves_matrix[x + i][y + i] = 1;
        }
        else
        {
            break;
        }
    }
    if ((y + i < BOARD_SIZE) && (x + i < BOARD_SIZE) && (chb->board[x + i][y + i].color != color))
        moves_matrix[x + i][y + i] = 2;

    for (i = 1; (y + i < BOARD_SIZE) && (x - i >= 0); ++i)
    {
        if (chb->board[x - i][y + i].type == empty)
        {
            moves_matrix[x - i][y + i] = 1;
        }
        else
        {
            break;
        }
    }
    if ((y + i < BOARD_SIZE) && (x - i >= 0) && (chb->board[x - i][y + i].color != color))
        moves_matrix[x - i][y + i] = 2;

    for (i = 1; (y - i >= 0) && (x + i < BOARD_SIZE); ++i)
    {
        if (chb->board[x + i][y - i].type == empty)
        {
            moves_matrix[x + i][y - i] = 1;
        }
        else
        {
            break;
        }
    }
    if ((y - i >= 0) && (x + i < BOARD_SIZE) && (chb->board[x + i][y - i].color != color))
        moves_matrix[x + i][y - i] = 2;

    for (i = 1; (y - i >= 0) && (x - i >= 0); ++i)
    {
        if (chb->board[x - i][y - i].type == empty)
        {
            moves_matrix[x - i][y - i] = 1;
        }
        else
        {
            break;
        }
    }
    if ((y - i >= 0) && (x - i >= 0) && (chb->board[x - i][y - i].color != color))
        moves_matrix[x - i][y - i] = 2;
}

static void fill_moves_matrix_queen(ChessBoard *chb, int **moves_matrix, int x, int y, int color)
{
    fill_moves_matrix_rook(chb, moves_matrix, x, y, color);
    fill_moves_matrix_bishop(chb, moves_matrix, x, y, color);
}

static void fill_moves_matrix_king(ChessBoard *chb, int **moves_matrix, int x, int y, int color)
{
    if (y + 1 < BOARD_SIZE)
    {
        if ((x - 1 >= 0) && (chb->board[x - 1][y + 1].color != color))
        {
            if (chb->board[x - 1][y + 1].type == empty)
            {
                moves_matrix[x - 1][y + 1] = 1;
            }
            else
            {
                moves_matrix[x - 1][y + 1] = 2;
            }
        }

        if (chb->board[x][y + 1].color != color)
        {
            if (chb->board[x][y + 1].type == empty)
            {
                moves_matrix[x][y + 1] = 1;
            }
            else
            {
                moves_matrix[x][y + 1] = 2;
            }
        }

        if ((x + 1 < BOARD_SIZE) && (chb->board[x + 1][y + 1].color != color))
        {
            if (chb->board[x + 1][y + 1].type == empty)
            {
                moves_matrix[x + 1][y + 1] = 1;
            }
            else
            {
                moves_matrix[x + 1][y + 1] = 2;
            }
        }
    }

    if ((x - 1 >= 0) && (chb->board[x - 1][y].color != color))
    {
        if (chb->board[x - 1][y].type == empty)
        {
            moves_matrix[x - 1][y] = 1;
        }
        else
        {
            moves_matrix[x - 1][y] = 2;
        }
    }

    if ((x + 1 < BOARD_SIZE) && (chb->board[x + 1][y].color != color))
    {
        if (chb->board[x + 1][y].type == empty)
        {
            moves_matrix[x + 1][y] = 1;
        }
        else
        {
            moves_matrix[x + 1][y] = 2;
        }
    }

    if (y - 1 >= 0)
    {
        if ((x - 1 >= 0) && (chb->board[x - 1][y - 1].color != color))
        {
            if (chb->board[x - 1][y - 1].type == empty)
            {
                moves_matrix[x - 1][y - 1] = 1;
            }
            else
            {
                moves_matrix[x - 1][y - 1] = 2;
            }
        }

        if (chb->board[x][y - 1].color != color)
        {
            if (chb->board[x][y - 1].type == empty)
            {
                moves_matrix[x][y - 1] = 1;
            }
            else
            {
                moves_matrix[x][y - 1] = 2;
            }
        }

        if ((x + 1 < BOARD_SIZE) && (chb->board[x + 1][y - 1].color != color))
        {
            if (chb->board[x + 1][y - 1].type == empty)
            {
                moves_matrix[x + 1][y - 1] = 1;
            }
            else
            {
                moves_matrix[x + 1][y - 1] = 2;
            }
        }
    }
}

void fill_moves_matrix(ChessBoard *chb, int **moves_matrix, int x, int y, int color)
{
    switch (chb->board[x][y].type)
    {
    case pawn:
        fill_moves_matrix_pawn(chb, moves_matrix, x, y, color);
        break;
    case rook:
        fill_moves_matrix_rook(chb, moves_matrix, x, y, color);
        break;
    case knight:
        fill_moves_matrix_knight(chb, moves_matrix, x, y, color);
        break;
    case bishop:
        fill_moves_matrix_bishop(chb, moves_matrix, x, y, color);
        break;
    case queen:
        fill_moves_matrix_queen(chb, moves_matrix, x, y, color);
        break;
    case king:
        fill_moves_matrix_king(chb, moves_matrix, x, y, color);
        break;
    default:
        break;
    }
}

static void fill_enemy_moves_matrix(ChessBoard *chb, int color)
{
    for (int x = 0, y; x < BOARD_SIZE; ++x)
    {
        for (y = 0; y < BOARD_SIZE; ++y)
        {
            if (chb->board[x][y].color == !color)
                fill_moves_matrix(chb, chb->enemy_moves_matrix, x, y, !color);
        }
    }
}

bool iskingsafe(ChessBoard *chb, int x1, int y1, int color, int x2, int y2)
{
    int pos2_type = chb->board[x2][y2].type;
    int pos2_color = chb->board[x2][y2].color;

    if (chb->board[x1][y1].type == king)
    {
        switch (color)
        {
        case white:
            chb->white_king_x = x2;
            chb->white_king_y = y2;
            break;
        case black:
            chb->black_king_x = x2;
            chb->black_king_y = y2;
            break;
        }
    }
    move_figure(chb->board, x1, y1, x2, y2);

    reset_moves_matrix(chb->enemy_moves_matrix);
    fill_enemy_moves_matrix(chb, color);

    move_figure(chb->board, x2, y2, x1, y1);
    chb->board[x2][y2].type = pos2_type;
    chb->board[x2][y2].color = pos2_color;

    switch (color)
    {
    case black:
        if (chb->enemy_moves_matrix[chb->black_king_x][chb->black_king_y] == 0)
        {
            if (chb->board[x1][y1].type == king)
            {

                chb->black_king_x = x1;
                chb->black_king_y = y1;
            }
            return 1;
        }
        if (chb->board[x1][y1].type == king)
        {

            chb->black_king_x = x1;
            chb->black_king_y = y1;
        }
        break;
    case white:
        if (chb->enemy_moves_matrix[chb->white_king_x][chb->white_king_y] == 0)
        {
            if (chb->board[x1][y1].type == king)
            {
                chb->white_king_x = x1;
                chb->white_king_y = y1;
            }
            return 1;
        }
        if (chb->board[x1][y1].type == king)
        {
            chb->white_king_x = x1;
            chb->white_king_y = y1;
        }
        break;
    default:
        printf("\x1b[31mОшибка:\x1b[0m неизвестный цвет\n");
        exit_failure(chb);
    }
    return 0;
}

void create_correct_moves_matrix(ChessBoard *chb, int x1, int y1)
{
    int color = chb->board[x1][y1].color;

    reset_moves_matrix(chb->moves_matrix);
    fill_moves_matrix(chb, chb->moves_matrix, x1, y1, color);

    for (int x = 0, y; x < BOARD_SIZE; ++x)
    {
        for (y = 0; y < BOARD_SIZE; ++y)
        {
            if ((chb->moves_matrix[x][y] != 0))
            {
                if (!iskingsafe(chb, x1, y1, color, x, y))
                {
                    chb->moves_matrix[x][y] = 0;
                }
            }
        }
    }
}

void make_long_castle(ChessBoard *chb)
{
    switch ((int)chb->player)
    {
    case white:
        if (chb->can_white_long_castle)
        {
            create_correct_moves_matrix(chb, 0, 0);
            if (chb->enemy_moves_matrix[4][0] == 2)
            {
                printf("\x1b[31mОшибка:\x1b[0m король под шахом\n");
                exit_failure(chb);
            }
            if (chb->moves_matrix[3][0] != 1)
            {
                printf("\x1b[31mОшибка:\x1b[0m путь до короля не свободен\n");
                exit_failure(chb);
            }
            for (int x = 1; x < 4; ++x)
            {
                if (!iskingsafe(chb, 4, 0, white, x, 0))
                {
                    printf("\x1b[31mОшибка:\x1b[0m путь короля под боем\n");
                    exit_failure(chb);
                }
            }
        }
        else
        {
            printf("\x1b[31mОшибка:\x1b[0m право рокировки отсутствует\n");
            exit_failure(chb);
        }

        move_figure(chb->board, 4, 0, 2, 0);
        chb->white_king_x = 2;
        chb->can_white_long_castle = 0;
        chb->can_white_short_castle = 0;
        move_figure(chb->board, 0, 0, 3, 0);
        break;
    case black:
        if (chb->can_black_long_castle)
        {
            create_correct_moves_matrix(chb, 0, 7);
            if (chb->enemy_moves_matrix[4][7] == 2)
            {
                printf("\x1b[31mОшибка:\x1b[0m король под шахом\n");
                exit_failure(chb);
            }
            if (chb->moves_matrix[3][7] != 1)
            {
                printf("\x1b[31mОшибка:\x1b[0m путь до короля не свободен\n");
                exit_failure(chb);
            }
            for (int x = 1; x < 4; ++x)
            {
                if (!iskingsafe(chb, 4, 7, white, x, 7))
                {
                    printf("\x1b[31mОшибка:\x1b[0m путь короля под боем\n");
                    exit_failure(chb);
                }
            }
        }
        else
        {
            printf("\x1b[31mОшибка:\x1b[0m право рокировки отсутствует\n");
            exit_failure(chb);
        }

        move_figure(chb->board, 4, 7, 2, 7);
        chb->white_king_x = 2;
        chb->can_black_long_castle = 0;
        chb->can_black_short_castle = 0;
        move_figure(chb->board, 0, 7, 3, 7);
        break;
    default:
        printf("\x1b[31mОшибка:\x1b[0m неизвестный цвет игрока\n");
        exit_failure(chb);
        break;
    }
}

void make_short_castle(ChessBoard *chb)
{
    switch ((int)chb->player)
    {
    case white:
        if (chb->can_white_short_castle)
        {
            create_correct_moves_matrix(chb, 7, 0);
            if (chb->enemy_moves_matrix[4][0] == 2)
            {
                printf("\x1b[31mОшибка:\x1b[0m король под шахом\n");
                exit_failure(chb);
            }
            if (chb->moves_matrix[5][0] != 1)
            {
                printf("\x1b[31mОшибка:\x1b[0m путь до короля не свободен\n");
                exit_failure(chb);
            }
            for (int x = 6; x >= 5; --x)
            {
                if (!iskingsafe(chb, 4, 0, white, x, 0))
                {
                    printf("\x1b[31mОшибка:\x1b[0m путь короля под боем\n");
                    exit_failure(chb);
                }
            }
        }
        else
        {
            printf("\x1b[31mОшибка:\x1b[0m право рокировки отсутствует\n");
            exit_failure(chb);
        }

        move_figure(chb->board, 4, 0, 6, 0);
        chb->white_king_x = 6;
        chb->can_white_long_castle = 0;
        chb->can_white_short_castle = 0;
        move_figure(chb->board, 7, 0, 5, 0);
        break;
    case black:
        if (chb->can_black_short_castle)
        {
            create_correct_moves_matrix(chb, 7, 7);
            if (chb->enemy_moves_matrix[4][7] == 2)
            {
                printf("\x1b[31mОшибка:\x1b[0m король под шахом\n");
                exit_failure(chb);
            }
            if (chb->moves_matrix[5][7] != 1)
            {
                printf("\x1b[31mОшибка:\x1b[0m путь до короля не свободен\n");
                exit_failure(chb);
            }
            for (int x = 6; x >= 5; --x)
            {
                if (!iskingsafe(chb, 4, 7, white, x, 7))
                {
                    printf("\x1b[31mОшибка:\x1b[0m путь короля под боем\n");
                    exit_failure(chb);
                }
            }
        }
        else
        {
            printf("\x1b[31mОшибка:\x1b[0m право рокировки отсутствует\n");
            exit_failure(chb);
        }

        move_figure(chb->board, 4, 7, 6, 7);
        chb->white_king_x = 6;
        chb->can_black_long_castle = 0;
        chb->can_black_short_castle = 0;
        move_figure(chb->board, 7, 7, 5, 7);
        break;
    default:
        printf("\x1b[31mОшибка:\x1b[0m неизвестный цвет игрока\n");
        exit_failure(chb);
        break;
    }
}
