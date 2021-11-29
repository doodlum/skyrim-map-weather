#pragma once

namespace Helper
{
	bool GetCurrentWeather(uint32_t& currentWeatherFormID);

	bool GetOutgoingWeather(uint32_t& outgoingWeatherFormID);

	bool GetWeatherTransition(float& weatherTransition);

	bool GetSkyMode(uint32_t& skyMode);

	bool GetTime(float& time);

	bool GetCurrentLocationID(uint32_t& locationFormID);

	bool GetWorldSpaceID(uint32_t& worldSpaceFormID);

	// Papyrus Weather.GetClassification
	int32_t GetClassification(RE::TESWeather* weather);

	bool GetCurrentWeatherClassification(int32_t& classification);

	bool GetOutgoingWeatherClassification(int32_t& classification);
}




