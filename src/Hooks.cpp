#include "Hooks.h"

#include "WorldMapWeatherHandler.h"

namespace Hooks
{
	void Install()
	{
		WorldMapWeatherHandler::InstallHooks();
		logger::info("Installed all hooks");
	}
}
