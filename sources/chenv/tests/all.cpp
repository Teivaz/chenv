#include "gtest/gtest.h"

#include "command_which.h"
#include "command_get.h"
#include "command_getreg.h"

TEST(CommandGet, Simple) {
	auto var = command_get(L"username");
	EXPECT_TRUE((bool)var);
	EXPECT_TRUE(var.mData.size() > 0);
}

TEST(CommandGet, Path) {
	auto var = command_get(L"path");
	EXPECT_TRUE((bool)var);
	EXPECT_TRUE(var.mData.size() > 1);
}

TEST(EnvVar, Compare) {
	EnvVar var1 = command_get(L"username");
	EnvVar var2 = command_get(L"username");
	EXPECT_TRUE(var1 == var2);
	EnvVar var3 = command_get(L"OS");
	EXPECT_FALSE(var1 == var3);
}

TEST(EnvVar, CompareEmpty) {
	EnvVar var1;
	EnvVar var2;
	EXPECT_TRUE(var1 == var2);
	EnvVar var3 = command_get(L"username");
	EXPECT_FALSE(var1 == var3);
}

TEST(CommandGet, CaseInsensitivity) {
	auto var1 = command_get(L"USERNAME");
	EXPECT_TRUE((bool)var1);
	auto var2 = command_get(L"username");
	EXPECT_TRUE(var1 == var2);
}

TEST(ComandGetReg, Path) {
	auto varReg = command_getreg(Reg::Key::CURRENT_USER, L"Environment", L"path");
	EXPECT_TRUE((bool)varReg);
	EXPECT_TRUE(varReg.mType == EnvVar::Type::ExpandableString);
	auto varEnv = command_get(L"path");
	// Local path environment is a concatenation of user path and system path.
	EXPECT_TRUE(varEnv.data().length() >= varReg.data().length());
}

TEST(ComandGetReg, ComplexSubKey) {
	auto varReg = command_getreg(Reg::Key::LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", L"path");
	EXPECT_TRUE((bool)varReg);
}

TEST(ComandGetReg, SystemPath) {
	auto varReg = command_getreg(Reg::Key::LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", L"path");
	EXPECT_TRUE(varReg.mType == EnvVar::Type::ExpandableString);
	auto varEnv = command_get(L"path");
	EXPECT_TRUE(varReg.data().length() >= varEnv.data().length());
}
