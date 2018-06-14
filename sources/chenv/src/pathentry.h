// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once
#include <string>

struct PathEntry {
	enum class Source {
		LocalChanges,
		CurrentDirectory,
		User,
		System,
	};

	PathEntry() {}
	PathEntry(const std::wstring& path, Source source = Source::LocalChanges) : mPath(path), mSource(source) {}

	static std::wstring toString(Source s) {
		if (s == Source::LocalChanges) return L"l";
		if (s == Source::CurrentDirectory) return L"c";
		if (s == Source::User) return L"u";
		if (s == Source::System) return L"s";
		return {};
	}

	std::wstring sourceStr() const { return toString(mSource); }
	const std::wstring& path() const { return mPath; }

	Source mSource = Source::LocalChanges;
	std::wstring mPath;

	std::wstring toString() const {
		return sourceStr() + L"\t" + path();
	}
};
