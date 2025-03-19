#include "chess_funcs.h"

void exit_failure(ChessBoard *chb)
{
    destroy_chessboard(chb);
    exit(EXIT_FAILURE);
}

void chessboard_constructor(ChessBoard *chb)
{
    chb->board = create_board();
    chb->moves_matrix = create_moves_matrix();
    chb->enemy_moves_matrix = create_moves_matrix();
    chb->player = white;
    chb->move_counter = 1;
    chb->is_enpassant = 0;
    chb->enpas_x = 0;
    chb->enpas_y = 0;
    chb->can_black_long_castle = 1;
    chb->can_black_short_castle = 1;
    chb->can_white_long_castle = 1;
    chb->can_white_short_castle = 1;
    chb->is_test = 0;
    set_default_board(chb);
}

void print_move(int x, int y)
{
    printf("%c%d", x + 'a', y + 1);
}

CHP **create_board(void)
{
    CHP **board = (CHP **)calloc(BOARD_SIZE * BOARD_SIZE, sizeof(CHP *));
    if (board == NULL)
    {
        printf("\x1b[31mОшибка выделения памяти.\x1b[0m");
        abort();
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board[i] = (CHP *)calloc(BOARD_SIZE, sizeof(CHP));

        if (board[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(board[j]);
            free(board);
            abort();
        }
    }

    return board;
}

int **create_moves_matrix(void)
{
    int **moves_matrix = (int **)calloc(BOARD_SIZE, sizeof(int *));
    if (moves_matrix == NULL)
    {
        printf("\x1b[31mОшибка выделения памяти.\x1b[0m");
        abort();
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        moves_matrix[i] = (int *)calloc(BOARD_SIZE, sizeof(int));

        if (moves_matrix[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(moves_matrix[j]);
            free(moves_matrix);
            abort();
        }
    }

    return moves_matrix;
}

void set_default_board(ChessBoard *chb)
{
    chb->board[0][0].type = rook;
    chb->board[1][0].type = knight;
    chb->board[2][0].type = bishop;
    chb->board[3][0].type = queen;
    chb->board[4][0].type = king;
    chb->white_king_x = 4;
    chb->white_king_y = 0;
    chb->board[5][0].type = bishop;
    chb->board[6][0].type = knight;
    chb->board[7][0].type = rook;

    for (int x = 0; x < BOARD_SIZE; ++x)
    {
        chb->board[x][1].type = pawn;
    }

    for (int y = 0, x; y < 2; ++y)
    {
        for (x = 0; x < BOARD_SIZE; ++x)
        {
            chb->board[x][y].color = white;
        }
    }

    for (int y = 2, x; y < BOARD_SIZE - 2; ++y)
    {
        for (x = 0; x < BOARD_SIZE; ++x)
        {
            chb->board[x][y].type = empty;
            chb->board[x][y].color = empty;
        }
    }

    for (int x = 0; x < BOARD_SIZE; ++x)
    {
        chb->board[x][6].type = pawn;
    }

    chb->board[0][7].type = rook;
    chb->board[1][7].type = knight;
    chb->board[2][7].type = bishop;
    chb->board[3][7].type = queen;
    chb->board[4][7].type = king;
    chb->black_king_x = 4;
    chb->black_king_y = 7;
    chb->board[5][7].type = bishop;
    chb->board[6][7].type = knight;
    chb->board[7][7].type = rook;

    for (int y = 6, x; y < BOARD_SIZE; ++y)
    {
        for (x = 0; x < BOARD_SIZE; ++x)
        {
            chb->board[x][y].color = black;
        }
    }
}

void show_board(ChessBoard *chb)
{
    printf("\n\n");
    for (int y = BOARD_SIZE - 1, x; y >= 0; --y)
    {
        printf(" %d ", y + 1);
        for (x = 0; x < BOARD_SIZE; ++x)
        {
            if (chb->board[x][y].type == empty)
            {
                if (chb->moves_matrix[x][y] == 0)
                {
                    printf("\x1b[%sm  \x1b[0m", ((x + y) % 2) ? "30;47" : "30;48;2;128;128;128");
                }
                else
                {
                    printf("\x1b[%sm  \x1b[0m", ((x + y) % 2) ? "30;45" : "30;48;2;128;0;128");
                }
            }
            else
            {
                if (chb->moves_matrix[x][y] == 0)
                {
                    printf("\x1b[%sm%lc \x1b[0m", ((x + y) % 2) ? "30;47" : "30;48;2;128;128;128", KING + (chb->board[x][y].type + 6 * chb->board[x][y].color));
                }
                else
                {
                    printf("\x1b[%sm%lc \x1b[0m", ((x + y) % 2) ? "30;45" : "30;48;2;128;0;128", KING + (chb->board[x][y].type + 6 * chb->board[x][y].color));
                }
            }
        }
        printf("\n");
    }
    printf("   a b c d e f g h\n\n");
}

int select_figure(ChessBoard *chb)
{
    char input_string[7] = {'\0'}, ch = '\0';
    char x1 = '\0';
    uint shift = 0;
    int y1 = 0;

    for (int i = 0; i < 6; ++i)
    {
        if ((ch = getchar()) == EOF)
        {

            printf("\x1b[31mОшибка считывания\x1b[0m\n");
            exit_failure(chb);
        }
        if (ch == 'q')
        {
            return -1;
        }
        input_string[i] = ch;
        if (isdigit(ch))
        {
            break;
        }
        if ((i == 3) && (strcmp(input_string, "O-O\n") == 0))
            return -2;

        if ((i == 5) && (strcmp(input_string, "O-O-O\n") == 0))
            return -3;
    }

    // Проверки
    if (input_string[0] >= 'a' && input_string[0] <= 'h')
    {
        x1 = input_string[0];
        y1 = input_string[1] - '0';
    }
    else
    {
        switch (input_string[0])
        {
        case 'K':
        case 'Q':
        case 'R':
        case 'B':
        case 'N':
            x1 = input_string[1];
            y1 = input_string[2] - '0';
            shift = 1;
            break;

        default:
            printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m", input_string[0]);
            for (int i = 2 + shift; i < 9; ++i)
            {
                putchar(input_string[i]);
            }
            puts(" ожидалось '' | 'K' | 'Q' | 'R' | 'B' | 'N'");
            exit_failure(chb);
        }
    }

    if (chb->is_test)
    {
        switch (input_string[0])
        {
        case 'K':
        case 'Q':
        case 'R':
        case 'B':
        case 'N':
            putchar(input_string[0]);
            break;
        default:
            break;
        }
    }

    if ((y1 < 1) | (y1 > 9))
    {
        printf("\x1b[31mОшибка:\x1b[0m некорректный ввод: %c\x1b[31m%c\x1b[0m", x1, y1 + '0');
        for (int i = 2 + shift; i < 9; ++i)
        {
            putchar(input_string[i]);
        }
        exit_failure(chb);
    }

    switch (input_string[0])
    {
    case 'K':
        if (chb->board[x1 - 'a'][y1 - 1].type != king)
        {
            printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m", input_string[0]);
            for (int i = 2 + shift; i < 5; ++i)
            {
                putchar(input_string[i]);
            }
            puts(" ожидалось 'K'");
            exit_failure(chb);
        }
        break;
    case 'Q':
        if (chb->board[x1 - 'a'][y1 - 1].type != queen)
        {
            printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m", input_string[0]);
            for (int i = 2 + shift; i < 5; ++i)
            {
                putchar(input_string[i]);
            }
            puts(" ожидалось 'Q'");
            exit_failure(chb);
        }
        break;
    case 'R':
        if (chb->board[x1 - 'a'][y1 - 1].type != rook)
        {
            printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m", input_string[0]);
            for (int i = 2 + shift; i < 5; ++i)
            {
                putchar(input_string[i]);
            }
            puts(" ожидалось 'R'");
            exit_failure(chb);
        }
        break;
    case 'B':
        if (chb->board[x1 - 'a'][y1 - 1].type != bishop)
        {
            printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m", input_string[0]);
            for (int i = 2 + shift; i < 5; ++i)
            {
                putchar(input_string[i]);
            }
            puts(" ожидалось 'B'");
            exit_failure(chb);
        }
        break;
    case 'N':
        if (chb->board[x1 - 'a'][y1 - 1].type != knight)
        {
            printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m", input_string[0]);
            for (int i = 2 + shift; i < 5; ++i)
            {
                putchar(input_string[i]);
            }
            puts(" ожидалось 'N'");
            exit_failure(chb);
        }
        break;
    default:
        if (chb->board[x1 - 'a'][y1 - 1].type != pawn)
        {
            printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m", input_string[0]);
            for (int i = 2 + shift; i < 5; ++i)
            {
                putchar(input_string[i]);
            }
            puts(" ожидалось ''");
            exit_failure(chb);
        }
        break;
    }

    if (chb->board[x1 - 'a'][y1 - 1].color != chb->player)
    {
        printf("\x1b[31mОшибка:\x1b[0m чужая фигура: \x1b[31m%c%c\x1b[0m", x1, y1 + '0');
        for (int i = 2 + shift; i < 5; ++i)
        {
            putchar(input_string[i]);
        }
        exit_failure(chb);
    }

    chb->cur_x1 = x1 - 'a';
    chb->cur_y1 = y1 - 1;

    if (chb->is_test)
        print_move(chb->cur_x1, chb->cur_y1);

    create_correct_moves_matrix(chb, chb->cur_x1, chb->cur_y1);
    show_board(chb);
    printf("%d. ", chb->move_counter);

    if (chb->is_test)
    {
        switch (input_string[0])
        {
        case 'K':
        case 'Q':
        case 'R':
        case 'B':
        case 'N':
            putchar(input_string[0]);
            break;
        default:
            break;
        }
    }
    print_move(chb->cur_x1, chb->cur_y1);

    while ((ch = getchar()) != EOF && !isprint(ch))
        ;
    if (ch == EOF)
    {
        printf("\x1b[31mОшибка считывания\x1b[0m\n");
        exit_failure(chb);
    }

    ungetc(ch, stdin);

    return 0;
}

void move_figure(CHP **board, int x1, int y1, int x2, int y2)
{
    board[x2][y2].color = board[x1][y1].color;
    board[x2][y2].type = board[x1][y1].type;
    board[x1][y1].color = empty;
    board[x1][y1].type = empty;
}

static void check_enpassant(ChessBoard *chb)
{
    char input_string[5] = {0}, ch;

    for (int i = 0; i < 4; ++i)
    {
        if (((ch = getchar()) == EOF) || !isprint(ch))
        {
            break;
        }
        input_string[i] = ch;
    }

    if (ch == EOF)
    {
        printf("\x1b[31mОшибка считывания\x1b[0m\n");
        exit_failure(chb);
    }
    else if (strcmp(input_string, "e.p.") != 0)
    {
        printf("\x1b[31mОшибка считывания\x1b[0m ожидалось 'e.p.'\n");
        exit_failure(chb);
    }

    if (chb->is_test)
        printf("e.p.");
}

static void promote_pawn(ChessBoard *chb)
{
    char ch;
    if ((ch = getchar()) == EOF)
    {
        printf("\x1b[31mОшибка ввода:\x1b[0m ожидалось \"e.p.\"\n");
        exit_failure(chb);
    }

    switch (ch)
    {
    case 'Q':
        chb->board[chb->cur_x1][chb->cur_y1].type = queen;
        break;
    case 'R':
        chb->board[chb->cur_x1][chb->cur_y1].type = rook;
        break;
    case 'B':
        chb->board[chb->cur_x1][chb->cur_y1].type = bishop;
        break;
    case 'N':
        chb->board[chb->cur_x1][chb->cur_y1].type = knight;
        break;
    default:
        printf("\x1b[31mОшибка:\x1b[0m некорректный тип фигуры: \x1b[31m%c\x1b[0m ожидалось '' | 'K' | 'Q' | 'R' | 'B' | 'N'", ch);
        exit_failure(chb);
    }

    if (chb->is_test)
    {
        putchar(ch);
    }
}

int isanymoves(ChessBoard *chb, int color)
{
    for (int x = 0, y; x < BOARD_SIZE; ++x)
    {
        for (y = 0; y < BOARD_SIZE; ++y)
        {
            if (chb->board[x][y].color == color)
            {
                create_correct_moves_matrix(chb, x, y);
                for (int x2 = 0, y2; x2 < BOARD_SIZE; ++x2)
                {
                    for (y2 = 0; y2 < BOARD_SIZE; ++y2)
                    {
                        if (chb->moves_matrix[x2][y2] != 0)
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int isgameover(ChessBoard *chb)
{
    if (!isanymoves(chb, white) || !isanymoves(chb, black))
    {
        return 1;
    }

    return 0;
}

int select_target(ChessBoard *chb)
{
    int move_type = 1, y2;
    char ch, x2;

    if ((ch = getchar()) == EOF)
    {
        printf("\x1b[31mОшибка ввода:\x1b[0m\n");
        exit_failure(chb);
    }
    if (ch == 'q')
    {
        return -1;
    }
    if ((ch != '-') && (ch != 'x'))
    {
        printf("\x1b[31mОшибка:\x1b[0m некорректный тип хода: \x1b[31m%c\x1b[0m ожидалось '-' | 'x'", ch);
        exit_failure(chb);
    }
    else if (ch == 'x')
    {
        move_type = 2;
    }

    if (chb->is_test)
        printf("%c", ch);

    if ((ch = scanf("%c%d", &x2, &y2)) == EOF)
    {
        printf("\x1b[31mОшибка ввода\x1b[0m\n");
        exit_failure(chb);
    }
    y2--;
    x2 -= 'a';

    if (chb->moves_matrix[(int)x2][y2] != move_type)
    {
        if (chb->moves_matrix[(int)x2][y2] == 0)
        {
            printf("\x1b[31mОшибка:\x1b[0m недопустимый ход\n");
            exit_failure(chb);
        }
        else if (move_type == 1)
        {
            printf("\x1b[31mОшибка:\x1b[0m ожидалось взятие\n");
            exit_failure(chb);
        }
        else
        {
            printf("\x1b[31mОшибка:\x1b[0m ожидался тихий ход\n");
            exit_failure(chb);
        }
    }

    if (chb->is_test)
        print_move(x2, y2);

    switch (chb->board[chb->cur_x1][chb->cur_y1].type)
    {
    case king:
        if (chb->board[chb->cur_x1][chb->cur_y1].color == white)
        {
            chb->white_king_x = x2;
            chb->white_king_y = y2;
            if (chb->can_white_long_castle)
                chb->can_white_long_castle = 0;
            if (chb->can_white_short_castle)
                chb->can_white_short_castle = 0;
        }
        break;
    case rook:
        if (chb->board[chb->cur_x1][chb->cur_y1].color == white)
        {
            if (chb->can_white_long_castle && (chb->cur_x1 == 0) && (chb->cur_y1 == 0))
                chb->can_white_long_castle = 0;
            if (chb->can_white_short_castle && (chb->cur_x1 == 7) && (chb->cur_y1 == 0))
                chb->can_white_short_castle = 0;
        }
        else
        {
            if (chb->can_black_long_castle && (chb->cur_x1 == 0) && (chb->cur_y1 == 7))
                chb->can_black_long_castle = 0;
            if (chb->can_black_short_castle && (chb->cur_x1 == 7) && (chb->cur_y1 == 7))
                chb->can_black_short_castle = 0;
        }
        break;
    case pawn:
        if (chb->is_enpassant && (x2 == chb->enpas_x))
        {
            if (chb->board[chb->cur_x1][chb->cur_y1].color == white)
            {
                if ((y2 - 1) == chb->enpas_y)
                {
                    check_enpassant(chb);
                    chb->board[(int)x2][y2 - 1].type = empty;
                    chb->board[(int)x2][y2 - 1].color = empty;
                }
            }
            else if ((y2 + 1) == chb->enpas_y)
            {
                check_enpassant(chb);
                chb->board[(int)x2][y2 + 1].type = empty;
                chb->board[(int)x2][y2 + 1].color = empty;
            }
        }
        else if (chb->board[chb->cur_x1][chb->cur_y1].color == white)
        {
            if (chb->cur_y1 == 6)
            {
                promote_pawn(chb);
            }
        }
        else if (chb->cur_y1 == 1)
        {
            promote_pawn(chb);
        }
        break;
    default:
        break;
    }

    if ((chb->board[chb->cur_x1][chb->cur_y1].type == pawn) && (abs(chb->cur_y1 - y2) == 2))
    {
        chb->is_enpassant = 1;
        chb->enpas_x = x2;
        chb->enpas_y = y2;
    }
    else
    {
        chb->is_enpassant = 0;
    }

    if (!iskingsafe(chb, chb->cur_x1, chb->cur_y1, !chb->board[chb->cur_x1][chb->cur_y1].color, x2, y2))
    {
        move_figure(chb->board, chb->cur_x1, chb->cur_y1, x2, y2);
        if (isgameover(chb))
        {
            if (((ch = getchar()) == EOF) || (ch != '#'))
            {
                printf("\x1b[31mОшибка ввода:\x1b[0m ожидалось '#'\n");
                exit_failure(chb);
            }
            if (chb->is_test)
                putchar('#');
            reset_moves_matrix(chb->moves_matrix);
            show_board(chb);
            return chb->board[chb->cur_x1][chb->cur_y1].color ? 2 : 3;
        }
        else if (((ch = getchar()) == EOF) || (ch != '+'))
        {
            printf("\x1b[31mОшибка ввода:\x1b[0m ожидалось '+'\n");
            exit_failure(chb);
        }
        if (chb->is_test)
            putchar('+');
    }
    else
    {
        move_figure(chb->board, chb->cur_x1, chb->cur_y1, x2, y2);
    }

    if (isgameover(chb))
    {
        reset_moves_matrix(chb->moves_matrix);
        show_board(chb);
        return 1;
    }

    return 0;
}

static void destroy_board(CHP **board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        free(board[i]);
    free(board);
    board = NULL;
}

static void destroy_moves_matrix(int **moves_matrix)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        free(moves_matrix[i]);
    free(moves_matrix);
    moves_matrix = NULL;
}

void destroy_chessboard(ChessBoard *chb)
{
    destroy_board(chb->board);
    chb->board = NULL;
    destroy_moves_matrix(chb->moves_matrix);
    chb->moves_matrix = NULL;
    destroy_moves_matrix(chb->enemy_moves_matrix);
    chb->enemy_moves_matrix = NULL;
    free(chb);
    chb = NULL;
}
