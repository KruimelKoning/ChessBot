#include "perft.hpp"
#include "uci.hpp"

#include <stdlib.h>

#define PERFT 0

int main(void) {
#if PERFT
	perft_run();
#else
	uci_run("HeatmapV1", "lbartels akuijer nleusden");
#endif

	return EXIT_SUCCESS;
}
