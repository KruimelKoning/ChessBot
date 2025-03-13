#pragma once

#include "position.hpp"
#include "move.hpp"

#include <bit>
#include <random>
#include <stdint.h>
#include <unordered_map>

enum struct Flag : int
{
	LOWER = -1,
	EXACT,
	UPPER
};

struct	Evaluation
{
	int		depth;
	int		score;
	Move	bestMove;
};

static std::unordered_map<uint64_t, Evaluation>	transpositionTable;

void		initRandTable();
uint64_t	hash(const Position& pos);
