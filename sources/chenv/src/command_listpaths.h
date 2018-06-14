// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once
#include "envvar.h"
#include "pathentry.h"
#include <string>

std::vector<PathEntry> command_listpaths(std::wstring command);
