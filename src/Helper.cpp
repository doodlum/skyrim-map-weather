#include "RE/CommandTable.h"
#include "RE/ConsoleManager.h"
#include "RE/PlayerCamera.h"
#include "RE/PlayerCharacter.h"
#include "RE/Sky.h"
#include "RE/TESObjectCELL.h"
#include "RE/TESWeather.h"
#include "RE/TESWorldSpace.h"


bool Helper::GetCurrentWeather(uint32_t& currentWeatherFormID)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->currentWeather) {
		currentWeatherFormID = skyPtr->currentWeather->formID;
		return true;
	}
	return false;
}

bool Helper::GetOutgoingWeather(uint32_t& outgoingWeatherFormID)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->outgoingWeather) {
		outgoingWeatherFormID = skyPtr->outgoingWeather->formID;
		return true;
	}
	return false;
}

bool Helper::GetWeatherTransition(float& weatherTransition)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr) {
		weatherTransition = skyPtr->transition;
		return true;
	}
	return false;
}

bool Helper::GetSkyMode(uint32_t& skyMode)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr) {
		skyMode = skyPtr->skyMode;
		return true;
	}
	return false;
}

bool Helper::GetTime(float& time)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr) {
		time = skyPtr->timeOfDay;
		return true;
	}
	return false;
}

bool Helper::GetCurrentLocationID(uint32_t& locationFormID)
{
	const auto playerPtr = RE::PlayerCharacter::GetSingleton();
	if (playerPtr && playerPtr->currentLocation) {
		locationFormID = reinterpret_cast<BGSLocation*>(playerPtr->currentLocation)->formID;
		return true;
	}
	return false;
}

bool Helper::GetWorldSpaceID(uint32_t& worldSpaceFormID)
{
	const auto playerPtr = RE::PlayerCharacter::GetSingleton();
	if (playerPtr && playerPtr->parentCell && (playerPtr->parentCell->flags & RE::TESObjectCELL::Flag::kIsInteriorCell) != RE::TESObjectCELL::Flag::kNone) {
		worldSpaceFormID = 0x0;
		return true;
	}
	if (playerPtr && playerPtr->currentWorldSpace) {
		worldSpaceFormID = playerPtr->currentWorldSpace->formID;
		return true;
	}
	return false;
}

// Papyrus Weather.GetClassification
int32_t Helper::GetClassification(RE::TESWeather* weather)
{
	typedef RE::TESWeather::Data::Flag Flags;

	const auto flags = weather->data.flags;

	if ((flags & Flags::kWeather_Pleasant) != Flags::kNone)
		return 0;
	if ((flags & Flags::kWeather_Cloudy) != Flags::kNone)
		return 1;
	if ((flags & Flags::kWeather_Rainy) != Flags::kNone)
		return 2;
	if ((flags & Flags::kWeather_Snowy) != Flags::kNone)
		return 3;

	return 0xFFFFFFFF;
}

bool Helper::GetCurrentWeatherClassification(int32_t& classification)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->currentWeather) {
		classification = GetClassification(skyPtr->currentWeather);
		return true;
	}
	return false;
}

bool Helper::GetOutgoingWeatherClassification(int32_t& classification)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->outgoingWeather) {
		classification = GetClassification(skyPtr->outgoingWeather);
		return true;
	}
	return false;
}
