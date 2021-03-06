// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#include "commands.h"
#include "exceptions.h"
#include <map>
#include <functional>
#include <cassert>

namespace {
	using CommandList = std::map<std::wstring, Command>;

	CommandList& getCommandList() {
		static CommandList cl;
		return cl;
	}

	void registerCommand(const Command& command) {
		assert(getCommandList().find(command.name) == getCommandList().end());
		getCommandList().emplace(command.name, command);
	}
}

Command::Command(const wchar_t* n, const wchar_t* d, Function f) :
	name(n), description(d), function(f) {
	registerCommand(*this);
}

void invokeCommand(std::wstring command, Command::Args& args) {
	auto& cl = getCommandList();
	auto cit = cl.find(command);
	if (cit == cl.end()) {
		//throw Usage;
	}
	else {
		args.pop_front();
		cit->second.function(args);
	}
}
