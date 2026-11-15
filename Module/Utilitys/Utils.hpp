#pragma once

#include <lstate.h>
#include <Windows.h>
#include <RBX/update.hpp>

namespace Utils
{
	inline bool IsInGame(uintptr_t DataModel)
	{
		__try
		{
			return DataModel && *reinterpret_cast<int64_t*>(DataModel + ClientOffsets::DataModel::PlaceId) > 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}
}
