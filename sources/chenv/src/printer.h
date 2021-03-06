// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once
#include <list>

struct Printer {
	void usage(const std::wstring& str) {
		printf("usage: %ls\n", str.c_str());
	}

	// For accompanying info
	void print(const std::wstring& str) {
		printf("%ls\n", str.c_str());
	}

	// For operations that only print
	void result(const std::wstring& str) {
		printf("%ls\n", str.c_str());
	}

	// When command result unsuccessful
	void fail(const std::wstring& str) {
		printf("%ls\n", str.c_str());
	}
};

extern Printer P;
