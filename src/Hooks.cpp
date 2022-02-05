#include "Hooks.h"
#include "WorldMapWeatherHandler.h"

namespace Hooks
{
	void Hooks::Install()
	{
		WorldMapWeatherHandler::InstallHooks();
		logger::info("Installed all hooks");
	}
}
