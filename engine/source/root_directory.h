/*
	This field was automatically created with CMake please don't modify it
*/
#pragma once

#include <filesystem>

namespace UWU
{
	
static const char * const source_directory = "D:/Users/Gabriel/Desktop/semestre9/motoresVideojuegos/uwu-engine/engine/source/";

static std::filesystem::path getPath(const std::string &relative_path){
	return source_directory + relative_path;
}
	
} // UWU
