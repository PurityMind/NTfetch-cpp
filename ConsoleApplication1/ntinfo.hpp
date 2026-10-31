#pragma once

#include <string>

namespace sysinfo
{
	[[nodiscard]] std::string get_username();
	[[nodiscard]] std::string get_hostname();
	[[nodiscard]] std::string get_uptime();
	[[nodiscard]] std::string get_os_name();
	[[nodiscard]] std::string get_os_version();
	[[nodiscard]] std::string get_os_architecture();
	[[nodiscard]] std::string get_cpu_info();
}