#include "ntinfo.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <string_view>

namespace
{
	constexpr std::string_view yellow = "\x1b[38;2;249;241;165m";
	constexpr std::string_view cyan = "\x1b[38;2;97;214;214m";
	constexpr std::string_view reset = "\x1b[0m";
}

int main()
{
	const std::string username = sysinfo::get_username();
	const std::string hostname = sysinfo::get_hostname();

	std::cout << yellow << username << reset
		<< '@'
		<< cyan << hostname << reset << '\n';

	const std::size_t title_len = username.length() + 1 + hostname.length();
	std::cout << std::string(title_len, '-') << '\n';

	std::vector<std::string> os_info = {
		"OS: " + sysinfo::get_os_name() + " (" + sysinfo::get_os_architecture() + ")",
		"OS Version: " + sysinfo::get_os_version(),
		"CPU: " + sysinfo::get_cpu_info(),
	};

	for (const auto& os : os_info)
	{
		std::cout << os << '\n';
	}
}