#pragma once

class WorldMapWeatherHandler
{
public:
	static void InstallHooks();

private:
	WorldMapWeatherHandler() = default;

	static RE::TESWeather* GetUniqueWeather(const std::string& a_fileName);

	static void SetWeather(RE::Sky* a_this, RE::TESWeather* weather, bool arg3);

	static inline REL::Relocation<decltype(SetWeather)> _SetWeather;
};
