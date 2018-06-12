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
