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
