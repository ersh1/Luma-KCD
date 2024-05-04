#include "RE.h"

#include "Offsets.h"

namespace RE
{
	CCryNameR::CCryNameR(const char* s)
	{
		using func_t = CCryNameR* (*)(CCryNameR*, const char* s);
		static func_t func = reinterpret_cast<func_t>(Offsets::baseAddress + 0x41C790);

		func(this, s);
	}
}
