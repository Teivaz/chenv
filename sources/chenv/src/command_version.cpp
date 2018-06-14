// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#include "command_version.h"
#include "commands.h"
#include "printer.h"

std::wstring command_version() {
	return { L"0.1" };
}

namespace {
	void _command_version(Command::Args& args) {
		P.result(command_version());
	}

	Command version = {
		L"version",
		L"Prints the current version.",
		_command_version
	};
}
