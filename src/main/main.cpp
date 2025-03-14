#include "perft.hpp"
#include "uci.hpp"
#include "transposition.hpp"

#include <stdlib.h>

#define PERFT 0

int main(void)
{
	initRandTable();
#if PERFT
	perft_run();
#else
	uci_run("MoveOrderingV3.1", "lbartels akuijer nleusden");
#endif

	return EXIT_SUCCESS;
}
