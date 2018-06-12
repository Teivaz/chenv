#pragma once
#include "envvar.h"
#include <string>

enum class RegKey {
	CLASSES_ROOT,
	CURRENT_CONFIG,
	CURRENT_USER,
	LOCAL_MACHINE,
	USERS,
};

EnvVar command_getreg(RegKey key, std::wstring path, std::wstring value, bool expand = false);
