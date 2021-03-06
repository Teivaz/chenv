// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#include "commands.h"
#include "exceptions.h"
#include "printer.h"

Printer P;

#ifdef _DEBUG
#include "gtest/gtest.h"
#endif

#include <Windows.h>

namespace {
	auto usageString =
		L"chenv <command> [<args>]";

	void handle_args(Command::Args& args) {
		while (args.size() > 0) {
			if (args.front().at(0) != L'-') {
				return;
			}
			// Put further arguments here
			P.usage(usageString);
			return;
		}
	}
}

int wmain(int argc, wchar_t* argv[]) {
#ifdef _DEBUG1
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
#endif
	try {
		Command::Args args;
		for (int a = 0; a < argc; a++) {
			args.push_back({ argv[a] });
		}
		args.pop_front(); // Consume the executable name
		handle_args(args);
		if (args.size() > 1) {
			invokeCommand(args.front(), args);
		}
	}
	catch (ChenvExceptionBase& e) {
		auto s = e.message();
		if (!s.empty()) P.print(s);
		return e.exitCode();
	}
	return 0;
}

