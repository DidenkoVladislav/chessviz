#include "chess_funcs.h"

int main(int argc, char const *argv[])
{
    setlocale(LC_CTYPE, "");
    char ch = 0, code = 0;

    ChessBoard *chb = (ChessBoard *)malloc(sizeof(ChessBoard));
    if (chb == NULL)
    {
        printf("\x1b[31mОшибка выделения памяти.\x1b[0m\n");
        abort();
    }
    chessboard_constructor(chb);

    if ((argc == 2))
    {
        if (strcmp(argv[1], "--test") != 0)
        {
            printf("\x1b[31mОшибка:\x1b[0m неправильный аргумент. Ожидалось \"--test\"\n");
            abort();
        }
        chb->is_test = 1;
    }
    else if (argc > 2)
    {
        printf("\x1b[31mОшибка:\x1b[0m неправильное число аргументов. Ожидалось 0-1.\n");
        abort();
    }

    show_board(chb);
    do
    {
        printf("%d. ", chb->move_counter);
        code = select_figure(chb);
        if (code == -2)
        {
            if (chb->is_test)
                printf("O-O\n");
            make_short_castle(chb);
            if (chb->player == black)
                chb->move_counter++;
            chb->player = !chb->player;
            reset_moves_matrix(chb->moves_matrix);
            show_board(chb);
            ungetc('\n', stdin);
            continue;
        }
        else if (code == -3)
        {
            if (chb->is_test)
                printf("O-O-O\n");
            make_long_castle(chb);
            if (chb->player == black)
                chb->move_counter++;
            chb->player = !chb->player;
            reset_moves_matrix(chb->moves_matrix);
            show_board(chb);
            ungetc('\n', stdin);
            continue;
        }
        else if (code != 0)
        {
            break;
        }
        code = select_target(chb);
        if (code == 1)
        {
            printf("\nНичья\n");
            break;
        }
        else if (code == 2)
        {
            printf("\nПобеда белых\n");
            break;
        }
        else if (code == 3)
        {
            printf("\nПобеда чёрных\n");
            break;
        }
        reset_moves_matrix(chb->moves_matrix);
        show_board(chb);
        if (chb->player == black)
            chb->move_counter++;
        chb->player = !chb->player;
    } while ((ch = getchar()) != 'q' && (ch != EOF));

    destroy_chessboard(chb);

    return 0;
}
