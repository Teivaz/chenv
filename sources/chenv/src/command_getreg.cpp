#include "command_getreg.h"
#include "envvar.h"
#include "commands.h"
#include "printer.h"
#include "exceptions.h"

#include <Windows.h>

#include <algorithm>
#include <map>

namespace {
	std::map<RegKey, HKEY> kKey2Handle{
		{ RegKey::CLASSES_ROOT, HKEY_CLASSES_ROOT },
		{ RegKey::CURRENT_CONFIG, HKEY_CURRENT_CONFIG },
		{ RegKey::CURRENT_USER, HKEY_CURRENT_USER },
		{ RegKey::LOCAL_MACHINE, HKEY_LOCAL_MACHINE },
		{ RegKey::USERS, HKEY_USERS },
	};

	struct KeyHandle {
		KeyHandle(RegKey hKey, std::wstring lpSubKey, REGSAM samDesired) {
			auto result = RegOpenKeyEx(kKey2Handle[hKey], lpSubKey.c_str(), 0, samDesired, &mHandle);
			if (result != ERROR_SUCCESS) {
				// TODO: Throw exception.
				// Format message https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351(v=vs.85).aspx
			}
		}
		~KeyHandle() {
			if (mHandle) RegCloseKey(mHandle);
		}
		operator HKEY() const { return mHandle; }
		HKEY mHandle = 0;
		KeyHandle operator =(KeyHandle) = delete;
		KeyHandle(const KeyHandle&) = delete;
	};
}

void regInfo(RegKey key, std::wstring path) {
	KeyHandle handle{ key, path, KEY_READ };
	DWORD subKeys, maxSubKeyLen, values, maxValueNameLen;

	auto result = RegQueryInfoKey(handle, nullptr, nullptr, nullptr, &subKeys, &maxSubKeyLen, nullptr, &values, &maxValueNameLen, nullptr, nullptr, nullptr);
	if (result != ERROR_SUCCESS) {
		// TODO: Throw something
	}
}

EnvVar getreg(KeyHandle& handle, std::wstring value, bool expand = false) {
	DWORD valueType = 0;
	DWORD dataSize = 0;
	std::vector<wchar_t> buffer;
	DWORD allowedTypes = RRF_RT_REG_EXPAND_SZ | RRF_RT_REG_SZ | (RRF_NOEXPAND * !expand);

	auto result = RegGetValue(handle, nullptr, value.c_str(), allowedTypes, &valueType, nullptr, &dataSize);
	if (result != ERROR_SUCCESS) {
		if (result == ERROR_FILE_NOT_FOUND) {
			return {};
		}
		// TODO: Throw last error
	}
	buffer.resize(dataSize);
	result = RegGetValue(handle, nullptr, value.c_str(), allowedTypes, &valueType, buffer.data(), &dataSize);
	if (result != ERROR_SUCCESS) {
		// TODO: Throw last error
	}
	if (valueType == REG_SZ) {
		return EnvVar(EnvVar::Type::String, value, { buffer.data() });
	}
	if (valueType == REG_EXPAND_SZ) {
		return EnvVar(EnvVar::Type::ExpandableString, value, { buffer.data() });
	}
	return {};
}

EnvVar command_getreg(RegKey key, std::wstring path, std::wstring value, bool expand) {
	KeyHandle handle{ key, path, KEY_READ };
	return getreg(handle, std::move(value), expand);
}

namespace {
	struct Options {
		RegKey key;
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
			o.key = RegKey::CLASSES_ROOT;
		}
		else if (key == L"hkcu") {
			o.key = RegKey::CURRENT_USER;
		}
		else if (key == L"hklm") {
			o.key = RegKey::LOCAL_MACHINE;
		}
		else if (key == L"hku") {
			o.key = RegKey::USERS;
		}
		else if (key == L"hkcc") {
			o.key = RegKey::CURRENT_CONFIG;
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
