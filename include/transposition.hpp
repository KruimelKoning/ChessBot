#pragma once

#include "position.hpp"

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

struct Evaluation
{
	Flag	flag;
	int		value;
};

static std::unordered_map<uint64_t, int32_t>	transpositionTable;
static std::unordered_map<uint64_t, int32_t>	repetitionCheck;

void		initRandTable();
uint64_t	hash(const Position& pos);
