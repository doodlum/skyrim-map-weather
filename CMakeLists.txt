cmake_minimum_required(VERSION 3.20)

project(
	UniqueMapWeatherFramework
	VERSION 1.0.0
	LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(XSEPlugin)

find_package(jsoncpp REQUIRED)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		${CommonLibName}::${CommonLibName}
		spdlog::spdlog
		jsoncpp_static
)