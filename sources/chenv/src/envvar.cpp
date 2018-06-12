#pragma once
#include "envvar.h"
#include <sstream>
#include <iterator>
#include <assert.h>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <assert.h>

#include <Windows.h>

std::wstring Lower(const std::wstring& input) {
	std::wstring result;
	result.resize(input.length());
	std::transform(input.begin(), input.end(), result.begin(), ::tolower);
	return result;
}

namespace {

	bool isListVariable(const std::wstring& name) {
		auto nameLower = Lower(name);
		return (nameLower == L"path") || (nameLower == L"pathext");// || (nameLower == L"");
	}

	std::wstring typeToString(EnvVar::Type type) {
		if (type == EnvVar::Type::None) return L"undefined";
		if (type == EnvVar::Type::String) return L"STRING";
		if (type == EnvVar::Type::ExpandableString) return L"EXPANDABLE STRING";
		assert(false);
		return {};
	}
}

std::vector<std::wstring> EnvVar::dataAsList(std::wstring separator) const {
	std::vector<std::wstring> result;
	std::wstring::size_type pos, lastPos = 0;
	auto length = data().length();
	while (lastPos < length + 1) {
		pos = data().find_first_of(separator, lastPos);
		if (pos == std::wstring::npos) {
			pos = length;
		}
		if (pos != lastPos) {
			result.push_back({ data().data() + lastPos, pos - lastPos });
		}
		lastPos = pos + 1;
	}
	return result;
}

EnvVar::EnvVar(const std::wstring& name, const std::wstring& value)
	: mType(Type::String)
	, mName(name)
	, mData(value)
{}

EnvVar::EnvVar(EnvVar::Type type, const std::wstring& name, const std::wstring& value) 
	: mType(type)
	, mName(name)
	, mData(value)
{}

EnvVar EnvVar::getExpanded() const {
	if (mType == Type::ExpandableString) {
		std::vector<wchar_t> buffer;
		auto size = ExpandEnvironmentStrings(data().c_str(), nullptr, 0);
		buffer.resize(size);
		auto writtenSize = ExpandEnvironmentStrings(data().c_str(), buffer.data(), size);
		if (writtenSize == NULL) {
			// TODO: Throw last error
		}
		if (writtenSize > size) {
			// TODO: The buffer is not big enough...
		}
		return { Type::String, mName, { buffer.data() } };
	}
	return { *this };
}

std::wstring EnvVar::toString(bool pretty) const {
	std::wostringstream result;

	const wchar_t* delim = L";";
	if (pretty) {
		const bool arrayHint = isListVariable(mName);
		result << L"Variable " << mName;
		if (arrayHint) {
			for (const auto& item : dataAsList()) {
				result << L"\n" << item;
			}
		}
		else {
			result << L" " << typeToString(mType);
		}
	}
	else {
		result << mData;
	}
	result << std::endl;
	return result.str();
}

EnvVar& EnvVar::operator |= (const EnvVar& other) {
	if (mType == EnvVar::Type::None) {
		*this = other;
	}
	else if (other.mType == EnvVar::Type::None) {
		// Nothing to do. Other is None
	}
	else if (mName != other.mName) {
		// TODO: Throw an eror saying "cannot merge different variables"
	}
	else if (mType != other.mType) {
		// TODO: Throw an error saying "Variables need to be either both strings or both expandable"
	}
	else if (other.data().empty()) {
		// Nothing to do. Other is empty
	}
	else if (Lower(mName) == L"path") {
		if (!mData.empty() && mData.back() != L';') {
			mData.push_back(L';');
		}
		mData += other.mData;
	}
	return *this;
}

EnvVar operator | (const EnvVar& a, const EnvVar& b) {
	EnvVar result;
	result |= a;
	result |= b;
	return result;
}

bool operator == (const EnvVar& a, const EnvVar& b) {
	std::wstring nameA, nameB;
	nameA.resize(a.mName.length());
	std::transform(a.mName.begin(), a.mName.end(), nameA.begin(), ::tolower);
	nameB.resize(b.mName.length());
	std::transform(b.mName.begin(), b.mName.end(), nameB.begin(), ::tolower);
	return (nameA == nameB) && (a.mType == b.mType) && (a.mData == b.mData);
}
