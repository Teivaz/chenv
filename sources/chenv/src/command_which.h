#pragma once
#include "envvar.h"
#include "pathentry.h"
#include <string>

PathEntry command_which(std::wstring command, EnvVar::Origin env = EnvVar::Origin::Local);
