#pragma once
#include "envvar.h"
#include <string>

EnvVar command_get(std::wstring name, EnvVar::Origin env = EnvVar::Origin::Local, bool expand = false);
