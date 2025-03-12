#include "tools.hpp"

bool	timesUp(uint64_t timeMS)
{
	static int64_t	maxTimeMS;
	auto			now = std::chrono::high_resolution_clock::now().time_since_epoch();
	int64_t			nowMS = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();

	if (timeMS != 0)
	{
		maxTimeMS = timeMS + nowMS;
		return false;
	}
	return nowMS >= maxTimeMS;
}
