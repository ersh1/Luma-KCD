#include "DKUtil/Config.hpp"

#include "Hooks.h"
#include "Offsets.h"

using namespace DKUtil::Alias;

inline DKUtil::Hook::Trampoline::Trampoline& AllocTrampoline(size_t a_size)
{
	using namespace DKUtil::Hook;
	auto& trampoline = Trampoline::GetTrampoline();
	if (!trampoline.capacity()) {
		trampoline.release();

		const auto textx = Module::get("WHGame.dll"sv).section(Module::Section::textx);
		uintptr_t from = textx.first + textx.second;

		trampoline.PageAlloc(a_size, from);
	}
	return trampoline;
}

BOOL APIENTRY DllMain(HMODULE a_hModule, DWORD a_ul_reason_for_call, LPVOID a_lpReserved)
{
	if (a_ul_reason_for_call == DLL_PROCESS_ATTACH) {
#ifndef NDEBUG
		while (!IsDebuggerPresent()) {
			Sleep(100);
		}
#endif
		dku::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
		//INFO("game type : {}", dku::Hook::GetProcessName());

		// do stuff
		// this allocates 1024 bytes for development builds, you can
		// adjust the value accordingly with the log result for release builds
		//dku::Hook::Trampoline::AllocTrampoline(1 << 10);
		AllocTrampoline(1 << 10);

		Offsets::Init();
		Hooks::Install();
	}

	return TRUE;
}