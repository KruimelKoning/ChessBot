#include "uci.hpp"
#include "search.hpp"
#include "move.hpp"
#include "types.hpp"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *get_line(FILE *stream) {
	size_t capacity = 1024;
	size_t size = 0;
	char *string = (char*)malloc(capacity);

	while (fgets(string + size, capacity - size, stream)) {
		size += strlen(string + size);

		if (string[size - 1] == '\n') {
			return string;
		}

		capacity *= 2;
		string = (char*)realloc(string, capacity);
	}

	free(string);

	return NULL;
}

static char *get_token(char *string, char *store) {
	string += strlen(string);
	*string = *store;

	while (isspace(*string)) {
		string++;
	}

	if (*string) {
		char *token = string;

		while (*string && !isspace(*string)) {
			string++;
		}

		*store = *string;
		*string = '\0';

		return token;
	}

	return NULL;
}

static void uci_position(Position *pos, char *token, char *store) {
	token = get_token(token, store);

	if (token && !strcmp(token, "startpos")) {
		parse_position(pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		token = get_token(token, store);
	} else if (token && !strcmp(token, "fen")) {
		char *fen = get_token(token, store);
		int index;

		token = fen;

		for (index = 0; token && index < 5; index++) {
			token = get_token(token, store);
		}

		if (token) {
			parse_position(pos, fen);
			token = get_token(token, store);
		}
	}

	if (token && !strcmp(token, "moves")) {
		while ((token = get_token(token, store))) {
			Move move;

			if (parse_move(&move, token) == SUCCESS) {
				do_move(pos, move);
			}
		}
	}
}

std::string move_to_string(Move move)
{
	std::string buffer(6, '\0');

	buffer[0] = "abcdefgh"[FILE(move.from_square)];
	buffer[1] = '1' + RANK(move.from_square);
	buffer[2] = "abcdefgh"[FILE(move.to_square)];
	buffer[3] = '1' + RANK(move.to_square);

	if (move.promotion_type != NO_TYPE) {
		buffer[4] = "pnbrqk"[move.promotion_type];
	}
	return buffer;
}

static void uci_go(const Position *pos, char *token, char *store) {
	SearchInfo info;
	Move move;

	info.pos = pos;
	info.time[WHITE] = 0;
	info.time[BLACK] = 0;
	info.increment[WHITE] = 0;
	info.increment[BLACK] = 0;

	while ((token = get_token(token, store))) {
		if (!strcmp(token, "searchmoves")) {
			break;
		} else if (!strcmp(token, "ponder")) {
			continue;
		} else if (!strcmp(token, "infinite")) {
			continue;
		} else if (!strcmp(token, "wtime")) {
			token = get_token(token, store);
			info.time[WHITE] = token ? atoi(token) : 0;
		} else if (!strcmp(token, "btime")) {
			token = get_token(token, store);
			info.time[BLACK] = token ? atoi(token) : 0;
		} else if (!strcmp(token, "winc")) {
			token = get_token(token, store);
			info.increment[WHITE] = token ? atoi(token) : 0;
		} else if (!strcmp(token, "binc")) {
			token = get_token(token, store);
			info.increment[BLACK] = token ? atoi(token) : 0;
		} else {
			token = get_token(token, store);
		}

		if (!token) {
			break;
		}
	}
	move = search(info);
	std::cout << "bestmove " << move_to_string(move) << std::endl;
}

static bool	check_empty(Position *pos)
{
	for (size_t i = 0; i < 64; i++)
	{
		if (pos->board[i] == KING)
		{
			return (true);
		}
	}
	return (false);
}

void uci_run(const char *name, const char *author) {
	char *line;
	int quit = 0;
	Position pos;

	while (!quit && (line = get_line(stdin))) {
		char *token = line;
		char store = *token;

		*token = '\0';

		while ((token = get_token(token, &store))) {
			if (!strcmp(token, "exit") || !strcmp(token, "quit")) {
				quit = 1;
			} else if (!strcmp(token, "uci")) {
				printf("id name %s\n", name);
				printf("id author %s\n", author);
				printf("uciok\n");
			} else if (!strcmp(token, "isready")) {
				printf("readyok\n");
			} else if (!strcmp(token, "position")) {
				uci_position(&pos, token, &store);
			} else if (!strcmp(token, "go")) {
				uci_go(&pos, token, &store);
			} else if (!strcmp(token, "setoption")) {
				break;
			} else if (!strcmp(token, "register")) {
				break;
			} else if (!strcmp(token, "print") && check_empty(&pos)) {
				print_position(&pos, stdout);
			} else {
				continue;
			}

			break;
		}

		free(line);
		fflush(stdout);
	}
}
