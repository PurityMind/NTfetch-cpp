#include "ntinfo.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winternl.h>

#include <cstdint>
#include <vector>
#include <string>


using RtlGetVersionPtr = NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOW);

namespace sysinfo
{
	namespace detail
	{
		static RTL_OSVERSIONINFOW get_os_version_info()
		{
			RTL_OSVERSIONINFOW os_info = {};
			os_info.dwOSVersionInfoSize = sizeof(os_info);
			HMODULE hModule = GetModuleHandleW(L"ntdll.dll");
			if (hModule)
			{
				RtlGetVersionPtr rtl_get_version = reinterpret_cast<RtlGetVersionPtr>(GetProcAddress(hModule, "RtlGetVersion"));
				if (rtl_get_version)
				{
					rtl_get_version(&os_info);
				}
			}
			return os_info;
		}

		static std::string trim(const std::string& str)
		{
			const std::string whitespace = " \t\n\r";
			const auto start = str.find_first_not_of(whitespace);
			if (start == std::string::npos)
				return "";
			const auto end = str.find_last_not_of(whitespace);
			return str.substr(start, end - start + 1);
		}
	}

	[[nodiscard]] std::string get_username()
	{
		DWORD length = 0;
		
		GetUserNameA(nullptr, &length);
		if (length == 0) return "unknown";

		std::string buffer(length, '\0');
		if (GetUserNameA(buffer.data(), &length))
		{
			buffer.resize(length > 0 ? length - 1 : 0);
			return buffer;
		}
		return "unknown";
	}

	[[nodiscard]] std::string get_hostname()
	{
		DWORD length = 0;
		GetComputerNameExA(ComputerNamePhysicalDnsHostname, nullptr, &length);
		if (length == 0) return "unknown";

		std::string buf(length, '\0');
		if (GetComputerNameExA(ComputerNamePhysicalDnsHostname, buf.data(), &length))
		{
			buf.resize(length);
			return buf;
		}

		return "unknown";
	}

	[[nodiscard]] std::string get_os_name()
	{
		RTL_OSVERSIONINFOW os_info = detail::get_os_version_info();
		switch (os_info.dwMajorVersion)
		{
		case 10:
			return (os_info.dwBuildNumber >= 22000) ? "Windows 11" : "Windows 10";
		case 6:
			switch (os_info.dwMinorVersion)
			{
			case 3:
				return "Windows 8.1";
			case 2:
				return "Windows 8";
			case 1:
				return "Windows 7";
			case 0:
				return "Windows Vista";
			default:
				return "Unknown Windows version";
			}
		case 5:
			switch (os_info.dwMinorVersion)
			{
			case 2:
				return "Windows Server 2003/XP x64";
			case 1:
				return "Windows XP";
			case 0:
				return "Windows 2000";
			default:
				return "Unknown Windows version";
			}
		default:
			return "Unknown Windows version";
		}
	}

	[[nodiscard]] std::string get_os_version()
	{
		RTL_OSVERSIONINFOW os_info = detail::get_os_version_info();
		return std::to_string(os_info.dwMajorVersion) + "." +
			std::to_string(os_info.dwMinorVersion) + "." +
			std::to_string(os_info.dwBuildNumber);
	}

	[[nodiscard]] std::string get_os_architecture()
	{
		SYSTEM_INFO sys_info;
		GetNativeSystemInfo(&sys_info);
		switch (sys_info.wProcessorArchitecture)
		{
		case PROCESSOR_ARCHITECTURE_AMD64:
			return "x64";
		case PROCESSOR_ARCHITECTURE_INTEL:
			return "x86";
		case PROCESSOR_ARCHITECTURE_ARM:
			return "ARM";
		case PROCESSOR_ARCHITECTURE_ARM64:
			return "ARM64";
		default:
			return "Unknown architecture";
		}
	}

	[[nodiscard]] std::string get_cpu_info()
	{
		int cpu_info[4] = { -1 };
		char cpu_brand[0x40] = { 0 };
		__cpuid(cpu_info, 0x80000000);
		unsigned int n_ex_ids = cpu_info[0];
		for (unsigned int i = 0x80000000; i <= n_ex_ids; ++i)
		{
			__cpuid(cpu_info, i);
			if (i == 0x80000002)
				memcpy(cpu_brand, cpu_info, sizeof(cpu_info));
			else if (i == 0x80000003)
				memcpy(cpu_brand + 16, cpu_info, sizeof(cpu_info));
			else if (i == 0x80000004)
				memcpy(cpu_brand + 32, cpu_info, sizeof(cpu_info));
		}
		return detail::trim(std::string(cpu_brand));
	}
}