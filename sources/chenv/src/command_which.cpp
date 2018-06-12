#include "command_which.h"
#include "command_get.h"
#include "command_getreg.h"
#include "command_path.h"
#include "commands.h"
#include "exceptions.h"
#include "printer.h"

#include <Windows.h>
#include <Shlwapi.h>

namespace {
	// TODO: remove duplicate
	bool FileExists(const std::wstring& fileName) {
		DWORD dwAttrib = GetFileAttributes(fileName.c_str());
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	std::wstring GetCwd() {
		auto bufferSize = GetCurrentDirectory(0, nullptr);
		std::vector<wchar_t> buffer;
		buffer.resize(bufferSize);
		auto written = GetCurrentDirectory(bufferSize, buffer.data());
		if (written == 0) {
			// TODO: Throw last error
		}
		if (written > bufferSize) {
			// TODO: Env var was changed. Try and read again maybe?
		}
		return { buffer.data() };
	}

	std::wstring hasFileInPathWithExtension(const std::wstring& p, const std::wstring& command, const std::wstring& ext) {
		std::wstring commandWithExt = command + ext;
		// Documentation recommends to have it MAX_PATH though new systems can have paths longer than that
		std::vector<wchar_t> combinedPath(p.length() + command.length() + ext.length() + 2);
		auto result = PathCombine(combinedPath.data(), p.c_str(), (command + ext).c_str());
		if (PathFileExists(result)) {
			return { combinedPath.data() };
		}
		return {};
	}

}



PathEntry command_which(std::wstring command, EnvVar::Origin env) {
	if (!PathIsFileSpec(command.c_str())) {
		// TODO: Search the path directly
	}
	else {
		auto path = command_path(env).dataAsList();

		/* Search order
		 *	Local directory
		 *	Each path from the PATH variable starting from the first
		 * In every directory search order is following:
		 *	If provided extension the extension should match
		 *	Otherwise each extension from the PATHEXT variable starting from the first is added to the name
		 */

		std::vector<std::wstring> paths = { GetCwd() };
		paths.insert(paths.end(), path.begin(), path.end());

		std::vector<std::wstring> usedExtensions{ {} };
		std::wstring ext{ PathFindExtension(command.c_str()) };
		if (ext.empty()) {
			usedExtensions = command_pathext(env).dataAsList();
		}
		for (const auto& p : paths) {
			for (const auto& ext : usedExtensions) {
				auto r = hasFileInPathWithExtension(p, command, ext);
				if (!r.empty()) return { r };
			}
		}
	}

	return {};
}

namespace {
	struct Options {
		std::wstring command;
		EnvVar::Origin pathOrigin = EnvVar::Origin::Local;
	};

	auto usageString =
		L"chenv which <variable> [--local|--effective|--user|--system]";

	Options handle_args(Command::Args& args) {
		Options o;
		if (args.size() < 1) {
			throw ChenvExceptionUsage(usageString);
		}
		o.command = args.front();
		while (args.size() > 1) {
			args.pop_front();
			if (args.front() == L"--local") {
				o.pathOrigin = EnvVar::Origin::Local;
			}
			else if (args.front() == L"--effective") {
				o.pathOrigin = EnvVar::Origin::RegEffective;
			}
			else if (args.front() == L"--user") {
				o.pathOrigin = EnvVar::Origin::RegUser;
			}
			else if (args.front() == L"--system") {
				o.pathOrigin = EnvVar::Origin::RegSystem;
			}
			else {
				throw ChenvExceptionUsage(usageString);
			}
		}
		return o;
	}
	void _command_which(Command::Args& args) {
		auto o = handle_args(args);
		auto result = std::wstring{};
		result = command_which(o.command, o.pathOrigin).path();

		if (result.empty()) {
			P.fail(L"Command was not found");
		}
		else {
			P.result(result);
		}
	}

	Command which = {
		L"which",
		L"Prints full path of the file that would be called.",
		_command_which
	};
}
