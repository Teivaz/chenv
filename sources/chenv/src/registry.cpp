#include "registry.h"
#include <map>
#include <vector>
#include <Windows.h>
#include <KtmW32.h>

using namespace Reg;

namespace {

	std::map<Key, HKEY> kKey2Handle{
		{ Key::CLASSES_ROOT, HKEY_CLASSES_ROOT },
		{ Key::CURRENT_CONFIG, HKEY_CURRENT_CONFIG },
		{ Key::CURRENT_USER, HKEY_CURRENT_USER },
		{ Key::LOCAL_MACHINE, HKEY_LOCAL_MACHINE },
		{ Key::USERS, HKEY_USERS },
	};
}

KeyHandle::KeyHandle(Key hKey, const std::wstring& lpSubKey, REGSAM samDesired) {
	auto result = RegOpenKeyEx(kKey2Handle[hKey], lpSubKey.c_str(), 0, samDesired, &mHandle);
	if (result != ERROR_SUCCESS) {
		// TODO: Throw exception.
		// Format message https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351(v=vs.85).aspx
	}
}

std::wstring KeyHandle::getValue(const std::wstring& valueName, bool expand) const {
	DWORD valueType = 0;
	DWORD dataSize = 0;
	std::vector<wchar_t> buffer;
	DWORD allowedTypes = RRF_RT_REG_EXPAND_SZ | RRF_RT_REG_SZ | (RRF_NOEXPAND * !expand);

	auto result = RegGetValue(mHandle, nullptr, valueName.c_str(), allowedTypes, &valueType, nullptr, &dataSize);
	if (result != ERROR_SUCCESS) {
		if (result == ERROR_FILE_NOT_FOUND) {
			return {};
		}
		// TODO: Throw last error
	}
	buffer.resize(dataSize);
	result = RegGetValue(mHandle, nullptr, valueName.c_str(), allowedTypes, &valueType, buffer.data(), &dataSize);
	if (result != ERROR_SUCCESS) {
		// TODO: Throw last error
	}
	return { buffer.data() };
}

KeyHandle::~KeyHandle() {
	if (mHandle) {
		RegCloseKey(mHandle);
	}
	mHandle = nullptr;
}

TKeyHandle::TKeyHandle(TransactionHandle& transaction, Key hKey, const std::wstring& lpSubKey, REGSAM samDesired) {
	auto result = RegOpenKeyTransacted(kKey2Handle[hKey], lpSubKey.c_str(), 0, samDesired, &mHandle, transaction, NULL);
	if (result != ERROR_SUCCESS) {
		// TODO: Throw exception.
		// Format message https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351(v=vs.85).aspx
	}
}

TKeyHandle::~TKeyHandle() {
	if (mHandle) {
		RegCloseKey(mHandle);
	}
	mHandle = nullptr;
}

TransactionHandle::TransactionHandle(DWORD timeout) {
	mHandle = CreateTransaction(NULL, 0, TRANSACTION_DO_NOT_PROMOTE, 0, 0, timeout, L"chenv registry operation");
	if (mHandle == INVALID_HANDLE_VALUE) {
		// TODO: Throw exception.
		// Format message https://msdn.microsoft.com/en-us/library/windows/desktop/aa366011(v=vs.85).aspx
	}
}

TransactionHandle::~TransactionHandle() {
	if (mHandle != INVALID_HANDLE_VALUE && mHandle != nullptr) {
		CloseHandle(mHandle);
	}
	mHandle = nullptr;
}

void TransactionHandle::commit() {
	auto result = CommitTransaction(mHandle);
	if (result) {

	}
}
