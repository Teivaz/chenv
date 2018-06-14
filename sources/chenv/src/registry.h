// Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
// Distributed under the terms of the GNU GPLv3 License.The full license
// is in the file LICENSE, distributed as part of this software.

#pragma once

#include <string>

#if 0 // Enable if Windows API changes or it fails to compile
#include <Windows.h>
#else
struct HKEY__;
using HKEY = HKEY__*;
using HANDLE = void*;
using DWORD = unsigned long;
using REGSAM = DWORD;
#endif

namespace Reg {
	enum class Key {
		CLASSES_ROOT,
		CURRENT_CONFIG,
		CURRENT_USER,
		LOCAL_MACHINE,
		USERS,
	};
	struct TransactionHandle;
	struct KeyHandle;

	struct KeyHandle {
		KeyHandle(Key hKey, const std::wstring& lpSubKey, REGSAM samDesired);
		~KeyHandle();
		std::wstring getValue(const std::wstring& valueName, bool expand = true) const;
		operator HKEY() const { return mHandle; }
	private:
		HKEY mHandle = nullptr;
		KeyHandle operator =(KeyHandle) = delete;
		KeyHandle(const KeyHandle&) = delete;
	};

	struct TKeyHandle {
		TKeyHandle(TransactionHandle& transaction, Key hKey, const std::wstring& lpSubKey, REGSAM samDesired);
		~TKeyHandle();
	private:
		HKEY mHandle = nullptr;
		TKeyHandle operator =(TKeyHandle) = delete;
		TKeyHandle(const TKeyHandle&) = delete;
	};

	struct TransactionHandle {
		TransactionHandle(DWORD timeout = 0);
		~TransactionHandle();
		operator HANDLE() const { return mHandle; }
		void commit();
	private:
		HANDLE mHandle = nullptr;
		TransactionHandle operator =(TransactionHandle) = delete;
		TransactionHandle(const TransactionHandle&) = delete;
	};

}
