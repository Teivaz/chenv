#include "command_save.h"
#include "commands.h"
#include "printer.h"

#include "command_path.h"

namespace {
	
}

void command_save() {
	auto userPath = command_path(EnvVar::Origin::RegSystem);
	// Read registry entries

	// Start transaction

	// Save registry entries

	// End transaction
	return;
}

namespace {
	void _command_save(Command::Args& args) {
		command_save();
		P.result(L"saved");
	}

	Command save = {
		L"save",
		L"Saves the current environment.",
		_command_save
	};
}
