#include "command_get.h"
#include "envvar.h"
#include "commands.h"
#include "exceptions.h"
#include "printer.h"

#include <Windows.h>

#include <algorithm>

EnvVar command_get(std::wstring name, EnvVar::Origin env, bool expand) {
	auto bufferSize = GetEnvironmentVariable(name.c_str(), nullptr, 0);
	std::vector<wchar_t> buffer;
	if (bufferSize == 0) {
		return {};
	}
	buffer.resize(bufferSize);
	auto written = GetEnvironmentVariable(name.c_str(), buffer.data(), bufferSize);
	if (written == 0) {
		// TODO: Throw last error
	}
	if (written > bufferSize) {
		// TODO: Env var was changed. Try and read again maybe?
	}
	return EnvVar(name, { buffer.data() });
}

namespace {
	struct Options {
		std::wstring variable;
	};

	auto usageString =
		L"chenv get <variable>";

	Options handle_args(Command::Args& args) {
		if (args.size() > 1) {
			throw ChenvExceptionUsage(usageString);
		}
		return { args.front() };
	}

	void _command_get(Command::Args& args) {
		auto opts = handle_args(args);
		auto var = command_get(opts.variable);
		if (var) {
			P.result(var.toString());
		}
		else {
			P.result(L"entry not found");
		}
	}

	Command get = {
		L"get",
		L"Prints the environment variable.",
		_command_get
	};
}
