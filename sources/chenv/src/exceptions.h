// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once
#include <string>

struct ChenvExceptionBase {
	virtual int exitCode() { return 1; }
	virtual std::wstring message() { return {}; }
};

struct ChenvExceptionNone : public ChenvExceptionBase {};

struct ChenvExceptionUsage : public ChenvExceptionBase {
	ChenvExceptionUsage(std::wstring content) : usage(content) {}
	std::wstring message() override { return usage; }
	std::wstring usage;
};
