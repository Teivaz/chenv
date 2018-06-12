#include "command_path.h"
#include "envvar.h"
#include "commands.h"
#include "command_getreg.h"
#include "command_get.h"
#include "exceptions.h"
#include "printer.h"

EnvVar command_path(EnvVar::Origin env) {
	EnvVar path;
	if (env == EnvVar::Origin::Local) {
		path = command_get(L"path");
	}
	if (env == EnvVar::Origin::RegSystem || env == EnvVar::Origin::RegEffective) {
		path = path | command_getreg(RegKey::LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", L"path", true);
	}
	if (env == EnvVar::Origin::RegUser || env == EnvVar::Origin::RegEffective) {
		path = path | command_getreg(RegKey::CURRENT_USER, L"Environment", L"path", true);
	}
	return path;
}

EnvVar command_pathext(EnvVar::Origin env) {
	EnvVar pathext;
	if (env == EnvVar::Origin::Local) {
		pathext = command_get(L"pathext");
	}
	if (env == EnvVar::Origin::RegSystem || env == EnvVar::Origin::RegEffective) {
		pathext |= command_getreg(RegKey::LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", L"pathext", true);
	}
	if (env == EnvVar::Origin::RegUser || env == EnvVar::Origin::RegEffective) {
		auto pathextUser = command_getreg(RegKey::CURRENT_USER, L"Environment", L"pathext", true);
		if (pathextUser) {
			pathext = pathextUser;
		}
	}
	return pathext;
}

namespace {
	struct Options {
		EnvVar::Origin pathOrigin;
	};

	auto usageString =
		L"chenv path [--local|--effective|--user|--system]";

	Options handle_args(Command::Args& args) {
		Options o;
		if (args.size() > 1) {
			throw ChenvExceptionUsage(usageString);
		}
		if (args.size() > 0) {
			if (args.front() == L"--local") {
				o.pathOrigin = EnvVar::Origin::Local;
			}
			else if (args.front() == L"--effective") {
				o.pathOrigin = EnvVar::Origin::RegEffective;
			}
			else if(args.front() == L"--user") {
				o.pathOrigin = EnvVar::Origin::RegUser;
			}
			else if(args.front() == L"--system") {
				o.pathOrigin = EnvVar::Origin::RegSystem;
			}
			else {
				throw ChenvExceptionUsage(usageString);
			}
		}
		return o;
	}

	void _command_path(Command::Args& args) {
		auto o = handle_args(args);
		auto var = command_path(o.pathOrigin);
		P.result(var.toString(true).c_str());
	}

	Command get = {
		L"path",
		L"Prints the environment PATH variable.",
		_command_path
	};
}
