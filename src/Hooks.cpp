#include "Hooks.h"
#include "WorldMapHandler.h"

void Hooks::Install()
{
	Hooks::InstallWorldMapWeatherHook();
}

void Hooks::InstallWorldMapWeatherHook()
{
	REL::Relocation<std::uintptr_t> hook{REL::ID(52256), 0xE4};

	auto& trampoline = SKSE::GetTrampoline();
	trampoline.write_call<5>(hook.address(), WorldMapHandler::ForceWeather);
}

