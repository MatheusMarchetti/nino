#pragma once

#include "Core/Random.h"

namespace nino
{
	class UUID
	{
	public:
		uint64_t GenerateUUID(size_t initialSeed = 5489U)
		{
			Random rng(initialSeed);

			return rng.Generate();
		}
	};
}