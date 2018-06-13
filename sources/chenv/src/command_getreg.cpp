#include "command_getreg.h"
#include "envvar.h"
#include "commands.h"
#include "printer.h"
#include "exceptions.h"

#include <algorithm>
#include <map>
#include <Windows.h>

namespace {
	void regInfo(Reg::Key key, std::wstring path) {
		Reg::KeyHandle handle{ key, path, KEY_READ };
		DWORD subKeys, maxSubKeyLen, values, maxValueNameLen;

		auto result = RegQueryInfoKey(handle, nullptr, nullptr, nullptr, &subKeys, &maxSubKeyLen, nullptr, &values, &maxValueNameLen, nullptr, nullptr, nullptr);
		if (result != ERROR_SUCCESS) {
			// TODO: Throw something
		}
	}
}

EnvVar command_getreg(Reg::Key key, std::wstring path, std::wstring valueName, bool expand) {
	Reg::KeyHandle handle{ key, path, KEY_READ };
	auto value = handle.getValue(valueName, expand);
	return EnvVar(valueName, value);
}

namespace {
	struct Options {
		Reg::Key key;
		std::wstring path;
		std::wstring value;
		bool expand;
	};

	auto usageString =
		L"chenv getreg <HKCR|HKCU|HKLM|HKU|HKCC> <path> <value> [--expand]";

	Options handle_args(Command::Args& args) {
		if (args.size() != 3 || args.size() != 4) {
			throw ChenvExceptionUsage(usageString);
		}
		auto it = args.begin();
		Options o;
		auto key = Lower(*it);
		if (key == L"hkcr") {
			o.key = Reg::Key::CLASSES_ROOT;
		}
		else if (key == L"hkcu") {
			o.key = Reg::Key::CURRENT_USER;
		}
		else if (key == L"hklm") {
			o.key = Reg::Key::LOCAL_MACHINE;
		}
		else if (key == L"hku") {
			o.key = Reg::Key::USERS;
		}
		else if (key == L"hkcc") {
			o.key = Reg::Key::CURRENT_CONFIG;
		}
		else {
			throw ChenvExceptionUsage(usageString);
		}

		o.path = *++it;
		o.value = *++it;
		o.expand = false;

		if (it != args.end()) {
			if (*it == L"--expand") {
				o.expand = true;
			}
			else {
				throw ChenvExceptionUsage(usageString);
			}
		}

		return o;
	}

	void _command_getreg(Command::Args& args) {
		auto opts = handle_args(args);
		auto var = command_getreg(opts.key, opts.path, opts.value, opts.expand);
		if (var) {
			P.result(var.toString());
		}
		else {
			P.result(L"entry not found");
		}
	}

	Command get = {
		L"getreg",
		L"Prints the registry entry info.",
		_command_getreg
	};
}
