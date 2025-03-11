#include "perft.hpp"
#include "generate.hpp"
#include "position.hpp"
#include "types.hpp"

#include <stddef.h>

struct perft_data {
	const char *fen;
	int depth;
	unsigned long nodes;
};

/* https://www.chessprogramming.org/Perft_Results                            */
static const struct perft_data perft_data[] = {
	{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6, 119060324 },
	{ "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 5, 193690690 },
	{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 7, 178633661 },
	{ "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6, 706045033 },
	{ "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 6, 706045033 },
	{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5, 89941194 },
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 5, 164075551 },
};

static unsigned long perft(const Position *pos, int depth)
{
	std::vector<Move> moves;

	moves.reserve(EXPECTED_MAX_MOVES);
	size_t count = generate_legal_moves(pos, moves);

	if (depth == 0) {
		return 1;
	} else if (depth == 1) {
		return count;
	} else {
		size_t index;
		unsigned long result = 0;

		for (index = 0; index < count; index++) {
			Position copy = *pos;

			do_move(&copy, moves[index]);

			result += perft(&copy, depth - 1);
		}

		return result;
	}
}

void perft_run(void) {
	int count = sizeof perft_data / sizeof *perft_data;
	int index;
	int passed = 0;

	for (index = 0; index < count; index++) {
		struct perft_data data = perft_data[index];
		Position pos;
		unsigned long nodes;

		if (parse_position(&pos, data.fen) != SUCCESS) {
			fprintf(stderr, "test %02d, parse error\n", index);

			continue;
		}

		nodes = perft(&pos, data.depth);

		if (nodes != data.nodes) {
			fprintf(stderr, "test %02d, %lu nodes, expected %lu\n", index, nodes, data.nodes);
		} else {
			fprintf(stderr, "test %02d, %lu nodes\n", index, nodes);

			passed++;
		}
	}

	fprintf(stderr, "%d/%d tests passed\n", passed, count);
}
