all:
	gcc moves_matrix.c chess_funcs.c main.c -Wall -Werror -Wextra -o chessviz
run:
	./chessviz
run2:
	./chessviz --test <input.txt
run3:
	./chessviz --test <stalemate.txt
run4:
	./chessviz --test <input.txt >result.txt
run5:
	./chessviz --test <stalemate.txt >result.txt
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./chessviz
valgrind2:
	valgrind --leak-check=full --show-leak-kinds=all ./chessviz --test <input.txt
valgrind3:
	valgrind --leak-check=full --show-leak-kinds=all ./chessviz --test <stalemate.txt