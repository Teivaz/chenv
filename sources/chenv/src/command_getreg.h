#pragma once
#include "envvar.h"
#include "registry.h"
#include <string>

EnvVar command_getreg(Reg::Key key, std::wstring path, std::wstring valueName, bool expand = false);
