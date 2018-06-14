// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#include "command_listpaths.h"
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

	// Search for x-y sequence in the a-b. Return the starting position of the matching sequence in a-b
	template<typename It>
	typename It::const_iterator contains(typename It::const_iterator a, typename It::const_iterator b, typename It::const_iterator x, typename It::const_iterator y) {
		for (; a != b; ++a) {
			auto ai = a;
			bool match = true;
			for (auto xi = x; xi != y; ++xi, ++ai) {
				if (ai == b) return b;
				if (*xi != *ai) {
					match = false;
					break;
				}
			}
			if (match) return a;
		}
		return b;
	}

}

std::vector<PathEntry> command_listpaths(std::wstring command) {
	if (!PathIsFileSpec(command.c_str())) {
		// TODO: Search the path directly
	}
	else {
		auto pathSystem = command_path(EnvVar::Origin::RegSystem).dataAsList();
		auto pathUser = command_path(EnvVar::Origin::RegUser).dataAsList();
		auto pathLocal = command_path(EnvVar::Origin::Local).dataAsList();

		std::vector<std::wstring> usedExtensions{ {} };
		std::wstring ext{ PathFindExtension(command.c_str()) };
		if (ext.empty()) {
			// TODO: this is not local...
			usedExtensions = command_pathext(EnvVar::Origin::Local).dataAsList();
		}

		/*
		 * 1. Search for [pathSystem, pathUser] in the pathLocal
		 * 2. If exist take first occurence and split as follows: [pathCurrentDir, pathLocal-before, pathSystem, pathUser, pathLocal-after]
		 * 3. Otherwise use [pathCurrentDir, pathLocal]
		 * 4. Search for all occurences of the command and store in order
		 */
		std::vector<PathEntry> path{ { GetCwd(), PathEntry::Source::CurrentDirectory } };
		path.reserve(pathLocal.size() + 1);
		auto pathSystemOffset = contains<decltype(pathLocal)>(std::cbegin(pathLocal), std::cend(pathLocal), std::cbegin(pathSystem), std::cend(pathSystem));
		if (pathSystemOffset != std::cend(pathLocal)) {
			auto pathUserOffset = contains<decltype(pathLocal)>(pathSystemOffset + pathSystem.size(), std::cend(pathLocal), std::cbegin(pathUser), std::cend(pathUser));
			if ( (pathUserOffset != std::cend(pathLocal)) && (pathSystemOffset + pathSystem.size() == pathUserOffset) ) {
				for (auto i = std::cbegin(pathLocal); i != pathSystemOffset; ++i) {
					path.push_back({ *i, PathEntry::Source::LocalChanges });
				}
				for (auto& i : pathSystem) {
					path.push_back({ i, PathEntry::Source::System });
				}
				for (auto& i : pathUser) {
					path.push_back({ i, PathEntry::Source::User });
				}
				for (auto i = (pathUserOffset + pathUser.size()); i != std::cend(pathLocal); ++i) {
					path.push_back({ *i, PathEntry::Source::LocalChanges });
				}
			}
		}
		else {
			for (auto i = std::cbegin(pathLocal); i != std::cend(pathLocal); ++i) {
				path.push_back({ *i, PathEntry::Source::LocalChanges });
			}
		}

		std::vector<PathEntry> result;
		for (const auto& p : path) {
			for (const auto& ext : usedExtensions) {
				auto r = hasFileInPathWithExtension(p.path(), command, ext);
				if (!r.empty()) {
					result.push_back({r, p.mSource});
				}
			}
		}
		return result;
	}
	return {};
}

namespace {
	struct Options {
		std::wstring command;
	};

	auto usageString =
		L"chenv listpaths <variable>";

	Options handle_args(Command::Args& args) {
		Options o;
		if (args.size() != 1) {
			throw ChenvExceptionUsage(usageString);
		}
		o.command = args.front();
		return o;
	}
	void _command_listpaths(Command::Args& args) {
		auto o = handle_args(args);
		auto result = std::wstring{};
		auto paths = command_listpaths(o.command);
		for (int i = 0; i < paths.size(); ++i) {
			result += std::to_wstring(i + 1) + L"\t" + paths[i].toString() + L"\n";
		}

		if (result.empty()) {
			P.fail(L"Command was not found");
		}
		else {
			P.result(result);
		}
	}

	Command listpaths = {
		L"listpaths",
		L"Prints the list of full paths to all visible occurences of the file that might be called.",
		_command_listpaths
	};
}
