// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once
#include <string>
#include <vector>

std::wstring Lower(const std::wstring& input);

struct EnvVar {
	enum class Type {
		None,
		String,
		ExpandableString,
	};
	
	enum class Origin {
		Local,
		RegEffective,
		RegSystem,
		RegUser,
	};
	
	Type mType = Type::None;
	std::wstring mName;
	std::wstring mData;

	EnvVar() {}
	EnvVar(const std::wstring& name, const std::wstring& value);
	EnvVar(Type type, const std::wstring& name, const std::wstring& value);

	std::wstring toString(bool pretty = false) const;

	EnvVar getExpanded() const;

	operator bool() const { return mType != Type::None; }
	const std::wstring& data() const { return mData; }
	std::vector<std::wstring> dataAsList(std::wstring separator = L";") const;
	EnvVar& operator |= (const EnvVar& other);
};

EnvVar operator | (const EnvVar& a, const EnvVar& b);
bool operator == (const EnvVar& a, const EnvVar& b);
