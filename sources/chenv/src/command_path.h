#pragma once
#include "envvar.h"
#include <string>

EnvVar command_pathext(EnvVar::Origin type = EnvVar::Origin::Local);
EnvVar command_path(EnvVar::Origin type = EnvVar::Origin::Local);
