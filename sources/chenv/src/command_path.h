// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once
#include "envvar.h"
#include <string>

EnvVar command_pathext(EnvVar::Origin type = EnvVar::Origin::Local);
EnvVar command_path(EnvVar::Origin type = EnvVar::Origin::Local);
