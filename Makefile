all:
	gcc moves_matrix.c chess_funcs.c main.c -Wall -Werror -Wextra -o chessviz
run:
	./chessviz
fastcheck:
	./chessviz --test <fastcheckmate.txt >result.txt
stalemate:
	./chessviz --test <stalemate.txt >result.txt
enpassant:
	./chessviz --test <enpassant.txt >result.txt
shortcastle:
	./chessviz --test <shortcastle.txt >result.txt
longcastle:
	./chessviz --test <longcastle.txt >result.txt
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./chessviz
valgrind2:
	valgrind --leak-check=full --show-leak-kinds=all ./chessviz --test <stalemate.txt