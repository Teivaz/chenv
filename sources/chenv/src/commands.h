// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once
#include <string>
#include <functional>
#include <list>

struct Command final {
	using Args = std::list<std::wstring>;
	using Function = std::function<void(Args&)>;
	std::wstring name;
	std::wstring description;
	Function function;
	Command(const wchar_t* name, const wchar_t* description, Function function);
};

void invokeCommand(std::wstring command, Command::Args&);
