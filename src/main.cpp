
#ifndef NDEBUG
	#define _AMD64_
	#include <debugapi.h>
#endif

#include "Hooks.h"

void InitLogger()
{
	static bool initialized = false;
	if (!initialized) {
		initialized = true;
	} else {
		return;
	}

#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return;
	}

	*path /= fmt::format("{}.log"sv, Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

	logger::info("{} v{}", Version::PROJECT, Version::NAME);
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent())
#endif
	InitLogger();

	logger::info("{} loaded"sv, Version::PROJECT);

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(14);

	Hooks::Install();

	return true;
}


extern "C" DLLEXPORT constinit auto SKSEPlugin_Version =
	[]() {
		SKSE::PluginVersionData v{};
		v.pluginVersion = Version::MAJOR;
		v.PluginName(Version::PROJECT);
		v.AuthorName("doodlez"sv);
		v.UsesAddressLibrary(true);
		return v;
	}();

#ifdef SKYRIMVR
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	InitLogger();

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver != SKSE::RUNTIME_VR_1_4_15_1) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"sv), ver.string());
		return false;
	}

	return true;
}
#else
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	InitLogger();

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver != SKSE::RUNTIME_1_5_97) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"sv), ver.string());
		return false;
	}

	return true;
}
#endif

