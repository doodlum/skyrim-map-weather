#include <json/json.h>

#include "FormUtil.h"
#include "WorldMapWeatherHandler.h"

void WorldMapWeatherHandler::InstallHooks()
{
	REL::Relocation<std::uintptr_t> hook{ REL::RelocationID(52256, 53146).address() + REL::Relocate(0xE4, 0x10F) };
	auto& trampoline = SKSE::GetTrampoline();
	_SetWeather = trampoline.write_call<5>(hook.address(), SetWeather);
}

RE::BSResourceNiBinaryStream& operator>>(RE::BSResourceNiBinaryStream& a_sin, Json::Value& a_root)
{
	Json::CharReaderBuilder fact;
	std::unique_ptr<Json::CharReader> const reader{ fact.newCharReader() };

	auto size = a_sin.stream->totalSize;
	auto buffer = std::make_unique<char[]>(size);
	a_sin.read(buffer.get(), size);

	auto begin = buffer.get();
	auto end = begin + size;

	std::string errs;
	bool ok = reader->parse(begin, end, std::addressof(a_root), std::addressof(errs));

	if (!ok) {
		throw std::runtime_error{ errs };
	}

	return a_sin;
}

RE::TESWeather* WorldMapWeatherHandler::GetUniqueWeather(const std::string& worldspaceID)
{
	auto fileName = std::filesystem::path{ worldspaceID };
	fileName.replace_extension("json");
	fileName = std::filesystem::path{ "MapWeathers" } / fileName;
	RE::BSResourceNiBinaryStream a_fileStream{ fileName.string() };

	if (!a_fileStream.good()) {
		return nullptr;
	}

	Json::Value root;
	a_fileStream >> root;

	logger::info("Reading file {}", fileName.string());

	Json::Value weathers = root["weathers"];
	if (weathers.isArray()) {
		for (auto& weather : weathers) {
			if (!weather.isObject()) {
				logger::warn("Failed to fetch weather from {}", fileName.string());
				continue;
			}

			auto formID = weather["formID"].asString();

			if (formID.empty()) {
				logger::warn("Weather missing form ID in {}", fileName.string());
				continue;
			}

			auto weatherRef = skyrim_cast<RE::TESWeather*>(FormUtil::GetFormFromIdentifier(formID));

			if (!weatherRef) {
				logger::warn("'{}' did not correspond to a weather reference in {}", formID, fileName.string());
				continue;
			}

			return weatherRef;
		}
	}
	return nullptr;
}

RE::TESWorldSpace* WorldspaceOnMap()
{
	const auto mapMenu = RE::UI::GetSingleton()->GetMenu<RE::MapMenu>();
	return mapMenu->GetRuntimeData2().worldSpace;
}

void WorldMapWeatherHandler::SetWeather(RE::Sky* a_this, RE::TESWeather* weather, bool arg3)
{
	const auto uniqueWeather = GetUniqueWeather(WorldspaceOnMap()->GetFormEditorID());

	if (uniqueWeather)
		_SetWeather(a_this, uniqueWeather, arg3);
	else
		_SetWeather(a_this, weather, arg3);
}
