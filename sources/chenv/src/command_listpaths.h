#pragma once
#include "envvar.h"
#include "pathentry.h"
#include <string>

std::vector<PathEntry> command_listpaths(std::wstring command);
